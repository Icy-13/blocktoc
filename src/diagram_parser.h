#pragma once

#include "i_diagram_parser.h"

class DiagramParser : public IDiagramParser {
public:
    Graph parse(std::istream& data) override;
    ~DiagramParser() override = default;
};