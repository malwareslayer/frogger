#pragma once

#include <array>
#include <string>

struct SPRITE {
    mutable std::array<char, 512> symbol;
    mutable int height;
    mutable int width;
};

auto sprite(const std::string &data) -> SPRITE;
