#include "tile.hpp"

auto tile(const SPRITE &sprite, const int &y, const int &x) -> TILE {
    return TILE{
        .board =
            {
                .y = y,
                .x = x,
            },
        .sprite = sprite,
    };
}
