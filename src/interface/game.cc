#include <memory>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include "../../src/interface/game.hpp"

#include "../../src/game/interface.hpp"
#include "../../src/game/sprite.hpp"
#include "../../src/game/tile.hpp"
#include "../../src/game/node.hpp"
#include "../../src/game/gameplay.hpp"
#include "../../src/game/utility.hpp"

#include "../../src/game/symbols/frog.hpp"

std::shared_mutex mutex;

void game(WINDOW* &parent, const CONFIGURATION &configuration) {
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

    auto window = create(context.visual.y, context.visual.x, context.visual.height, context.visual.width);

    nodelay(window, true);
    keypad(window, true);
    clearok(window, true);

    configuration.status.play = true;
    configuration.status.game_over = false;

    const SPRITE frog = sprite(LFROG);
    const TILE player = tile(PLAYER, context.visual.height - frog.height - 1, context.visual.width / 2 + 1);

    auto root = std::make_shared<NODE>(NODE {
        .index = 1,
        .active = true,
        .sprite = frog,
        .tile = player
    });

    std::thread playing(play, std::ref(mutex), std::ref(root), std::ref(window), std::ref(context), std::ref(configuration));

    while (configuration.status.running) {
        wclear(window);

        wbkgd(window, ' ' | A_NORMAL);

        werase(window);

        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            if (configuration.status.play == false) {
                break;
            }

            for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
                if (current->active == true) {
                    switch (current->tile.type) {
                        case SEPARATOR:
                            for (size_t index = 0; index < context.visual.width; index++) {
                                mvwprintw(window, current->tile.board.y, index, "%c", '=');
                            }
                            break;
                        case RIGHT_CAR:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 3, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 7, "%s", std::to_string(current->tile.board.x).c_str());
#endif
                            break;
                        case LEFT_CAR:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 3, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 7, "%s", std::to_string(current->tile.board.x).c_str());
#endif
                            break;
#if RELEASE
                        case WATER:
                            for (size_t index = 0; index < context.visual.width; index++) {
                                mvwprintw(window, current->tile.board.y, index, "%c", '~');
                            }
                            break;
#endif
                        case LILY:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
                            break;
                        case RIGHT_LOG:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y - 1, current->tile.board.x + 3, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y - 1, current->tile.board.x + 7, "%s", std::to_string(current->tile.board.x).c_str());
#endif
                            break;
                        case LEFT_LOG:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y - 1, current->tile.board.x + 3, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y - 1, current->tile.board.x + 7, "%s", std::to_string(current->tile.board.x).c_str());
#endif

                            break;
                        default:
                            break;
                    }
                }
            }

            for (size_t index = 0; index < root->sprite.symbol.size(); index++) {
                mvwprintw(window, root->tile.board.y + index, root->tile.board.x, "%s",
                          root->sprite.symbol[index].c_str());
            }

            mvwprintw(window, context.visual.height - 1, 0, "%s", "ATAS = KEY UP");
            mvwprintw(window, context.visual.height - 1, 15, "%s", "BAWAH = KEY DOWN");
            mvwprintw(window, context.visual.height - 1, 35, "%s", "KANAN = KEY RIGHT");
            mvwprintw(window, context.visual.height - 1, 55, "%s", "KIRI = KEY LEFT");

#if DEBUG
            const int right = total(root, RIGHT_CAR);

            mvwprintw(window, context.visual.height - 1, 100, "%s", "RIGHT CAR = ");
            mvwprintw(window, context.visual.height - 1, 115, "%s", std::to_string(right).c_str());

            const int left = total(root, LEFT_CAR);

            mvwprintw(window, context.visual.height - 1, 125, "%s", "LEFT CAR = ");
            mvwprintw(window, context.visual.height - 1, 145, "%s", std::to_string(left).c_str());

            mvwprintw(window, context.visual.height - 1, 155, "%s", "Y = ");
            mvwprintw(window, context.visual.height - 1, 160, "%s", std::to_string(root->tile.board.y).c_str());

            mvwprintw(window, context.visual.height - 1, 165, "%s", "X = ");
            mvwprintw(window, context.visual.height - 1, 170, "%s", std::to_string(root->tile.board.x).c_str());
#endif
        }

        wnoutrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        doupdate();

        wrefresh(window);
    }

    if (configuration.status.game_over) {
        wclear(window);
        box(window, 0, 0);
        wrefresh(window);
        mvwprintw(window, (getmaxy(stdscr) - 4) / 2, getmaxx(stdscr) / 2, "%s", "GAME OVER");
        wrefresh(window);
    }

    playing.join();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    werase(window);
    delwin(window);
}
