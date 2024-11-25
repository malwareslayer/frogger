#pragma once

#include <shared_mutex>

#include "../../src/utility/configuration.hpp"

#include "../../src/game/node.hpp"
#include "../../src/game/interface.hpp"

void play(std::shared_mutex &mutex, std::shared_ptr<NODE> &root, WINDOW* &window, const INTERFACE &context, const CONFIGURATION &configuration);
