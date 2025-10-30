#pragma once

#include <istream>

#include "models/graph_model.h"

class DiagramParser {
public:
    Graph parse(std::istream& data);
};