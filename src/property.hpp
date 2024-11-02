/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#pragma once

#include <string>
#include <vector>

#include "../src/engine/window.hpp"

struct PROPERTY {
    mutable int choose;
};

struct INTERFACE {
    VISUAL visual;
    PROPERTY interface;
};

auto choose(WINDOW* window, const PROPERTY& property, const std::vector<std::string>& labels) -> std::string;
