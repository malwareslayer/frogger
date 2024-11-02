#pragma once

#include <string>

struct ENVIRONMENT {
    mutable int car;
    mutable int log;
};

struct STATUS {
    mutable bool running;
};

struct CONFIGURATION {
    STATUS status;
    ENVIRONMENT environment;
};

void increase(const CONFIGURATION &configuration, const std::string &label);

void decrease(const CONFIGURATION &configuration, const std::string &label);