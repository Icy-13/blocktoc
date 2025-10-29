#include <fstream>
#include <iostream>

#include "codegen.h"
#include "cxxopts.hpp"
#include "diagram_parser.h"
#include "models/codegen_model.h"
#include "models/graph_model.h"

template <typename Stream>
bool checkStream(const Stream& stream, const std::string& filename) {
    if (!stream.is_open()) {
        std::cerr << std::format("Failed to open file: {}", filename) << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("BlockToC", "Translates block diagram from XML to C");

    // clang-format off
    options.add_options()
        ("input", "Input XML file", cxxopts::value<std::string>())
        ("o,output", "Output C file", cxxopts::value<std::string>())
        ("h,help", "Show help");
    // clang-format on

    options.parse_positional({"input"});
    options.positional_help("<input-file>");
    auto result = options.parse(argc, argv);
    if (result.count("help") || !result.count("input") || !result.count("output")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    std::string input = result["input"].as<std::string>();
    std::string output = result["output"].as<std::string>();

    std::ifstream infile(input);
    std::ofstream outfile(output);

    if (!checkStream(infile, input) || !checkStream(outfile, output)) {
        return 1;
    }

    auto parser = DiagramParser();
    auto codegen = Codegen();
    try {
        Graph graph = parser.parse(infile);
        auto instructions = codegen.generateCode(graph);
        for (const auto& instr : instructions) {
            outfile << instr->emit() << std::endl;
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
