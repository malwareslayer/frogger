#include "tile.hpp"

auto tile(const TYPE &type, const int &y, const int &x) -> TILE {
    return TILE{.type = type,
                .board = {
                    .y = y,
                    .x = x,
                }};
}
