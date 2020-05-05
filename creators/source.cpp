#include "source.hh"

#include <string_view>

namespace cxxh::Creators {

constexpr std::string_view class_source_template =
    "#include \"{header_file_name}\"\n\n"
    "{namespace_begin}\n{namespace_end}"
    "\n";

bool Source::create(boost::program_options::variables_map& vm) {
  return true;
}

}  // namespace cxxh::Creators
