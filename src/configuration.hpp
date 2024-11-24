/*
 * Copyright 2024 <Team 1 UPH IF 2024>
 */

#pragma once

#include <string>

struct ENVIRONMENT {
    mutable int car;
    mutable int log;
    mutable int lives = 3;
};

struct STATUS {
    mutable bool running;
    mutable bool play;
};

struct CONFIGURATION {
    STATUS status;
    ENVIRONMENT environment;
};

void increase(const CONFIGURATION &configuration, const std::string &label);

void decrease(const CONFIGURATION &configuration, const std::string &label);
