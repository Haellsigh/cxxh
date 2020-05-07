#include "source.hh"

#include <string_view>

#include <boost/locale.hpp>

#include <creators/utilities.hh>
#include <parsers/identifiers.hh>

namespace po = boost::program_options;

namespace cxxh::Creators::Source {

constexpr std::string_view class_source_template =
    "#include \"{header_file_name}\"\n\n"
    "{namespace_begin}\n{namespace_end}"
    "\n";

bool create(boost::program_options::variables_map& vm) {
  // Uppercase and lowercase conversions
  auto locale = boost::locale::generator()("en.UTF-8");

  // Options
  const auto& identifiers = vm["source"].as<std::vector<std::string>>();

  const auto& verbose   = vm["verbose"].as<bool>();
  const auto& dry_run   = vm["dry-run"].as<bool>();
  const auto& lowercase = vm["lowercase"].as<bool>();
  const auto& force     = vm["force"].as<bool>();
  const auto& extension = boost::locale::to_lower(
      utilities::replace_all(vm["header-ext"].as<std::string>(), ".", ""), locale);

  // Includes
  std::string includes;
  if (vm.count("include")) {
    auto include_list = vm["include"].as<std::vector<std::string>>();
    includes.append("\n");
    std::cout << "include list: ";
    for (auto include : include_list) {
      std::cout << include << " ";
      includes.append("#include <" + include + ">\n");
    }
    std::cout << "\n";
  }

  return true;
}

}  // namespace cxxh::Creators::Source
