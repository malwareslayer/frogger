#include "../../src/game/node.hpp"

auto create(const std::shared_ptr<NODE> &root, const std::shared_ptr<NODE> &previous, const TILE &tile, const SPRITE &sprite, const bool &active) -> std::shared_ptr<NODE> {
    for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
        if (current == previous) {
            if (!current->next && !previous->next) {
                current->next = std::make_shared<NODE>(NODE {
                    .index = current->index + 1,
                    .active = active,
                    .sprite = sprite,
                    .tile = tile,
                    .previous = previous,
                });
            }
        }
    }

    return previous->next;
}

auto create(const std::shared_ptr<NODE> &root, const std::shared_ptr<NODE> &previous, const std::shared_ptr<NODE> &next) -> std::shared_ptr<NODE> {
    for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
        if (current == previous) {
            if (!current->next && !previous->next) {
                next->index = current->index + 1;
                next->previous = current;

                current->next = next;
            }
        }
    }

    return previous->next;
}
