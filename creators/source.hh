#pragma once

#include <string_view>

namespace Wizards {

constexpr std::string_view class_source_template =
    "#include \"{header_file_name}\"\n\n"
    "{namespace_begin}\n{namespace_end}";

class Source {
 public:
  static void create();
};

}  // namespace Wizards
