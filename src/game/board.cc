#include "board.hpp"

void moving(const TILE &tile, const INTERFACE &context, WINDOW *window) {
    switch (wgetch(window)) {
        case KEY_UP:
            if (tile.board.y > 1)
                tile.board.y--;
            break;
        case KEY_DOWN:
            if (tile.board.y < context.visual.height - tile.sprite.height)
                tile.board.y++;
            break;
        case KEY_LEFT:
            if (tile.board.x - tile.sprite.width > 1)
                tile.board.x++;
            break;
        case KEY_RIGHT:
            if (tile.board.x < context.visual.width - tile.sprite.width)
                tile.board.x--;
            break;
        default:
            break;
    }
}
