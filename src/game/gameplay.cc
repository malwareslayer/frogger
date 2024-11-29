#include <mutex>
#include <random>
#include <unordered_map>

#include "../../src/game/gameplay.hpp"
#include "../../src/game/utility.hpp"
#include "../../src/game/animation.hpp"

#include "../../src/game/symbols/car.hpp"
#include "../../src/game/symbols/lily.hpp"
#include "../../src/game/symbols/log.hpp"

void play(std::shared_mutex &mutex, std::shared_ptr<NODE> &root, WINDOW* &window, const INTERFACE &context, const CONFIGURATION &configuration) {
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
    for (size_t i = 1; i <= 5 * root->sprite.height; i++) {
        last = create(root, last, tile(WATER, last->tile.board.y - 1, 0), {});
    }

    last = create(root, last, tile(SEPARATOR, last->tile.board.y, 0), {});

    // Generate Lily
    const SPRITE x = sprite(LILY_1);
    const SPRITE y = sprite(LILY_2);
    const SPRITE z = sprite(LILY_3);

    int ax = 0;

    for (size_t i = 1; i <= 4; i++) {
        if (i == 1) {
            if (context.visual.width % 10 == 0) {
                ax = last->tile.board.x + x.width * 1;
            } else {
                ax = last->tile.board.x + x.width * 2;
            }
        } else if (i == 3) {
            ax = last->tile.board.x + x.width * 4;
        }else {
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
                    lily->tile = tile(LILY, separator->tile.board.y - root->sprite.height - 1, ax);
                } else {
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

        for (size_t j = 1; j <= distribution_log(generate); j++) {
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

    std::vector<int> car_speed = {1000, 950, 900, 850, 800};
    std::uniform_int_distribution<int> distribution_car_defaults(0, car_speed.size() - 1);

    std::unordered_map<int, int> car_default_speed;

    car_default_speed[48] = car_speed[distribution_car_defaults(seed)];
    car_default_speed[43] = car_speed[distribution_car_defaults(seed)];
    car_default_speed[38] = car_speed[distribution_car_defaults(seed)];
    car_default_speed[33] = car_speed[distribution_car_defaults(seed)];

    // Activate Object Gameplay
    for (std::shared_ptr<NODE> current = root->next; current != nullptr; current = current->next) {
        if (current->active == false && (current->tile.type != SEPARATOR || current->tile.type != PLAYER || current->tile.type != WATER)) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);

                current->active = true;

                if (current->tile.type == LEFT_CAR) {
                    current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), 125);
                }

                if (current->tile.type == RIGHT_CAR) {
                    current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), 125);
                }

                if (current->tile.type == LILY) {
                    current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), 500);
                }

                if (current->tile.type == LEFT_LOG || current->tile.type == RIGHT_LOG) {
                    current->worker = std::thread(animate, std::ref(mutex), current, root, std::ref(context), std::ref(configuration), 1250);
                }
            }
        }
    }

    // Player Gameplay Interface
    while (configuration.status.play) {
        if (const std::string interface = keypad(window, context, root->sprite, root->tile); interface == "q") {
            break;
        }

        for (std::shared_ptr<NODE> current = root->next; current != nullptr; current = current->next) {
            switch (current->tile.type) {
                case RIGHT_LOG:
                    {
                        std::shared_lock<std::shared_mutex> lock(mutex);
                        if (current->tile.board.y == root->tile.board.y) {
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

    for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
        if (current->active == true && (current->tile.type == RIGHT_CAR || current->tile.type == LEFT_CAR || current->tile.type == LILY || current->tile.type == RIGHT_LOG || current->tile.type == LEFT_LOG)) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                current->active = false;
            }

            current->worker.join();
        }
    }

    // Stop Interface And Make Thread Done
    configuration.status.play = false;
}
