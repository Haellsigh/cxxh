#include "header.hh"

#include <filesystem>
#include <iostream>
#include <string_view>

#include <boost/program_options.hpp>

#include "../utilities.hh"

namespace po = boost::program_options;

namespace cxxh::Creators {

constexpr std::string_view header_template =
    "{header_guard_start}\n\n"
    "{namespace_begin}\n"
    "\n{content}\n"
    "{namespace_end}"
    "{header_guard_end}";

bool Header::create(boost::program_options::variables_map& vm) {
  const auto& identifiers = vm["header"].as<std::vector<std::string>>();

  bool lowercase_filenames = vm["lowercase"].as<bool>();
  bool use_pragma          = vm["pragma"].as<bool>();

  for (const auto& identifier : identifiers) {
    // Extract (directories, identifier) from identifier
    auto [directories, nameIdentifier] = utility::split_last(identifier, "/");
    // Extract (namespaces, name) from cxxIdentifier
    auto [namespaces, name] = utility::split_last(nameIdentifier, "::");

    std::cout << "directories:" << directories << "\n";
    std::cout << "namespaces:" << namespaces << "\n";
    std::cout << "name:" << name << "\n";
  }

  return true;
}

}  // namespace cxxh::Creators
