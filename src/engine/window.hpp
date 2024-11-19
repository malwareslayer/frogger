#pragma once

#include <string>
#include <vector>

#include "ncurses.h"

struct VISUAL {
    mutable int y;
    mutable int x;
    mutable int height;
    mutable int width;
};

auto create(const int &y, const int &x, const int &height, const int &width) -> WINDOW*;

void boxes(WINDOW* &window, const int &highlight, const std::vector<std::string> &choices);

void bar(WINDOW* &window, const int &data, const int &y, const int &x);
