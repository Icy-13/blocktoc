#pragma once

#include <vector>

#include "i_codegen.h"

class Codegen : public ICodegen {
public:
    std::vector<InstructionPtr> generateCode(const Graph& graph) override;
};