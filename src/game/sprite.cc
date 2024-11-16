#include "sprite.hpp"

#include <algorithm>

struct GEOMETRY {
    mutable int height;
    mutable int width;
};

auto calculate(const std::array<char, 512> &symbol) -> GEOMETRY {
    int height = 1, width = 1;

    for (const auto &character : symbol) {
        if (character == '\0') {
            break;
        }

        switch (character) {
            case '\n':
                height = height + 1;
                break;
            default:
                width = width + 1;
                break;
        }
    }

    return GEOMETRY{.height = height, .width = width};
}

auto sprite(const std::string &data) -> SPRITE {
    std::array<char, 512> symbol{};

    std::copy_n(data.begin(), std::min(data.size(), data.length() - 1), symbol.begin());

    auto [height, width] = calculate(symbol);

    return SPRITE{
        .symbol = symbol,
        .height = height,
        .width = width,
    };
}
