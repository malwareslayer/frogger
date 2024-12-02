#include <mutex>

#include "../../src/game/animation.hpp"

void animate(std::shared_mutex &mutex, const std::shared_ptr<NODE> &root, const std::shared_ptr<NODE> &player, const INTERFACE &context, const CONFIGURATION &configuration, const int &cycle) {
    while (root->active) {
        switch (root->tile.type) {
            case RIGHT_CAR:
                if (root->tile.board.x <= context.visual.width) {
                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        root->tile.board.x = root->tile.board.x + root->sprite.width;
                    }

                    if (root->tile.board.y == player->tile.board.y &&
                        root->tile.board.x <= player->tile.board.x &&
                        root->tile.board.x + root->sprite.width >= player->tile.board.x) {

                        if (configuration.environment.lives > 0) {
                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                configuration.environment.lives--;
                            }

                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                player->tile.board.y = context.visual.height - player->sprite.height - 1;
                                player->tile.board.x = context.visual.width / 2 - 5;
                            }
                        } else {
                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                configuration.status.game_over = true;
                            }
                        }
                    }

                    if (root->tile.board.x >= context.visual.width) {
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            root->tile.board.x = 0;
                        }
                    }
                }

                break;
            case LEFT_CAR:
                if (root->tile.board.x >= 0) {
                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        root->tile.board.x = root->tile.board.x - root->sprite.width;
                    }

                    if (root->tile.board.y == player->tile.board.y &&
                        root->tile.board.x >= player->tile.board.x &&
                        root->tile.board.x - root->sprite.width <= player->tile.board.x) {

                        if (configuration.environment.lives > 0) {
                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                configuration.environment.lives--;
                            }

                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                player->tile.board.y = context.visual.height - player->sprite.height - 1;
                                player->tile.board.x = context.visual.width / 2 - 5;
                            }
                        } else {
                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                configuration.status.game_over = true;
                            }
                        }
                    }

                    if (root->tile.board.x <= -root->sprite.width) {
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            root->tile.board.x = context.visual.width - root->sprite.width;
                        }
                    }
                }

                break;
            case LILY:
                {
                    std::unique_lock<std::shared_mutex> lock(mutex);
                    root->sprite.track = root->sprite.track + 1;
                    root->pause = false;
                }

                std::this_thread::sleep_for(std::chrono::seconds(1));

                if (root->sprite.track == 3) {
                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        root->sprite.track = 0;
                        root->pause = true;
                    }

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }

                break;
            case RIGHT_LOG:
                if (root->tile.board.x <= context.visual.width) {
                    const int previous = root->tile.board.x;

                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        root->tile.board.x = root->tile.board.x + root->sprite.width / 2;
                    }

                    if (root->tile.board.y == player->tile.board.y) {
                        if (previous + player->sprite.width == player->tile.board.x) {
                            {
                                std::shared_lock<std::shared_mutex> lock(mutex);
                                if (player->tile.board.x <= context.visual.width - player->sprite.width) {
                                    player->tile.board.x = player->tile.board.x + player->sprite.width;
                                }
                            }
                        }
                    }

                    if (root->tile.board.y == player->tile.board.y) {
                        if (previous == player->tile.board.x) {
                            {
                                std::shared_lock<std::shared_mutex> lock(mutex);
                                player->tile.board.x = root->tile.board.x;
                            }
                        }
                    }

                    if (root->tile.board.x >= context.visual.width) {
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            root->tile.board.x = 0;
                        }
                    }
                }

                break;
            case LEFT_LOG:
                if (root->tile.board.x >= 0) {
                    const int previous = root->tile.board.x;

                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        root->tile.board.x = root->tile.board.x - root->sprite.width / 2;
                    }

                    {
                        std::shared_lock<std::shared_mutex> lock(mutex);
                        if (root->tile.board.y == player->tile.board.y) {
                            if (previous == player->tile.board.x) {
                                player->tile.board.x = root->tile.board.x;
                            }
                        }
                    }

                    {
                        std::shared_lock<std::shared_mutex> lock(mutex);
                        if (root->tile.board.y == player->tile.board.y) {
                            if (previous + player->sprite.width == player->tile.board.x) {
                                if (player->tile.board.x != 0) {
                                    player->tile.board.x = player->tile.board.x - player->sprite.width;
                                }
                            }
                        }
                    }

                    if (root->tile.board.x - root->sprite.width < -root->sprite.width) {
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            root->tile.board.x = context.visual.width - root->sprite.width / 2;
                        }
                    }
                }

                break;
            default:
                break;
        }

        if (root->tile.type != LILY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(cycle));
        }
    }
}
