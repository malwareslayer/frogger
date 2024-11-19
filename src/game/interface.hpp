#pragma once

#include "../../src/engine/window.hpp"
#include "../../src/game/sprite.hpp"
#include "../../src/game/tile.hpp"
#include "../../src/property.hpp"

auto keypad(WINDOW *window, const INTERFACE &context, const SPRITE &sprite, const TILE &tile) -> std::string;
