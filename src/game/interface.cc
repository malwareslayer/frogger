#include "../../src/game/interface.hpp"

auto keypad(WINDOW *window, const INTERFACE &context, const TILE &tile) -> std::string {
    switch (wgetch(window)) {
        case 'q':
            return "q";
        case KEY_UP:
            if (tile.board.y > 1)
                tile.board.y--;
            break;
        case KEY_DOWN:
            if (tile.board.y < context.visual.height - tile.sprite.height)
                tile.board.y++;
            break;
        case KEY_LEFT:
            if (tile.board.x > 1)
                tile.board.x = tile.board.x - 2;
            break;
        case KEY_RIGHT:
            if (tile.board.x < context.visual.width - 10)
                tile.board.x = tile.board.x + 2;
            break;
        default:
            break;
    }

    return std::string{""};
}
