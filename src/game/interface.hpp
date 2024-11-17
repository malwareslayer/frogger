#pragma once

#include "../../src/property.hpp"

#include "../../src/engine/window.hpp"

#include "../../src/game/sprite.hpp"
#include "../../src/game/tile.hpp"

auto keypad(WINDOW *window, const INTERFACE &context, const SPRITE &sprite, const TILE &tile) -> std::string;
