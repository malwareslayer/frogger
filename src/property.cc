/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#include "../src/property.hpp"

auto choose(WINDOW* window, const PROPERTY& property, const std::vector<std::string>& labels) -> std::string {
    const int maximum = static_cast<int>(labels.size());

    switch (wgetch(window)) {
        case KEY_UP:
            property.choose--;

            if (property.choose < 1) {
                property.choose = maximum;
            }

            wclear(window);

            break;
        case KEY_DOWN:
            property.choose++;

            if (property.choose > maximum) {
                property.choose = 1;
            }

            wclear(window);

            break;
        case KEY_RIGHT:
            return "KEY_RIGHT";
        case KEY_LEFT:
            return "KEY_LEFT";
        case 10:
            return labels[property.choose - 1];
        default:
            break;
    }

    return std::string{""};
}
