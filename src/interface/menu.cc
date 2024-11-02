#include "menu.hpp"
#include "setting.hpp"
#include "credit.hpp"

#include "../property.hpp"

const std::vector<std::string> labeling = {"Play", "Setting", "Credit", "Exit"};

void menu(const CONFIGURATION &configuration) {
    constexpr INTERFACE context = {
        .visual = {
            .y = 10,
            .x = 30,
        },
        .interface = {
            .choose = 1,
        }
    };

    getmaxyx(stdscr, context.visual.height, context.visual.width);

    WINDOW *window = create(context.visual.y, context.visual.x,
        (context.visual.height - context.visual.y) / 2, (context.visual.width - context.visual.x) / 2);

    nodelay(window, true);
    keypad(window, true);

    boxes(window, context.interface.choose, labeling);

    while (configuration.status.running) {
        if (const std::string choosing = choose(window, context.interface, labeling); choosing == "Play") {
            wclear(window);

            game(window, configuration);
        } else if (choosing == "Setting") {
            wclear(window);

            setting(window, configuration);
        } else if (choosing == "Credit") {
            wclear(window);

            credit(window, context.visual.height, context.visual.width);
        } else if (choosing == "Exit") {
            configuration.status.running = false;
        }

        boxes(window, context.interface.choose, labeling);
    }

    clrtoeol();
    refresh();
    endwin();
}
