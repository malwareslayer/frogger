#include "game.hpp"

#include <mutex>

#include "../../src/game/interface.hpp"
#include "../../src/game/sprite.hpp"
#include "../../src/game/tile.hpp"

#include "../../src/game/symbols/car.hpp"
#include "../../src/game/symbols/frog.hpp"

#include <random>
#include <thread>

std::mutex mutex;

struct NODE {
    mutable int index;
    mutable bool active;
    mutable std::thread worker;
    mutable SPRITE sprite;
    mutable TILE tile;
    mutable NODE *previous = nullptr;
    mutable NODE *next = nullptr;
};

void animate(const NODE *root, const INTERFACE &context) {
    while (root->active) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            if (root->tile.board.x < context.visual.width + root->sprite.width) {
                root->tile.board.x = root->tile.board.x + root->sprite.width + 1;

                if (root->tile.board.x + root->sprite.width > context.visual.width) {
                    root->tile.board.x = 0;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

void play(NODE *&root, WINDOW *&window, const INTERFACE &context, const CONFIGURATION &configuration) {
    std::random_device seed;
    std::mt19937 generate(seed());
    std::uniform_int_distribution<int> distribution(1, configuration.environment.car);

    {
        std::lock_guard<std::mutex> lock(mutex);
        root->next = new NODE{
            .index = root->index + 1,
            .active = true,
            .tile = tile(TSEPARATOR, context.visual.height - 1 - root->sprite.height - 1, 0),
            .previous = root,
        };
    }

    const SPRITE rcar = sprite(RCAR);

    /*
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < distribution(generate); ++j) {
            for (const NODE* current = root; current != nullptr; current = current->next) {
                if (!current->next) {

                }
            }
        }
    }

    {

        std::lock_guard<std::mutex> lock(mutex);

        root->next->next = new NODE {
            .index = root->next->index + 1,
            .active = false,
            .sprite = rcar,
            .tile = tile(TCAR, root->next->tile.board.y - rcar.height, 0),
            .previous = root->next,
        };
    }

    for (const NODE* current = root->next; current != nullptr; current = current->next) {
        switch (current->tile.type) {
            case TCAR:
                if (!current->active) {
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        current->active = true;
                        current->worker = std::thread(animate, current, std::ref(context));
                    }
                }
                break;
            default:
                break;
        }
    }
    */

    while (configuration.status.play) {
        if (const std::string interface = keypad(window, context, root->sprite, root->tile); interface == "q") {
            break;
        }

        /*
        for (const NODE* current = root->next; current != nullptr; current = current->next) {}
        */

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    configuration.status.play = false;
}

void game(WINDOW *parent, const CONFIGURATION &configuration) {
    const INTERFACE context = {
        .visual =
            {
                .y = 0,
                .x = 0,
                .height = getmaxy(stdscr),
                .width = getmaxx(stdscr),
            },
    };

    wclear(parent);
    wrefresh(parent);
    werase(parent);

    WINDOW *window = create(context.visual.y, context.visual.x, context.visual.height, context.visual.width);

    nodelay(window, true);
    keypad(window, true);

    const SPRITE frog = sprite(LFROG);
    const TILE player = tile(TFROG, context.visual.height - frog.height - 1, context.visual.width / 2);

    auto root = new NODE{.index = 1, .active = true, .sprite = frog, .tile = player};

    configuration.status.play = true;

    std::thread thread(play, std::ref(root), std::ref(window), std::ref(context), std::ref(configuration));

    while (configuration.status.running) {
        if (configuration.status.play == false) {
            break;
        }

        wclear(window);

        for (const NODE *current = root; current != nullptr; current = current->next) {
            if (current->active == true) {
                switch (current->tile.type) {
                    case TFROG:
                        for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                            mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                      current->sprite.symbol[index].c_str());
                        }
                        break;
                    case TSEPARATOR:
                        for (size_t index = 0; index < context.visual.width; index++) {
                            mvwprintw(window, current->tile.board.y, index, "%c", '=');
                        }
                        break;
                    case TCAR:
                        for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                            mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                      current->sprite.symbol[index].c_str());
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        mvwprintw(window, context.visual.height - 1, 0, "%s", "ATAS = KEY UP");
        mvwprintw(window, context.visual.height - 1, 25, "%s", "BAWAH = KEY DOWN");
        mvwprintw(window, context.visual.height - 1, 50, "%s", "KANAN = KEY RIGHT");
        mvwprintw(window, context.visual.height - 1, 75, "%s", "KIRI = KEY LEFT");

        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    for (const NODE *current = root; current != nullptr; current = current->next) {
        switch (current->tile.type) {
            case TCAR:
                if (current->active) {
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        current->active = false;
                    }

                    current->worker.join();
                }
                break;
            default:
                break;
        }
    }

    thread.join();

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    werase(window);
    delwin(window);

    delete root;
}
