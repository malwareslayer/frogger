#include "game.hpp"

#include "../game/board.hpp"
#include "../game/interface.hpp"
#include "../game/symbols/frog.hpp"
#include "../game/tile.hpp"

#include <sstream>
#include <thread>

auto symbolize(const std::array<char, 512> &symbol) -> std::vector<std::string> {
    const std::string data(symbol.data());
    std::istringstream stream(data);
    std::string line;

    std::vector<std::string> result;

    while (std::getline(stream, line)) {
        result.push_back(line);
    }

    return result;
}

void game(WINDOW *parent, const CONFIGURATION &configuration) {
    wclear(parent);
    wrefresh(parent);
    werase(parent);

    const INTERFACE context = {
        .visual =
            {
                .y = 0,
                .x = 0,
                .height = getmaxy(stdscr),
                .width = getmaxx(stdscr),
            },
    };

    WINDOW *window = create(context.visual.y, context.visual.x, context.visual.height, context.visual.width);

    nodelay(window, true);
    keypad(window, true);

    const SPRITE frog = sprite(LFROG);
    const TILE player = tile(frog, context.visual.height - frog.height, context.visual.width / 2);

    const std::vector<std::string> symbol = symbolize(player.sprite.symbol);

    while (configuration.status.running) {
        if (const std::string interface = keypad(window, context, player); interface == "q") {
            break;
        }

        wclear(window);

        for (size_t i = 0; i < symbol.size(); ++i) {
            mvwprintw(window, player.board.y + i, player.board.x, "%s", symbol[i].c_str());
        }

        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    delwin(window);
}
