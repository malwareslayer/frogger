#include "ncurses.h"

#include "interface/menu.hpp"

int main(int argc, char *argv[]) {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    menu({
        .status = {
            .running = true,
        },
        .environment = {
            .car = 1,
            .log = 1
        }
    });

    return 0;
}