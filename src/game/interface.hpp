#pragma once

#include "../engine/window.hpp"

#include "../../src/game/tile.hpp"
#include "../../src/property.hpp"

auto keypad(WINDOW *window, const INTERFACE &context, const TILE &tile) -> std::string;
