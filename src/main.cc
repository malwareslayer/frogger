#include "../src/interface/menu.hpp"

#include "ncurses.h"

auto main() -> int {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    constexpr auto configuration = CONFIGURATION {
        .status = {
            .running = true,
            .play = false,
            .game_over = false,
        },
        .environment = {
            .car = 1,
            .log = 3,
            .lives = 3
        },
    };

    menu(configuration);

    endwin();
    clear();
    echo();
    nocbreak();
    curs_set(1);

    return 0;
}
