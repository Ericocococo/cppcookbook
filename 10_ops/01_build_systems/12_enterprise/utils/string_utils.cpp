#include "utils/string_utils.h"
#include <algorithm>
#include <cctype>

namespace utils {

std::string to_upper(std::string s) {
    std::ranges::transform(s, s.begin(), [](unsigned char c) { return std::toupper(c); });
    return s;
}

std::string trim(std::string s) {
    auto not_space = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::ranges::find_if(s, not_space));
    s.erase(std::ranges::find_if(s | std::views::reverse, not_space).base(), s.end());
    return s;
}

}  // namespace utils
