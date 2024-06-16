#pragma once

#include <string>
#include <unordered_map>

enum class ECapstoneArchMode {
    UNDEFINED,
    X86_16,
    X86_32,
    X86_64,
    ARM32_ARM,
    ARM32_THUMB,
    AARCH64_ARM,
};

static ECapstoneArchMode ECapstoneArchModeFromString(const std::string& str) {
    static const std::unordered_map<std::string, ECapstoneArchMode> strToEnumMap = {
        {"X86_16", ECapstoneArchMode::X86_16},
        {"X86_32", ECapstoneArchMode::X86_32},
        {"X86_64", ECapstoneArchMode::X86_64},
        {"ARM32_ARM", ECapstoneArchMode::ARM32_ARM},
        {"ARM32_THUMB", ECapstoneArchMode::ARM32_THUMB},
        {"AARCH64_ARM", ECapstoneArchMode::AARCH64_ARM},
    };

    auto it = strToEnumMap.find(str);
    if (it != strToEnumMap.end()) {
        return it->second;
    }
    
    return ECapstoneArchMode::UNDEFINED;
}