#pragma once

enum TYPE { PLAYER, SEPARATOR, RIGHT_CAR, LEFT_CAR, WATER, LILY, RIGHT_LOG, LEFT_LOG };

struct BOARD {
    mutable int y;
    mutable int x;
};

struct TILE {
    TYPE type;
    BOARD board;
};

auto tile(const TYPE &type, const int &y, const int &x) -> TILE;
