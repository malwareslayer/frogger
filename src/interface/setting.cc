#include "setting.hpp"

#include "../property.hpp"
#include "../configuration.hpp"

const std::vector<std::string> labeling = {"Car", "Log", "Back"};

void setting(WINDOW *window, const CONFIGURATION &configuration) {
    constexpr INTERFACE context = {
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
                    bar(window, configuration.environment.car, 1, 10);
                }
            } else if (labeling[context.interface.choose - 1] == "Log") {
                if (configuration.environment.log < 5) {
                    increase(configuration, labeling[context.interface.choose - 1]);
                    bar(window, configuration.environment.log, 1, 10);
                }
            }
        } else if (choosing == "KEY_LEFT") {
            if (labeling[context.interface.choose - 1] == "Car") {
                if (configuration.environment.car > 0) {
                    decrease(configuration, labeling[context.interface.choose - 1]);
                    bar(window, configuration.environment.car, 1, 10);
                }
            } else if (labeling[context.interface.choose - 1] == "Log") {
                if (configuration.environment.log > 0) {
                    decrease(configuration, labeling[context.interface.choose - 1]);
                    bar(window, configuration.environment.log, 1, 10);
                }
            }
        } else {
            wclear(window);
        }

        boxes(window, context.interface.choose, labeling);
    }

    wclear(window);
}