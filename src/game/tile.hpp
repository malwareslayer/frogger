#pragma once

enum TYPE { TFROG, TCAR, TSEPARATOR };

struct BOARD {
    mutable int y;
    mutable int x;
};

struct TILE {
    TYPE type;
    BOARD board;
};

auto tile(const TYPE &type, const int &y, const int &x) -> TILE;
