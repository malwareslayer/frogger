#include <algorithm>
#include <mutex>
#include <random>
#include <ranges>

#include "../../src/game/gameplay.hpp"

#include "../../src/game/utility.hpp"
#include "../../src/game/animation.hpp"

#include "../../src/game/symbols/car.hpp"
#include "../../src/game/symbols/lily.hpp"
#include "../../src/game/symbols/log.hpp"

void play(std::shared_mutex &mutex, std::shared_ptr<NODE> &root, WINDOW* &window, const INTERFACE &context, const CONFIGURATION &configuration) {
    configuration.environment.lives = 3;

    std::random_device seed;
    std::mt19937 generate(seed());
    std::uniform_int_distribution<int> distribution_car(1, configuration.environment.car);
    std::uniform_int_distribution<int> distribution_log(1, configuration.environment.log);

    // First Separator
    auto last = create(root, root, tile(SEPARATOR, context.visual.height - 1 - root->sprite.height - 1, 0), {}, true);

    int initial = total(root);

    // Generate Car
    for (size_t i = 1; i <= 4; i++) {
        for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
            const int now = total(root);

            if (current->index < now) {
                continue;
            }

            if (initial != now) {
                initial = now;
                break;
            }

            if (!current->next) {
                if (i % 2 != 0) {
                    const SPRITE rcar = sprite(RCAR);

                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        for (int j = now + 1; j <= now + distribution_car(generate); j++) {
                            switch (last->tile.type) {
                                case SEPARATOR:
                                    last = create(last, last, tile(RIGHT_CAR, last->tile.board.y - rcar.height, 0), rcar, false);
                                    break;
                                case RIGHT_CAR:
                                    last = create(last, last, tile(RIGHT_CAR, last->tile.board.y, last->tile.board.x + rcar.width), rcar, false);
                                    break;
                                case LEFT_CAR:
                                    last = create(last, last, tile(RIGHT_CAR, last->tile.board.y - rcar.height - 1, 0), rcar, false);
                                    break;
                                default:
                                    exit(1);
                            }
                        }
                    }
                } else {
                    const SPRITE lcar = sprite(LCAR);

                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        for (int j = now + 1; j <= now + distribution_car(generate); j++) {
                            switch (last->tile.type) {
                                case SEPARATOR:
                                    last = create(last, last, tile(LEFT_CAR, last->tile.board.y - lcar.height, context.visual.width - lcar.width), lcar, false);
                                    break;
                                case RIGHT_CAR:
                                    last = create(last, last, tile(LEFT_CAR, last->tile.board.y - lcar.height - 1, context.visual.width), lcar, false);
                                    break;
                                case LEFT_CAR:
                                    last = create(last, last, tile(LEFT_CAR, last->tile.board.y, last->tile.board.x - lcar.width), lcar, false);
                                    break;
                                default:
                                    exit(1);
                            }
                        }
                    }
                }
            }
        }
    }

    last = create(root, last, tile(SEPARATOR, last->tile.board.y - 1, 0), {});
    last = create(root, last, tile(SEPARATOR, last->tile.board.y - root->sprite.height - 1, 0), {});

    // Save Separator For Fixed Lily Later
    auto separator = last;

    // Generate Water
    for (int i = 1; i <= 5 * root->sprite.height; i++) {
        last = create(root, last, tile(WATER, last->tile.board.y - 1, 0), {});
    }

    std::vector<int> prefix;

    // Generate Lily
    const SPRITE x = sprite(LILY_1);
    const SPRITE y = sprite(LILY_2);
    const SPRITE z = sprite(LILY_3);

    int ax = 0;

    for (int i = 1; i <= 4; i++) {
        if (i == 1) {
            if (context.visual.width % 10 == 0) {
                ax = last->tile.board.x + x.width * 1;
            } else {
                ax = last->tile.board.x + x.width * 2;
            }
        } else if (i == 3) {
            ax = last->tile.board.x + x.width * 4;
        } else {
            ax = last->tile.board.x + x.width * 3;
        }

        for (size_t j = 1; j <= 3; j++) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                auto lily = std::make_shared<NODE>(NODE {
                    .active = false,
                    .sprite = {
                        .symbol = x.symbol,
                        .height = x.height,
                        .width = x.width,
                        .next = new SPRITE {
                            .symbol = y.symbol,
                            .height = y.height,
                            .width = y.width,
                            .next = new SPRITE {
                                .symbol = z.symbol,
                                .height = z.height,
                                .width = z.width,
                            }
                        }
                    },
                });

                if (j == 1) {
                    prefix.insert(prefix.end(), ax);

                    lily->tile = tile(LILY, separator->tile.board.y - root->sprite.height - 1, ax);
                } else {
                    prefix.insert(prefix.end(), last->tile.board.x + x.width);

                    lily->tile = tile(LILY, separator->tile.board.y - root->sprite.height - 1, last->tile.board.x + x.width);
                }

                last = create(last, last, lily);
            }
        }
    }

    const SPRITE log = sprite(LOG);

    int ay = last->tile.board.y;

    for (size_t i = 1; i <= 3; i++) {
        ay = (ay - log.height);

        int cx = 0;
        int dx = context.visual.width + log.width;

        for (int j = 1; j <= distribution_log(generate); j++) {
            if (i % 2 != 0) {
                last = create(last, last, tile(RIGHT_LOG, ay, cx), log, false);

                cx = cx + log.width;
            } else {
                dx = dx - log.width;

                last = create(last, last, tile(LEFT_LOG, ay, dx), log, false);
            }
        }

        ay = ay - 1;
    }

    last = create(root, last, tile(SEPARATOR, last->tile.board.y - 1, 0), {});

    std::vector<int> car_speeds = { 200, 175, 150, 125, 100 };
    std::uniform_int_distribution<int> distribution_car_speed(0, car_speeds.size() - 1);

    const int first_line_car = car_speeds[distribution_car_speed(generate)];
    const int second_line_car = car_speeds[distribution_car_speed(generate)];
    const int third_line_car = car_speeds[distribution_car_speed(generate)];
    const int fourth_line_car = car_speeds[distribution_car_speed(generate)];

    std::vector<int> log_speeds = { 700, 600, 500, 400, 300 };
    std::uniform_int_distribution<int> distribution_log_speed(0, log_speeds.size() - 1);

    const int first_line_log = log_speeds[distribution_log_speed(generate)];
    const int second_line_log = log_speeds[distribution_log_speed(generate)];
    const int third_line_log = log_speeds[distribution_log_speed(generate)];

    std::vector<int> lily_cycles = { 4, 3, 2, 1 };
    std::uniform_int_distribution<int> distribution_lily_cycle(0, lily_cycles.size() - 1);

    const int first_group_lily = lily_cycles[distribution_lily_cycle(generate)];
    const int second_group_lily = lily_cycles[distribution_lily_cycle(generate)];
    const int third_group_lily = lily_cycles[distribution_lily_cycle(generate)];
    const int fourth_group_lily = lily_cycles[distribution_lily_cycle(generate)];

    int group = 1;
    int counter = 0;

    // Activate Object Gameplay
    for (std::shared_ptr<NODE> current = root->next; current != nullptr; current = current->next) {
        if (current->active == false && (current->tile.type != SEPARATOR || current->tile.type != PLAYER || current->tile.type != WATER)) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);

                current->active = true;

                if (current->tile.type == LEFT_CAR || current->tile.type == RIGHT_CAR) {
                    switch (current->tile.board.y) {
                        case 45:
                            current->tile.speed = first_line_car;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), first_line_car);
                            break;
                        case 40:
                            current->tile.speed = second_line_car;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), second_line_car);
                            break;
                        case 35:
                            current->tile.speed = third_line_car;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), third_line_car);
                            break;
                        case 30:
                            current->tile.speed = fourth_line_car;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), fourth_line_car);
                            break;
                        default:
                            current->tile.speed = 125;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), 125);
                            break;
                    }
                }

                if (current->tile.type == LILY) {
                    counter++;

                    switch (group) {
                        case 1:
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), first_group_lily);
                            break;
                        case 2:
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), second_group_lily);
                            break;
                        case 3:
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), third_group_lily);
                            break;
                        case 4:
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), fourth_group_lily);
                            break;
                        default:
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), 3000);
                            break;
                    }

                    if (counter == 3) {
                        counter = 0;
                        group++;
                    }
                }

                if (current->tile.type == LEFT_LOG || current->tile.type == RIGHT_LOG) {
                    switch (current->tile.board.y) {
                        case 15:
                            current->tile.speed = first_line_log;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), first_line_log);
                            break;
                        case 10:
                            current->tile.speed = second_line_log;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), second_line_log);
                            break;
                        case 5:
                            current->tile.speed = third_line_log;
                            current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), third_line_log);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    // Player Gameplay Interface
    while (configuration.status.play) {
        if (configuration.status.game_over) {
            break;
        }

        if (const std::string interface = keypad(window, context, root->sprite, root->tile); interface == "q") {
            break;
        }

        for (std::shared_ptr<NODE> current = root->next; current != nullptr; current = current->next) {
            switch (current->tile.type) {
                case LILY:
                    {
                        std::shared_lock<std::shared_mutex> lock(mutex);
                        if (current->tile.board.y == root->tile.board.y - 1) {
                            if (std::ranges::find(prefix, root->tile.board.x) != prefix.end()) { // NOLINT
                                if (current->pause && current->sprite.track == 0) {
                                    if (configuration.environment.lives > 0) {
                                        configuration.environment.lives--;
                                        root->tile.board.y = context.visual.height - root->sprite.height - 1;
                                        root->tile.board.x = context.visual.width / 2 - 5;
                                    } else {
                                        configuration.status.game_over = true;
                                    }
                                }
                            } else {
                                if (configuration.environment.lives > 0) {
                                    configuration.environment.lives--;
                                    root->tile.board.y = context.visual.height - root->sprite.height - 1;
                                    root->tile.board.x = context.visual.width / 2 - 5;
                                } else {
                                    configuration.status.game_over = true;
                                }
                            }
                        }
                    }

                    break;
                case RIGHT_LOG:
                    {
                        std::shared_lock<std::shared_mutex> lock(mutex);
                        if (current->tile.board.y == root->tile.board.y) {
                            if (current->previous->tile.type != RIGHT_LOG) {
                                if (current->tile.board.x - current->sprite.width / 2 > root->tile.board.x) {
                                    if (configuration.environment.lives > 0) {
                                        configuration.environment.lives--;
                                        root->tile.board.y = context.visual.height - root->sprite.height - 1;
                                        root->tile.board.x = context.visual.width / 2 - 5;
                                    } else {
                                        configuration.status.game_over = true;
                                    }
                                }
                            }

                            if (current->next->tile.type != RIGHT_LOG) {
                                if (root->tile.board.x > current->tile.board.x + current->sprite.width / 2) {
                                    if (configuration.environment.lives > 0) {
                                        configuration.environment.lives--;
                                        root->tile.board.y = context.visual.height - root->sprite.height - 1;
                                        root->tile.board.x = context.visual.width / 2 - 5;
                                    } else {
                                        configuration.status.game_over = true;
                                    }
                                }
                            }

                            if (current->tile.board.x == root->tile.board.x) {
#if DEBUG
                                mvwprintw(window, 0, 0, "%s", std::string("TRIGGERED #1").c_str());
#endif
                                root->tile.board.x = current->tile.board.x;
                            }

                            if (current->tile.board.x + root->sprite.width == root->tile.board.x) {
#if DEBUG
                                mvwprintw(window, 0, 15, "%s", std::string("TRIGGERED #2").c_str());
#endif
                                root->tile.board.x = current->tile.board.x + root->sprite.width;
                            }
                        }
                    }

                    break;
                case LEFT_LOG:
                    {
                        std::shared_lock<std::shared_mutex> lock(mutex);
                        if (current->tile.board.y == root->tile.board.y) {
                            if (current->previous->tile.type != LEFT_LOG) {
                                if (current->tile.board.x - current->sprite.width / 2 > root->tile.board.x) {
                                    if (configuration.environment.lives > 0) {
                                        configuration.environment.lives--;
                                        root->tile.board.y = context.visual.height - root->sprite.height - 1;
                                        root->tile.board.x = context.visual.width / 2 - 5;
                                    } else {
                                        configuration.status.game_over = true;
                                    }
                                }
                            }

                            if (current->next->tile.type != LEFT_LOG) {
                                if (current->tile.board.x - current->sprite.width / 2 > root->tile.board.x) {
                                    if (configuration.environment.lives > 0) {
                                        configuration.environment.lives--;
                                        root->tile.board.y = context.visual.height - root->sprite.height - 1;
                                        root->tile.board.x = context.visual.width / 2 - 5;
                                    } else {
                                        configuration.status.game_over = true;
                                    }
                                }
                            }

                            if (current->tile.board.x == root->tile.board.x) {
#if DEBUG
                                mvwprintw(window, 0, 0, "%s", std::string("TRIGGERED #1").c_str());
#endif
                                root->tile.board.x = current->tile.board.x;
                            }

                            if (current->tile.board.x + root->sprite.width == root->tile.board.x) {
#if DEBUG
                                mvwprintw(window, 0, 15, "%s", std::string("TRIGGERED #2").c_str());
#endif
                                root->tile.board.x = current->tile.board.x + root->sprite.width;
                            }
                        }
                    }

                    break;
                default:
                    break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Stop Interface And Make Thread Done
    configuration.status.play = false;
}
