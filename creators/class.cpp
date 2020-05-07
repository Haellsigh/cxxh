#include "class.hh"

#include <string_view>

namespace cxxh::Creators::Class {

constexpr std::string_view class_template =
    "class {class_name} {\n"
    " public:\n"
    "  {class_name}() = default;\n"
    "  ~{class_name}() = default;\n"
    "};\n";

bool create(boost::program_options::variables_map& vm) {
  return true;
}

}  // namespace cxxh::Creators::Class
