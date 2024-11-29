#include "../../src/game/interface.hpp"

auto keypad(WINDOW *window, const INTERFACE &context, const SPRITE &sprite, const TILE &tile) -> std::string {
    switch (wgetch(window)) {
        case 'q':
            return "q";
        case KEY_UP:
            if (tile.board.y >= 0) {
                tile.board.y = tile.board.y - sprite.height - 1;

                if (tile.board.y <= 0) {
                    tile.board.y = 0;
                }
            }
            break;
        case KEY_DOWN:
            if (tile.board.y + sprite.height < context.visual.height) {
                tile.board.y = tile.board.y + sprite.height + 1;

                if (tile.board.y + sprite.height > context.visual.height) {
                    tile.board.y = context.visual.height - sprite.height - 1;
                }
            }
            break;
        case KEY_LEFT:
            if (tile.board.x >= 0) {
                tile.board.x = tile.board.x - sprite.width;

                if (tile.board.x <= 0) {
                    tile.board.x = 0;
                }
            }
            break;
        case KEY_RIGHT:
            if (tile.board.x <= context.visual.width) {
                tile.board.x = tile.board.x + sprite.width;

                if (tile.board.x + sprite.width >= context.visual.width) {
                    tile.board.x = context.visual.width - sprite.width;
                }
            }
            break;
        default:
            break;
    }

    return std::string{""};
}
