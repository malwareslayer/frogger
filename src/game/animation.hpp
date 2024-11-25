#pragma once

#include <shared_mutex>

#include "../../src/configuration.hpp"

#include "../../src/game/interface.hpp"
#include "../../src/game/gameplay.hpp"

void animate(std::shared_mutex &mutex, const std::shared_ptr<NODE> &root, const std::shared_ptr<NODE> &player, const INTERFACE &context, const CONFIGURATION &configuration, const int &cycle);
