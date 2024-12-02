#include <memory>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>

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
    int height = getmaxy(stdscr);

    if (height % 5 != 0) {
        height = height - (height % 5);
    }

#if RELEASE
    if (height < 55) {
        exit(-1);
    }
#endif

    int width = getmaxx(stdscr);

    if (width % 10 != 0) {
        width = width - (width % 10);
    }

    const INTERFACE context = {
        .visual = {
            .y = 0,
            .x = 0,
            .height = height,
            .width = width,
        },
        .interface = {
            .choose = 0
        }
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
    configuration.status.game_win = false;
    configuration.status.now = std::chrono::system_clock::now();

    const SPRITE frog = sprite(LFROG);
    const TILE player = tile(PLAYER, context.visual.height - frog.height - 1, context.visual.width / 2 - 5);

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
                            for (int index = 0; index < context.visual.width; index++) {
                                mvwprintw(window, current->tile.board.y, index, "%c", '=');
                            }
                            break;
                        case RIGHT_CAR:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 1, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 5, "%s", std::to_string(current->tile.board.x).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 9, "%s", std::to_string(current->tile.speed).c_str());
#endif
                            break;
                        case LEFT_CAR:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 1, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 5, "%s", std::to_string(current->tile.board.x).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 9, "%s", std::to_string(current->tile.speed).c_str());
#endif
                            break;
#if RELEASE
                        case WATER:
                            for (int index = 0; index < context.visual.width; index++) {
                                mvwprintw(window, current->tile.board.y, index, "%c", '~');
                            }
                            break;
#endif
                        case LILY:
                            if (!current->pause) {
                                switch (current->sprite.track) {
                                    case 1:
                                        for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                            mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                                      current->sprite.symbol[index].c_str());
                                        }

                                        break;
                                    case 2:
                                        for (size_t index = 0; index < current->sprite.next->symbol.size(); index++) {
                                            mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                                      current->sprite.next->symbol[index].c_str());
                                        }

                                        break;
                                    case 3:
                                        for (size_t index = 0; index < current->sprite.next->next->symbol.size(); index++) {
                                            mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                                      current->sprite.next->next->symbol[index].c_str());
                                        }

                                        break;
                                    default:
                                        break;
                                }
                            }

                            break;
                        case RIGHT_LOG:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y, current->tile.board.x, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 4, "%s", std::to_string(current->tile.board.x).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 8, "%s", std::to_string(current->tile.speed).c_str());
#endif
                            break;
                        case LEFT_LOG:
                            for (size_t index = 0; index < current->sprite.symbol.size(); index++) {
                                mvwprintw(window, current->tile.board.y + index, current->tile.board.x, "%s",
                                          current->sprite.symbol[index].c_str());
                            }
#if DEBUG
                            mvwprintw(window, current->tile.board.y, current->tile.board.x, "%s", std::to_string(current->tile.board.y).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 4, "%s", std::to_string(current->tile.board.x).c_str());
                            mvwprintw(window, current->tile.board.y, current->tile.board.x + 8, "%s", std::to_string(current->tile.speed).c_str());
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
            mvwprintw(window, context.visual.height - 1, 20, "%s", "BAWAH = KEY DOWN");
            mvwprintw(window, context.visual.height - 1, 40, "%s", "KANAN = KEY RIGHT");
            mvwprintw(window, context.visual.height - 1, 60, "%s", "KIRI = KEY LEFT");

#if DEBUG
            const int right = total(root, RIGHT_CAR);

            mvwprintw(window, context.visual.height - 1, 80, "%s", "RIGHT CAR = ");
            mvwprintw(window, context.visual.height - 1, 92, "%s", std::to_string(right).c_str());

            const int left = total(root, LEFT_CAR);

            mvwprintw(window, context.visual.height - 1, 95, "%s", "LEFT CAR = ");
            mvwprintw(window, context.visual.height - 1, 106, "%s", std::to_string(left).c_str());

            mvwprintw(window, context.visual.height - 1, 115, "%s", "Y = ");
            mvwprintw(window, context.visual.height - 1, 119, "%s", std::to_string(root->tile.board.y).c_str());

            mvwprintw(window, context.visual.height - 1, 125, "%s", "X = ");
            mvwprintw(window, context.visual.height - 1, 129, "%s", std::to_string(root->tile.board.x).c_str());

            mvwprintw(window, context.visual.height - 1, 186, "%s", "HEIGHT = ");
            mvwprintw(window, context.visual.height - 1, 195, "%s", std::to_string(context.visual.height).c_str());

            mvwprintw(window, context.visual.height - 1, 199, "%s", "WIDTH = ");
            mvwprintw(window, context.visual.height - 1, 207, "%s", std::to_string(context.visual.width).c_str());
#endif
        }

        wnoutrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        doupdate();

        wrefresh(window);
    }

    if (configuration.status.game_over) {
        root->sprite = sprite(DFROG);

        for (size_t index = 0; index < root->sprite.symbol.size(); index++) {
            mvwprintw(window, root->tile.board.y + index, root->tile.board.x, "%s",
                      root->sprite.symbol[index].c_str());
        }

        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        wclear(window);
        box(window, 0, 0);
        wrefresh(window);
        mvwprintw(window, (getmaxy(stdscr) - 4) / 2, getmaxx(stdscr) / 2, "%s", "GAME OVER");
        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    if (configuration.status.game_win) {
        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        wclear(window);
        box(window, 0, 0);
        wrefresh(window);
        mvwprintw(window, (getmaxy(stdscr) - 4) / 2, getmaxx(stdscr) / 2, "%s", "YOU WIN!");
        wrefresh(window);
        mvwprintw(window, (getmaxy(stdscr) - 3) / 2, (getmaxx(stdscr) / 2) - 5, "%s", "SCORE: ");
        wrefresh(window);
        mvwprintw(window, (getmaxy(stdscr) - 3) / 2, (getmaxx(stdscr) / 2) + 10, "%s", std::to_string(std::chrono::duration_cast<std::chrono::seconds>(configuration.status.last - configuration.status.now).count()).c_str());
        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
        if (current->active == true && (current->tile.type == RIGHT_CAR || current->tile.type == LEFT_CAR || current->tile.type == LILY || current->tile.type == RIGHT_LOG || current->tile.type == LEFT_LOG)) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                current->active = false;
            }

            current->worker.join();
        }
    }

    playing.join();

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    werase(window);
    delwin(window);
}
