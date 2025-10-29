#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Block {
    std::size_t id;
    std::string name;
    std::string type;
    std::unordered_map<std::string, std::string> params;
    std::vector<std::size_t> inputs;
};

struct Graph {
    using BlockPtr = std::shared_ptr<Block>;
    using PortConnection = std::pair<std::size_t, std::size_t>;

    std::unordered_map<std::size_t, BlockPtr> idToBlock;
    std::unordered_map<std::size_t, std::vector<PortConnection>> adj;
};
