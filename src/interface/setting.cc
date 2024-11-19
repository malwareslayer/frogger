/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#include "../../src/interface/setting.hpp"

#include "../../src/configuration.hpp"
#include "../../src/property.hpp"

#include <thread>

const std::vector<std::string> labeling = {"Car", "Log", "Back"};

void setting(WINDOW *window, const CONFIGURATION &configuration) {
    constexpr INTERFACE context = {
        .visual = {
            .y = 10,
            .x = 30
        },
        .interface = {
            .choose = 1,
        }
    };

    boxes(window, context.interface.choose, labeling);

    while (configuration.status.running) {
        if (const std::string choosing = choose(window, context.interface, labeling); choosing == "Back") {
            break;
        } else if (choosing == "KEY_RIGHT") {
            if (labeling[context.interface.choose - 1] == "Car") {
                if (configuration.environment.car < 5) {
                    increase(configuration, labeling[context.interface.choose - 1]);
                }

                bar(window, configuration.environment.car, 1, 10);
            } else if (labeling[context.interface.choose - 1] == "Log") {
                if (configuration.environment.log < 5) {
                    increase(configuration, labeling[context.interface.choose - 1]);
                }

                bar(window, configuration.environment.log, 1, 10);
            }
        } else if (choosing == "KEY_LEFT") {
            if (labeling[context.interface.choose - 1] == "Car") {
                if (configuration.environment.car > 0) {
                    decrease(configuration, labeling[context.interface.choose - 1]);
                }

                bar(window, configuration.environment.car, 1, 10);
            } else if (labeling[context.interface.choose - 1] == "Log") {
                if (configuration.environment.log > 0) {
                    decrease(configuration, labeling[context.interface.choose - 1]);
                }

                bar(window, configuration.environment.log, 1, 10);
            }
        } else {
            if (labeling[context.interface.choose - 1] == "Car") {
                bar(window, configuration.environment.car, 1, 10);
            } else if (labeling[context.interface.choose - 1] == "Log") {
                bar(window, configuration.environment.log, 1, 10);
            }
        }

        wrefresh(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        boxes(window, context.interface.choose, labeling);
    }
}
