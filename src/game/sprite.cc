#include "../../src/game/sprite.hpp"

#include <algorithm>
#include <sstream>

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

    return GEOMETRY{.height = height, .width = width / height};
}

auto symbolize(const std::array<char, 512> &source) -> std::vector<std::string> {
    const std::string data(source.data());
    std::istringstream stream(data);
    std::string line;

    std::vector<std::string> result;

    while (std::getline(stream, line)) {
        result.push_back(line);
    }

    return result;
}

auto sprite(const std::string &data) -> SPRITE {
    std::array<char, 512> symbol{};

    std::copy_n(data.begin(), std::min(data.size(), data.length() - 1), symbol.begin());

    auto [height, width] = calculate(symbol);

    return SPRITE{
        .source = symbol,
        .symbol = symbolize(symbol),
        .height = height,
        .width = width,
    };
}
