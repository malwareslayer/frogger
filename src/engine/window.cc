#include "../../src/engine/window.hpp"

auto create(const int &y, const int &x, const int &height, const int &width) -> WINDOW* {
    return newwin(height, width, y, x);
}

void boxes(WINDOW* &window, const int &highlight, const std::vector<std::string> &choices) {
    int y = 3;

    box(window, 0, 0);

    for (size_t i = 0; i < choices.size(); ++i) {
        const int x = (getmaxx(window) - static_cast<int>(choices[i].length())) / 2;

        if (highlight == static_cast<int>(i) + 1) {
            wattron(window, A_REVERSE);
            mvwprintw(window, y, x, "%s", choices[i].c_str());
            wattroff(window, A_REVERSE);
        } else {
            mvwprintw(window, y, x, "%s", choices[i].c_str());
        }

        ++y;
    }

    wrefresh(window);
}

void bar(WINDOW* &window, const int &data, const int &y, const int &x) {
    wmove(window, y, x);
    whline(window, ' ', y);
    mvwprintw(window, y, x, "[%-*s] %d/5", 5, std::string(data, '|').c_str(), data);
    wrefresh(window);
}
