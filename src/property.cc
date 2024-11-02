#include "property.hpp"

std::string choose(WINDOW *window, const PROPERTY &property, const std::vector<std::string> &labels) {
    const int maximum = static_cast<int>(labels.size());

    switch (wgetch(window)) {
        case KEY_UP:
            property.choose--;

            if (property.choose < 1) {
                property.choose = maximum;
            }

            break;
        case KEY_DOWN:
            property.choose++;

            if (property.choose > maximum) {
                property.choose = 1;
            }

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

    return std::string { "" };
}
