#include <optional>
#include <random>
#include <thread>
#include <mutex>
#include <shared_mutex>

#include "../../src/interface/game.hpp"

#include "../../src/game/interface.hpp"
#include "../../src/game/sprite.hpp"
#include "../../src/game/tile.hpp"

#include "../../src/game/symbols/car.hpp"
#include "../../src/game/symbols/frog.hpp"
#include "../../src/game/symbols/lily.hpp"
#include "../../src/game/symbols/log.hpp"

std::shared_mutex mutex;

struct NODE {
    mutable int index;
    mutable bool active;
    mutable bool pause = false;
    mutable std::thread worker;
    mutable SPRITE sprite;
    mutable TILE tile;
    mutable NODE* previous = nullptr;
    mutable NODE* next = nullptr;
};

auto total(NODE* &root, const std::optional<TYPE> &type = std::nullopt) -> int {
    int index = 0;

    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        for (const NODE *current = root; current != nullptr; current = current->next) {
            if (type.has_value()) {
                if (type.value() == current->tile.type) {
                    index++;
                }
            } else {
                index++;
            }

            if (!current->next) {
                break;
            }
        }
    }

    return index;
}

auto create(NODE* &root, NODE* &previous, const TILE &tile, const SPRITE &sprite, const bool &active = true) -> NODE* {
    for (NODE* current = root; current != nullptr; current = current->next) {
        if (current == previous) {
            if (!current->next && !previous->next) {
                current->next = new NODE {
                    .index = current->index + 1,
                    .active = active,
                    .sprite = sprite,
                    .tile = tile,
                    .previous = previous,
                };
            }
        }
    }

    return previous->next;
}

auto create(NODE* &root, NODE* &previous, NODE* &newest) -> NODE* {
    for (NODE* current = root; current != nullptr; current = current->next) {
        if (current == previous) {
            if (!current->next && !previous->next) {
                newest->index = current->index + 1;
                newest->previous = current;

                current->next = newest;
            }
        }
    }

    return previous->next;
}

void animate(WINDOW* &window, const NODE* root, const NODE* player, const INTERFACE &context, const CONFIGURATION &configuration, const int &cycle) {
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
                                player->tile.board.x = (context.visual.width + 2) / 2;
                            }
                        } else {
                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                configuration.status.play = false;
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
                                player->tile.board.x = (context.visual.width + 2) / 2;
                            }
                        } else {
                            {
                                std::unique_lock<std::shared_mutex> lock(mutex);
                                configuration.status.play = false;
                                configuration.status.game_over = true;
                            }
                        }
                    }

                    if (root->tile.board.x <= -root->sprite.width) {
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            root->tile.board.x = context.visual.width - root->sprite.width + 2;
                        }
                    }
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
                                player->tile.board.x = player->tile.board.x + player->sprite.width;
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
                                player->tile.board.x = player->tile.board.x - player->sprite.width;
                            }
                        }
                    }

                    if (root->tile.board.x - root->sprite.width < -root->sprite.width) {
                        {
                            std::unique_lock<std::shared_mutex> lock(mutex);
                            root->tile.board.x = context.visual.width + 2 - root->sprite.width / 2;
                        }
                    }
                }
                break;
            default:
                break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(cycle));
    }
}

void play(NODE* &root, WINDOW* &window, const INTERFACE &context, const CONFIGURATION &configuration) {
    std::random_device seed;
    std::mt19937 generate(seed());
    std::uniform_int_distribution<int> distribution_car(1, configuration.environment.car);
    std::uniform_int_distribution<int> distribution_log(1, configuration.environment.log);

    // First Separator
    auto last = create(root, root, tile(SEPARATOR, context.visual.height - 1 - root->sprite.height - 1, 0), {});

    int initial = total(root);

    // Generate Car
    for (size_t i = 1; i <= 4; i++) {
        for (NODE* current = root; current != nullptr; current = current->next) {
            const int now = total(root);

            if (current->index < now) {
                continue;
            }

            const int random_car = distribution_car(generate);

            if (initial != now) {
                initial = now;
                break;
            }

            if (!current->next) {
                if (i % 2 != 0) {
                    const SPRITE rcar = sprite(RCAR);

                    {
                        std::unique_lock<std::shared_mutex> lock(mutex);
                        for (int j = now + 1; j <= now + random_car; j++) {
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
                        for (int j = now + 1; j <= now + random_car; j++) {
                            switch (last->tile.type) {
                                case SEPARATOR:
                                    last = create(last, last, tile(LEFT_CAR, last->tile.board.y - lcar.height, context.visual.width - lcar.width), lcar, false);
                                    break;
                                case RIGHT_CAR:
                                    last = create(last, last, tile(LEFT_CAR, last->tile.board.y - lcar.height - 1, context.visual.width + 2), lcar, false);
                                    break;
                                case LEFT_CAR:
                                    last = create(last, last, tile(LEFT_CAR, last->tile.board.y, context.visual.width + 2 - lcar.width), lcar, false);
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
            ax = last->tile.board.x + x.width * 2;
        } else if (i == 3) {
            ax = last->tile.board.x + x.width * 5;
        }else {
            ax = last->tile.board.x + x.width * 3;
        }

        for (size_t j = 1; j <= 3; j++) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                auto lily = new NODE {
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
                };

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
        int dx = context.visual.width + 2 + log.width;

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

    // Activate Object Gameplay
    for (NODE* current = root->next; current != nullptr; current = current->next) {
        if (current->active == false && (current->tile.type != SEPARATOR || current->tile.type != PLAYER || current->tile.type != WATER)) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);

                int cycle = 250;

                if (current->tile.type == LEFT_LOG || current->tile.type == RIGHT_LOG) {
                    cycle = 1250;
                }

                current->active = true;
                current->worker = std::thread(animate, std::ref(window), current, root, std::ref(context), std::ref(configuration), cycle);
            }
        }
    }

    // Player Gameplay Interface
    while (configuration.status.play) {
        if (const std::string interface = keypad(window, context, root->sprite, root->tile); interface == "q") {
            break;
        }

        for (const NODE* current = root->next; current != nullptr; current = current->next) {
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

    // Stop Interface And Make Thread Done
    configuration.status.play = false;
}

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

    WINDOW *window = create(context.visual.y, context.visual.x, context.visual.height, context.visual.width);

    nodelay(window, true);
    keypad(window, true);
    clearok(window, true);

    {
        std::unique_lock<std::shared_mutex> lock(mutex);
        configuration.status.play = true;
    }

    const SPRITE frog = sprite(LFROG);
    const TILE player = tile(PLAYER, context.visual.height - frog.height - 1, context.visual.width / 2 + 1);

    auto root = new NODE {
        .index = 1,
        .active = true,
        .sprite = frog,
        .tile = player
    };

    std::thread playing(play, std::ref(root), std::ref(window), std::ref(context), std::ref(configuration));

    while (configuration.status.running) {
        wclear(window);

        wbkgd(window, ' ' | A_NORMAL);

        werase(window);

        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            if (configuration.status.play == false) {
                break;
            }

            for (const NODE *current = root; current != nullptr; current = current->next) {
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

    for (const NODE* current = root; current != nullptr; current = current->next) {
        if (current->active == true && (current->tile.type == RIGHT_CAR || current->tile.type == LEFT_CAR || current->tile.type == LILY)) {
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                current->active = false;
            }

            current->worker.join();
        }
    }

    playing.join();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    wclear(window);
    wclrtoeol(window);
    wrefresh(window);
    werase(window);
    delwin(window);

    delete root;
}
