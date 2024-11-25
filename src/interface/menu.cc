/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#include "../../src/utility/property.hpp"

#include "../../src/interface/menu.hpp"
#include "../../src/interface/credit.hpp"
#include "../../src/interface/game.hpp"
#include "../../src/interface/setting.hpp"

#include <thread>

const std::vector<std::string> labeling = {"Bermain", "Pengaturan", "Kredit", "Keluar"};

void menu(const CONFIGURATION &configuration) {
    const INTERFACE context = {
        .visual = {
            .y = (getmaxy(stdscr) - 10) / 2,
            .x = (getmaxx(stdscr) - 30) / 2,
            .height = 10,
            .width = 30,
        },
        .interface = {
            .choose = 1
        }
    };

    WINDOW *window = create(context.visual.y, context.visual.x, context.visual.height, context.visual.width);

    nodelay(window, true);
    keypad(window, true);

    boxes(window, context.interface.choose, labeling);

    while (configuration.status.running) {
        if (const std::string choosing = choose(window, context.interface, labeling); choosing == "Bermain") {
            wclear(window);

            game(window, configuration);
        } else if (choosing == "Pengaturan") {
            wclear(window);

            setting(window, configuration);
        } else if (choosing == "Kredit") {
            wclear(window);

            credit(window);
        } else if (choosing == "Keluar") {
            configuration.status.running = false;
        } else {
            wrefresh(window);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        boxes(window, context.interface.choose, labeling);
    }

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    werase(window);
    delwin(window);
}
