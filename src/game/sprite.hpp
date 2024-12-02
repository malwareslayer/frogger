#pragma once

#include <array>
#include <string>
#include <vector>

struct SPRITE {
    mutable std::vector<std::string> symbol;
    mutable int height;
    mutable int width;
    mutable int track = 0;
    mutable SPRITE* next = nullptr;
};

auto symbolize(const std::array<char, 512> &source) -> std::vector<std::string>;

auto sprite(const std::string &data) -> SPRITE;
