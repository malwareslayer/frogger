#pragma once

#include "sprite.hpp"

struct BOARD {
    mutable int y;
    mutable int x;
};

struct TILE {
    BOARD board;
    SPRITE sprite;
};

auto tile(const SPRITE &sprite, const int &y, const int &x) -> TILE;
