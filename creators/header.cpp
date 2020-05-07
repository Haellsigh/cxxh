#include "header.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <boost/locale.hpp>

#include <creators/utilities.hh>
#include <parsers/identifiers.hh>

namespace po = boost::program_options;

namespace cxxh::Creators::Header {

constexpr std::string_view header_template =
    "{include_guard_begin}"  // 1 \n after
    "{includes}"             // 1 \n before, 1 \n after
    "{namespaces_begin}"     // 1 \n before, 2 \n after
    "{content}"
    "{namespaces_end}"     // 2 \n before
    "{include_guard_end}"  // 2 \n before
    "\n";

bool create(boost::program_options::variables_map& vm, const std::string& content) {
  // Uppercase and lowercase conversions
  auto locale = boost::locale::generator()("en.UTF-8");

  // Options
  const auto& identifiers = vm["header"].as<std::vector<std::string>>();

  const auto& verbose       = vm["verbose"].as<bool>();
  const auto& dry_run       = vm["dry-run"].as<bool>();
  const auto& lowercase     = vm["lowercase"].as<bool>();
  const auto& force         = vm["force"].as<bool>();
  const auto& include_guard = vm["include-guard"].as<std::string>();
  const auto& extension     = boost::locale::to_lower(
      utilities::replace_all(vm["header-ext"].as<std::string>(), ".", ""), locale);

  // Includes
  std::string includes;
  if (vm.count("include")) {
    auto include_list = vm["include"].as<std::vector<std::string>>();
    includes.append("\n");
    for (auto include : include_list)
      includes.append("#include <" + include + ">\n");
  }

  for (const auto& identifier : identifiers) {
    auto [directories, namespaces, name] = Parsers::Identifiers::extract(identifier);

    if (vm.count("folder")) {
      directories = vm["folder"].as<std::string>() / directories;
    }

    if (verbose) {
      std::cout << "Parsing identifier: " << identifier << "\n";
      std::cout << "  - directory: " << directories << "\n";
      std::cout << "  - namespace: " << namespaces << "\n";
      std::cout << "  - filename:  " << name << "\n";
    }

    // Include guard
    std::string include_guard_begin, include_guard_end;
    if (include_guard == "pragma")
      include_guard_begin = "#pragma once\n";
    else if (include_guard == "ifndef") {
      auto include_guard_name = boost::locale::to_upper(
          utilities::replace_all(namespaces, "::", "_") +
              (namespaces.empty() ? "" : "_") + name + "_" + extension,
          locale);
      include_guard_begin = fmt::format("#ifndef {0}\n#define {0}\n", include_guard_name);
      include_guard_end   = fmt::format("\n\n#endif // {}", include_guard_name);
    }

    // Namespaces
    std::string namespaces_begin, namespaces_end;
    if (!namespaces.empty()) {
      namespaces_begin = fmt::format("\nnamespace {} {{\n\n", namespaces);
      namespaces_end   = fmt::format("\n\n}} // namespace {}", namespaces);
    }

    // Format header template
    auto header_contents = fmt::format(                        //
        header_template,                                       //
        fmt::arg("include_guard_begin", include_guard_begin),  //
        fmt::arg("include_guard_end", include_guard_end),      //
        fmt::arg("namespaces_begin", namespaces_begin),        //
        fmt::arg("namespaces_end", namespaces_end),            //
        fmt::arg("includes", includes),                        //
        fmt::arg("content", content)                           //
    );

    auto filename =
        (lowercase ? boost::locale::to_lower(name, locale) : name) + "." + extension;

    auto current_path = std::filesystem::current_path();
    if (!dry_run)
      utilities::build_directories(directories);
    auto header_path = current_path / directories / filename;
    header_path.make_preferred();

    if (!dry_run) {
      if (!force && utilities::file_exists(header_path)) {
        throw std::filesystem::filesystem_error(
            "File creation error", header_path,
            std::make_error_code(std::errc::file_exists));
      } else {
        std::ofstream header_file;
        header_file.open(header_path);
        header_file << header_contents;
        header_file.close();
      }
    }

    std::cout << "Created file " << header_path.lexically_relative(current_path).string()
              << "\n";
  }

  return true;
}

}  // namespace cxxh::Creators::Header
