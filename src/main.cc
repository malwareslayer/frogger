#include "../src/interface/menu.hpp"
#include "ncurses.h"

auto main(int argc, char* argv[]) -> int {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    menu({.status = {.running = true}, .environment = {.car = 1, .log = 1}});

    return 0;
}
