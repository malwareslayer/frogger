/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#include "../../src/interface/credit.hpp"

#include <string>
#include <thread>
#include <vector>

const std::vector<std::string> labeling = {"Thanks for Playing",
                                           "",
                                           "Member:",
                                           "Dwi Mulia Mokoginta - ",
                                           "Ramot Hotland Haposan - ",
                                           "Sony Surahman - ",
                                           "Sapwages Triziana - ",
                                           "Sulaiman Rasyid Dinitra Aziz - ",
                                           "",
                                           "Engine:",
                                           "Dwi Mulia Mokoginta",
                                           "",
                                           "Game:",
                                           "Dwi Mulia Mokoginta",
                                           "",
                                           "Interface:",
                                           "Ramot Hotland Haposan",
                                           "Sony Surahman",
                                           "",
                                           "Utility:",
                                           "Sulaiman Rasyid Dinitra Aziz",
                                           "",
                                           "Mainframe:",
                                           "Sapwages Triziana",
                                           ""};

void credit(WINDOW* window, const int& height, const int& width) {
    wclear(window);
    wrefresh(window);

    int start = height;

    while (start + labeling.size() > 0) {
        clear();

        for (size_t i = 0; i < labeling.size(); ++i) {
            if (const int y = start + static_cast<int>(i); y >= 0 && y < height) {
                mvprintw(y, (width - static_cast<int>(labeling[i].length())) / 2, "%s", labeling[i].c_str());
            }
        }

        refresh();

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        start--;
    }

    wclear(window);
}
