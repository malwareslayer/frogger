#include "tile.hpp"

auto tile(const TYPE &type, const int &y, const int &x) -> TILE {
    const auto tiling  = TILE {
        .type = type,
        .board = {
            .y = y,
            .x = x,
        },
        .speed = 0
    };

    return tiling;
}
