#include "../../src/game/interface.hpp"

auto key(WINDOW* window) -> std::string {
    switch (wgetch(window)) {
        case 'q':
            return "q";
        default:
            break;
    }

    return std::string{""};
}
