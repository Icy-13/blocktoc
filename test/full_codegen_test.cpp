#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <string>

#include "../src/codegen.h"
#include "../src/diagram_parser.h"

void checkGeneratorOutput(std::string filename) {
    std::string inputFile = "data/" + filename + ".xml";
    std::string outputFile = "data/" + filename + "Gen.h";
    std::string expectedFile = "data/" + filename + "Expected.h";

    DiagramParser parser;
    std::ifstream input(inputFile);
    REQUIRE(input.is_open());
    auto graph = parser.parse(input);
    input.close();

    Codegen codegen;
    auto instructions = codegen.generateCode(graph);

    std::ofstream output(outputFile);
    REQUIRE(output.is_open());
    std::ostringstream generatedOutput;
    for (const auto& instr : instructions) {
        generatedOutput << instr->emit() << "\n";
    }
    output << generatedOutput.str();
    output.close();

    std::ifstream expected(expectedFile);
    REQUIRE(expected.is_open());
    std::ostringstream expectedOutput;
    expectedOutput << expected.rdbuf();
    expected.close();

    REQUIRE(generatedOutput.str() == expectedOutput.str());
}

TEST_CASE("Sample test", "[full_codegen]") {
    checkGeneratorOutput("sample");
}

TEST_CASE("Easy test", "[full_codegen]") {
    checkGeneratorOutput("easy");
}

TEST_CASE("Add3 test", "[full_codegen]") {
    checkGeneratorOutput("add3");
}

TEST_CASE("Cycles test", "[full_codegen]") {
    checkGeneratorOutput("cycles");
}
