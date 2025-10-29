#include "codegen.h"

#include <unordered_set>
#include <vector>

using AdjacencyMap = std::unordered_map<std::size_t, std::vector<std::size_t>>;

void dfs_topsort(std::size_t id, const Graph& graph,
                 std::unordered_set<std::size_t>& used,
                 const AdjacencyMap& bannedEdges,
                 std::vector<std::size_t>& order) {
    used.insert(id);
    if (graph.adj.contains(id)) {
        for (const auto& [neighbor, _] : graph.adj.at(id)) {
            if (bannedEdges.contains(id)) {
                bool found = false;
                for (const auto& banned : bannedEdges.at(id)) {
                    if (neighbor == banned) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    continue;
                }
            }
            if (!used.contains(neighbor)) {
                dfs_topsort(neighbor, graph, used, bannedEdges, order);
            }
        }
    }
    order.push_back(id);
}

AdjacencyMap buildBannedEdges(const Graph& graph) {
    AdjacencyMap bannedEdges;
    for (const auto& [id, block] : graph.idToBlock) {
        if (impl::isMemoryType(block->type)) {
            for (const auto& input_id : block->inputs) {
                bannedEdges[input_id].push_back(id);
            }
        }
    }
    return bannedEdges;
}

std::vector<std::size_t> buildTopsort(const Graph& graph,
                                      const AdjacencyMap& bannedEdges) {
    std::unordered_set<std::size_t> used;
    std::vector<std::size_t> order;
    for (const auto& [node, _] : graph.adj) {
        if (!used.contains(node)) {
            dfs_topsort(node, graph, used, bannedEdges, order);
        }
    }
    std::reverse(order.begin(), order.end());
    return order;
}

void generateHeaders(std::vector<InstructionPtr>& instructions) {
    instructions.push_back(std::make_shared<Instruction>("#include <math.h>"));
    instructions.push_back(std::make_shared<Instruction>("#include \"state_run.h\"\n"));
}

void generateStruct(const Graph& graph, std::vector<InstructionPtr>& instructions) {
    instructions.push_back(std::make_shared<Instruction>("static struct {"));
    for (const auto& [id, block] : graph.idToBlock) {
        instructions.push_back(std::make_shared<Instruction>(
            std::format("    {} {};", "double", block->name)));
    }
    instructions.push_back(std::make_shared<Instruction>("} state;\n"));
}

void generateInitFunction(const Graph& graph,
                          std::vector<InstructionPtr>& instructions,
                          const std::vector<std::size_t>& order) {
    instructions.push_back(std::make_shared<Instruction>("void state_generated_init() {"));
    for (const auto& id : order) {
        auto block = graph.idToBlock.at(id);
        if (impl::isMemoryType(block->type)) {
            instructions.push_back(std::make_shared<Instruction>(
                std::format("    state.{} = 0.0;", block->name)));
        }
    }
    instructions.push_back(std::make_shared<Instruction>("}\n"));
}

void generateStepFunction(const Graph& graph,
                          std::vector<InstructionPtr>& instructions,
                          const std::vector<std::size_t>& order) {
    instructions.push_back(std::make_shared<Instruction>("void state_generated_step() {"));

    auto generateLine = [&](Graph::BlockPtr block) {
        std::string line = std::format("    state.{} = ", block->name);
        for (std::size_t i = 0; i < block->inputs.size(); ++i) {
            auto input_id = block->inputs[i];
            auto input_block = graph.idToBlock.at(input_id);
            if (block->params.contains("Gain")) {
                line += block->params["Gain"] + " ";
            }
            std::string_view operation;
            if (block->params.contains("Inputs")) {
                operation = std::string{block->params["Inputs"][i]};
            } else {
                operation = impl::getOperation(block->type);
            }
            line += std::format("{} state.{} ", operation, input_block->name);
            if (i == block->inputs.size() - 1) {
                line += ";";
            }
        }
        instructions.push_back(std::make_shared<Instruction>(line));
    };

    std::vector<std::size_t> memoryBlocks;
    for (const auto& id : order) {
        auto block = graph.idToBlock.at(id);
        if (block->inputs.size() == 0) {
            continue;
        }
        if (impl::isMemoryType(block->type)) {
            memoryBlocks.push_back(id);
            continue;
        }
        generateLine(block);
    }

    instructions.push_back(std::make_shared<Instruction>("    // Update memory blocks"));
    for (const auto& id : memoryBlocks) {
        generateLine(graph.idToBlock.at(id));
    }

    instructions.push_back(std::make_shared<Instruction>("}\n"));
}

void generateTable(const Graph& graph,
                   std::vector<InstructionPtr>& instructions,
                   const std::vector<std::size_t>& order) {
    instructions.push_back(std::make_shared<Instruction>("static const state_ExtPort ext_ports[] = {"));

    auto generateEntry = [&](Graph::BlockPtr block, bool isInput) {
        instructions.push_back(std::make_shared<Instruction>(
            std::format("    {{\"{}\", &state.{}, {} }},", block->name, block->name, isInput ? "1" : "0")));
    };

    std::unordered_map<std::size_t, std::size_t> outDegree;
    for (const auto& id : order) {
        auto block = graph.idToBlock.at(id);
        if (block->inputs.size() == 0) {
            generateEntry(block, true);
        }
        for (const auto& input_id : block->inputs) {
            outDegree[input_id]++;
        }
    }
    for (const auto& id : order) {
        if (outDegree[id] == 0) {
            generateEntry(graph.idToBlock.at(id), false);
        }
    }
    instructions.push_back(std::make_shared<Instruction>("    { 0, 0, 0 },"));
    instructions.push_back(std::make_shared<Instruction>("};\n"));
    instructions.push_back(std::make_shared<Instruction>(
        "const state_ExtPort * const state_generated_ext_ports = ext_ports;"));
    instructions.push_back(std::make_shared<Instruction>(
        "const size_t state_generated_ext_ports_size = sizeof(ext_ports);"));
}

std::vector<InstructionPtr> Codegen::generateCode(const Graph& graph) {
    std::vector<InstructionPtr> instructions;
    generateHeaders(instructions);
    generateStruct(graph, instructions);

    auto bannedEdges = buildBannedEdges(graph);
    auto order = buildTopsort(graph, bannedEdges);
    
    generateInitFunction(graph, instructions, order);
    generateStepFunction(graph, instructions, order);
    generateTable(graph, instructions, order);

    return instructions;
}
