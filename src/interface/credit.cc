/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#include "../../src/interface/credit.hpp"

#include "../../src/game/interface.hpp"
#include "../../src/engine/window.hpp"

#include <string>
#include <thread>
#include <vector>

const std::vector<std::string> labeling = {
    "Your adventure is our passion. Thank you for being a part of it",
    "",
    "Developed by:",
    "Team I DPKK - Faculty Of Computer Science Of Pelita Harapan University",
    "",
    "Advisor",
    "Robertus Hudi, S.Inf., M.Kom",
    "",
    "Team Leader:",
    "Dwi Mulia Mokoginta",
    "",
    "Software Developers:",
    "Ramot Hotland Haposan ",
    "Sony Surahman",
    "Sapwages Triziana ",
    "Sulaiman Rasyid Dinitra Aziz",
    "",
    "Immeasurable thanks to Mr. Robertus Hudi, S.Inf., M.Kom for making everything possible"};

void credit(WINDOW* &parent) {
    const INTERFACE context = {
        .visual = {
            .y = 0,
            .x = 0,
            .height = getmaxy(stdscr),
            .width = getmaxx(stdscr),
        },
        .interface = {
            .choose = 0,
        }
    };

    wclear(parent);
    wrefresh(parent);
    werase(parent);

    WINDOW *window = create(context.visual.y, context.visual.x, context.visual.height, context.visual.width);

    int start = context.visual.height;

    while (start + labeling.size() > 0) {
        wclear(window);

        for (size_t i = 0; i < labeling.size(); ++i) {
            if (const int y = start + static_cast<int>(i); y >= 0 && y < context.visual.height) {
                mvwprintw(window, y, (context.visual.width - static_cast<int>(labeling[i].length())) / 2, "%s", labeling[i].c_str());
            }
        }

        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(800));

        start--;
    }

    wclear(window);
    wrefresh(window);
    werase(window);
    delwin(window);
}
