#include "diagram_parser.h"

#include <format>
#include <iostream>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string_view>

#include "pugixml.hpp"

struct Connection {
    std::size_t block_id;
    std::size_t port_index;
    bool is_input;
};

std::optional<Connection> parseConnection(const std::string& data) {
    // {block_id}#{in/out}:{port_index}
    auto hash_pos = data.find('#');
    auto colon_pos = data.find(':');
    if (hash_pos == std::string::npos || colon_pos == std::string::npos) {
        return std::nullopt;
    }
    try {
        Connection conn{};
        conn.block_id = std::stoull(data.substr(0, hash_pos));
        conn.port_index = std::stoull(data.substr(colon_pos + 1));
        auto direction = data.substr(hash_pos + 1, colon_pos - hash_pos - 1);
        if (direction == "in") {
            conn.is_input = true;
        } else if (direction == "out") {
            conn.is_input = false;
        } else {
            return std::nullopt;
        }
        return conn;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

void parseConnections(std::vector<Connection>& srcs, std::vector<Connection>& dsts, pugi::xml_node node) {
    for (const auto& p_node : node.children("P")) {
        std::string_view name = p_node.attribute("Name").as_string();
        if (name == "Src" || name == "Dst") {
            auto conn = parseConnection(p_node.text().as_string());
            if (!conn) {
                continue;
            }
            if (name == "Src") {
                srcs.emplace_back(*conn);
            } else {
                dsts.emplace_back(*conn);
            }
        }
    }
}

Graph DiagramParser::parse(std::istream& data) {
    pugi::xml_document initialDoc;
    auto result = initialDoc.load(data);
    if (!result) {
        throw std::runtime_error(std::format("Failed to parse XML: {}\n Error offset: {}",
                                             result.description(), result.offset));
    }
    auto doc = initialDoc.child("System");
    if (doc.type() == pugi::node_null) {
        throw std::runtime_error("No <System> root node found in XML.");
    }

    Graph graph;
    for (const auto& node : doc.children("Block")) {
        auto block = std::make_shared<Block>();
        block->id = node.attribute("SID").as_ullong();
        block->name = node.attribute("Name").as_string();
        block->type = node.attribute("BlockType").as_string();
        if (block->id == 0 || block->name.empty() || block->type.empty()) {
            continue;
        }
        // replace all spaces with _
        block->name = std::regex_replace(block->name, std::regex(" "), "_");

        for (const auto& p_node : node.children("P")) {
            auto name = p_node.attribute("Name").as_string();
            block->params[name] = p_node.text().as_string();
        }

        graph.idToBlock[block->id] = block;
    }

    for (const auto& node : doc.children("Line")) {
        std::vector<Connection> srcs, dsts;
        parseConnections(srcs, dsts, node);
        for (const auto& branch : node.children("Branch")) {
            parseConnections(srcs, dsts, branch);
        }
        for (const auto& src : srcs) {
            for (const auto& dst : dsts) {
                if (src.is_input || !dst.is_input) {
                    continue;
                }
                auto dst_block_it = graph.idToBlock.find(dst.block_id);
                if (!graph.idToBlock.contains(src.block_id) || dst_block_it == graph.idToBlock.end()) {
                    continue;
                }
                dst_block_it->second->inputs.push_back(src.block_id);
                graph.adj[src.block_id].emplace_back(dst.block_id, dst.port_index);
            }
        }
    }
    return graph;
}