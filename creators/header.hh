#pragma once

#include <string_view>

namespace cxxh::Creators {

constexpr std::string_view header_template =
    "{header_guard_start}\n\n"
    "{namespace_begin}\n"
    "\n{content}\n"
    "{namespace_end}"
    "{header_guard_end}";

class Header {
 public:
  static void create(const std::string& content);
};

}  // namespace cxxh::Creators
