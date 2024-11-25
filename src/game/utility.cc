#include "../../src/game/utility.hpp"

auto total(const std::shared_ptr<NODE> &root, const std::optional<TYPE> &type) -> int {
    int index = 0;

    for (std::shared_ptr<NODE> current = root; current != nullptr; current = current->next) {
        if (type.has_value()) {
            if (type.value() == current->tile.type) {
                index++;
            }
        } else {
            index++;
        }

        if (!current->next) {
            break;
        }
    }

    return index;
}
