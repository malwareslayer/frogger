#pragma once

#include <optional>

#include "../../src/game/node.hpp"

auto total(const std::shared_ptr<NODE> &root, const std::optional<TYPE> &type = std::nullopt) -> int;
