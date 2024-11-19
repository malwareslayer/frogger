#include "../../src/game/interface.hpp"

auto keypad(WINDOW *window, const INTERFACE &context, const SPRITE &sprite, const TILE &tile) -> std::string {
    switch (wgetch(window)) {
        case 'q':
            return "q";
        case KEY_UP:
            if (tile.board.y > 1) {
                tile.board.y = tile.board.y - sprite.height - 1;

                if (tile.board.y < 1) {
                    tile.board.y = 3;
                }
            }
            break;
        case KEY_DOWN:
            if (tile.board.y + sprite.height < context.visual.height) {
                tile.board.y = tile.board.y + sprite.height + 1;

                if (tile.board.y + sprite.height > context.visual.height) {
                    tile.board.y = context.visual.height - 1 - sprite.height;
                }
            }
            break;
        case KEY_LEFT:
            if (tile.board.x > 1) {
                tile.board.x = tile.board.x - sprite.width - 1;

                if (tile.board.x < 1) {
                    tile.board.x = 0;
                }
            }
            break;
        case KEY_RIGHT:
            if (tile.board.x + sprite.width < context.visual.width) {
                tile.board.x = tile.board.x + sprite.width + 1;

                if (tile.board.x + sprite.width > context.visual.width) {
                    tile.board.x = context.visual.width - 1 - sprite.width;
                }
            }
            break;
        default:
            break;
    }

    return std::string{""};
}
