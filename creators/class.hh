#pragma once

namespace Wizards {

constexpr std::string_view class_template =
    "class {class_name} {\n"
    " public:\n"
    "  {class_name}() = default;\n"
    "  ~{class_name}() = default;\n"
    "};\n";

class Class {
 public:
  Class()  = default;
  ~Class() = default;
};

}  // namespace Wizards
