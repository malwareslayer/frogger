#include "configuration.hpp"

void increase(const CONFIGURATION &configuration, const std::string &label) {
    if (label == "Car") {
        configuration.environment.car++;
    } else if (label == "Log") {
        configuration.environment.log++;
    }
}

void decrease(const CONFIGURATION &configuration, const std::string &label) {
    if (label == "Car") {
        configuration.environment.car--;
    } else if (label == "Log") {
        configuration.environment.log--;
    }
}