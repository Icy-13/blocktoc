#pragma once

#include <istream>

#include "models/graph_model.h"

class IDiagramParser {
public:
    virtual Graph parse(std::istream& data) = 0;
    virtual ~IDiagramParser() = default;
};
