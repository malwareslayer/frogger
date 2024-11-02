/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#include "../src/configuration.hpp"

void increase(const CONFIGURATION& configuration, const std::string& label) {
    if (label == "Car") {
        configuration.environment.car++;
    } else if (label == "Log") {
        configuration.environment.log++;
    }
}

void decrease(const CONFIGURATION& configuration, const std::string& label) {
    if (label == "Car") {
        configuration.environment.car--;
    } else if (label == "Log") {
        configuration.environment.log--;
    }
}
