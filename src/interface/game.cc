#include "game.hpp"

#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>

#include "../../src/game/interface.hpp"
#include "../../src/game/sprite.hpp"
#include "../../src/game/symbols/car.hpp"
#include "../../src/game/symbols/frog.hpp"
#include "../../src/game/tile.hpp"

std::shared_mutex mutex;

struct NODE {
    mutable int index;
    mutable bool active;
    mutable std::thread worker;
    mutable SPRITE sprite;
    mutable TILE tile;
    mutable NODE *previous = nullptr;
    mutable NODE *next = nullptr;
};

auto total(NODE* &root) -> int {
    int index = 0;

    for (const NODE *current = root; current != nullptr; current = current->next) {
        index = current->index;

        if (!current->next) {
            break;
        }
    }

    return index;
}

auto create(NODE* &root, const TILE &tile, const SPRITE &sprite, const bool &active = true) -> NODE * {
    {
        std::unique_lock<std::shared_mutex> lock(mutex);
        if (!root->next) {
            root->next = new NODE {
                .index = root->index + 1,
                .active = active,
                .sprite = sprite,
                .tile = tile,
                .previous = root,
            };
        }
    }

    return root->next;
}

void animate(const NODE *root, const INTERFACE &context) {
    while (root->active) {
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            if (root->tile.board.x < context.visual.width + root->sprite.width) {
                root->tile.board.x = root->tile.board.x + root->sprite.width + 1;

                if (root->tile.board.x + root->sprite.width > context.visual.width) {
                    root->tile.board.x = 0;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }
}

void play(NODE *&root, WINDOW *&window, const INTERFACE &context, const CONFIGURATION &configuration) {
    configuration.status.play = true;

    std::random_device seed;
    std::mt19937 generate(seed());
    std::uniform_int_distribution<int> distribution(1, configuration.environment.car);

    create(root, tile(SEPARATOR, context.visual.height - 1 - root->sprite.height - 1, 0), {});

    const SPRITE car = sprite(RCAR);

    for (size_t j = total(root) + 1; j < total(root) + distribution(generate) + 1; j++) {
        for (NODE* current = root; current != nullptr; current = current->next) {
            if (current->index == j) {
                continue;
            }

            if (!current->next) {
                switch (current->tile.type) {
                    case SEPARATOR:
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            current->next = new NODE{
                                .index = current->index + 1,
                                .active = false,
                                .sprite = car,
                                .tile = tile(CAR, current->tile.board.y - car.height, 0),
                                .previous = current,
                            };
                        }
                        break;
                    case CAR:
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            current->next = new NODE{
                                .index = current->index + 1,
                                .active = false,
                                .sprite = car,
                                .tile = tile(CAR, current->tile.board.y, current->tile.board.x + car.width + 1),
                                .previous = current,
                            };
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    for (NODE *current = root->next; current != nullptr; current = current->next) {
        switch (current->tile.type) {
            case CAR:
                if (current->active == false) {
                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        current->active = true;
                        current->worker = std::thread(animate, current, std::ref(context));
                    }
                }
                break;
            default:
                break;
        }
    }

    while (configuration.status.play) {
        if (const std::string interface = keypad(window, context, root->sprite, root->tile); interface == "q") {
            break;
        }

        /*
        for (const NODE* current = root->next; current != nullptr; current = current->next) {}
        */

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    for (const NODE *current = root; current != nullptr; current = current->next) {
        switch (current->tile.type) {
            case CAR:
                if (current->active == true) {
                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        current->active = false;
                    }

                    current->worker.join();
                }
                break;
            default:
                break;
        }
    }

    configuration.status.play = false;
}

void game(WINDOW *&parent, const CONFIGURATION &configuration) {
    const INTERFACE context = {
        .visual = {
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
    const TILE player = tile(PLAYER, context.visual.height - frog.height - 1, context.visual.width / 2);

    auto root = new NODE {
        .index = 1,
        .active = true,
        .sprite = frog,
        .tile = player
    };

    std::thread thread(play, std::ref(root), std::ref(window), std::ref(context), std::ref(configuration));

    while (configuration.status.running) {
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            wclear(window);

            if (configuration.status.play == false) {
                break;
            }

            for (const NODE *current = root; current != nullptr; current = current->next) {
                if (current->active == true) {
                    switch (current->tile.type) {
                        case PLAYER:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
                            break;
                        case SEPARATOR:
                            for (size_t index = 0; index < context.visual.width; index++) {
                                mvwprintw(window, current->tile.board.y, index, "%c", '=');
                            }
                            break;
                        case CAR:
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
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        werase(window);
    }

    thread.join();

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    werase(window);
    delwin(window);

    delete root;
}
