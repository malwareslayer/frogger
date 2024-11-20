#include "../../src/game/sprite.hpp"

#include <algorithm>
#include <sstream>

struct GEOMETRY {
    mutable int height;
    mutable int width;
};

auto calculate(const std::vector<std::string> &symbol) -> GEOMETRY {
    return GEOMETRY {
        .height = static_cast<int>(symbol.size()),
        .width = static_cast<int>(symbol[0].size()) + 1
    };
}

auto symbolize(const std::string &source) -> std::vector<std::string> {
    std::istringstream stream(source);
    std::string line;

    std::vector<std::string> result;

    while (std::getline(stream, line)) {
        result.push_back(line);
    }

    return result;
}

auto sprite(const std::string &data) -> SPRITE {
    const std::vector<std::string> symbol = symbolize(data);

    auto [height, width] = calculate(symbol);

    return SPRITE {
        .symbol = symbol,
        .height = height,
        .width = width,
    };
}
