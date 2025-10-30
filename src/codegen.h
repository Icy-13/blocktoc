#pragma once

#include <vector>

#include "models/graph_model.h"
#include "models/codegen_model.h"

class Codegen {
public:
    std::vector<InstructionPtr> generateCode(const Graph& graph);
};