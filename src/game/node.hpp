#pragma once

#include <memory>
#include <thread>

#include "../../src/game/sprite.hpp"
#include "../../src/game/tile.hpp"

struct NODE {
    mutable int index = 0;
    mutable bool active;
    mutable bool pause = false;
    mutable std::thread worker = std::thread();
    mutable SPRITE sprite;
    mutable TILE tile = {};
    mutable std::shared_ptr<NODE> previous = nullptr;
    mutable std::shared_ptr<NODE> next = nullptr;
};

auto create(const std::shared_ptr<NODE> &root, const std::shared_ptr<NODE> &previous, const TILE &tile, const SPRITE &sprite, const bool &active = true) -> std::shared_ptr<NODE>;

auto create(const std::shared_ptr<NODE> &root, const std::shared_ptr<NODE> &previous, const std::shared_ptr<NODE> &next) -> std::shared_ptr<NODE>;
