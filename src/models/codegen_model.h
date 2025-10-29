#pragma once

#include <string>
#include <memory>

class IInstruction {
public:
    virtual std::string emit() const = 0;
    virtual ~IInstruction() = default;
};

using InstructionPtr = std::shared_ptr<IInstruction>;

class Instruction : public IInstruction {
    std::string code;
public:
    Instruction(const std::string& c) : code(c) {}
    Instruction(std::string&& c) : code(std::move(c)) {}

    std::string emit() const override {
        return code;
    }

    ~Instruction() override = default;
};

namespace impl {

constexpr bool isMemoryType(std::string_view type) {
    return type == "UnitDelay";
}

constexpr std::string_view getOperation(
    std::string_view blockType,
    std::string_view defaultValue = "") {
    static const std::unordered_map<std::string, std::string> ops = {
        {"Sum", "+"}, {"Gain", "*"}};
    auto it = ops.find(std::string(blockType));
    return it != ops.end() ? it->second : defaultValue;
}

}  // namespace impl
