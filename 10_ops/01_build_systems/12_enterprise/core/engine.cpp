#include "core/engine.h"
#include "utils/string_utils.h"

namespace core {

std::string process(const std::string& input) {
    return utils::to_upper(utils::trim(input));
}

}  // namespace core
