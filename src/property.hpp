#pragma once

#include "engine/window.hpp"

#include <string>
#include <vector>

struct PROPERTY {
    mutable int choose;
};

struct INTERFACE {
    VISUAL visual;
    PROPERTY interface;
};

std::string choose(WINDOW *window, const PROPERTY &property, const std::vector<std::string> &labels);