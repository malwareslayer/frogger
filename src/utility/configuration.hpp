/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#pragma once

#include <chrono>
#include <string>

struct ENVIRONMENT {
    mutable int car;
    mutable int log;
    mutable int lives;
};

struct STATUS {
    mutable bool running;
    mutable bool play;
    mutable bool game_over = false;
    mutable bool game_win = false;
    mutable std::chrono::system_clock::time_point now;
    mutable std::chrono::system_clock::time_point last;
};

struct CONFIGURATION {
    STATUS status;
    ENVIRONMENT environment;
};

void increase(const CONFIGURATION &configuration, const std::string &label);

void decrease(const CONFIGURATION &configuration, const std::string &label);
