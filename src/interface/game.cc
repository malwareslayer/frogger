#include "game.hpp"

#include "../game/board.hpp"
#include "../game/interface.hpp"
#include "../game/symbols/frog.hpp"
#include "../game/tile.hpp"

#include <iostream>
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

void render(const INTERFACE &context, const CONFIGURATION &configuration, const TILE &player) {}

void game(WINDOW *parent, const CONFIGURATION &configuration) {
    wclear(parent);
    wrefresh(parent);

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

    const TILE player = tile(sprite(LFROG), 15, 15);

    // std::thread runner(render, context, configuration, player);

    while (configuration.status.running) {
        if (const std::string interface = key(window); interface == "q") {
            break;
        }

        const std::vector<std::string> symbol = symbolize(player.sprite.symbol);

        wclear(window);

        for (size_t i = 0; i < symbol.size(); ++i) {
            mvwprintw(window, (context.visual.height - player.sprite.height) / 2 + i, player.sprite.width, "%s",
                      symbol[i].c_str());
        }

        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    // runner.join();

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    delwin(window);
}
