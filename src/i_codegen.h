#pragma once

#include "models/graph_model.h"
#include "models/codegen_model.h"

class ICodegen {
public:
    virtual std::vector<InstructionPtr> generateCode(const Graph& graph) = 0;
    virtual ~ICodegen() = default;
};  