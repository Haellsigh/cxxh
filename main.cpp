#include <iostream>

#include <boost/program_options.hpp>

#include <creators/class.hh>
#include <creators/header.hh>
#include <creators/source.hh>

namespace po = boost::program_options;
using namespace cxxh;

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
  return os;
}

/*!
 * \brief Creates a class from the full identifier.
 *
 * fullIdentifier is in the format:
 *   [folder(s)][::namespaces::]className
 *
 * Example
 *   optional/relative/path/to/folder/optional::namespaces::ClassName
 *
 * \param fullIdentifier
 */
/*
void create_class(const std::string& pathAndIdentifier) {
  // Extract (directories, identifier) from pathAndIdentifier
  auto [directories, identifier] = utility::split_last(pathAndIdentifier, "/");
  // Extract (namespaces, name) from identifier
  auto [namespaces, name] = utility::split_last(identifier, "::");

  // Build file_name
  std::string file_name;
  file_name.resize(name.size());
  std::transform(std::begin(name), std::end(name), std::begin(file_name), ::tolower);

  std::string header_file_name = file_name + ".hh";
  std::string source_file_name = file_name + ".cpp";

  // Build namespace begin/end strings
  std::string namespace_begin =
      namespaces.empty() ? "" : "namespace " + namespaces + " {\n\n";
  std::string namespace_end =
      namespaces.empty() ? "" : "\n} // namespace " + namespaces + "\n";

  // Apply templates
  // clang-format off
  auto header = utility::parse_template(class_header_template,
    "header_guard_start", "#pragma once",
    "namespace_begin", namespace_begin,
    "class_name", name,
    "file_name", file_name,
    "namespace_end", namespace_end,
    "header_guard_end", ""
  );
  // clang-format on

  // clang-format off
  auto source = utility::parse_template(class_source_template,
    "header_guard_start", "#pragma once",
    "namespace_begin", namespaces.empty() ? "" : "namespace " + namespaces + " {\n\n",
    "class_name", name,
    "header_file_name", header_file_name,
    "namespace_end", namespaces.empty() ? "" : "\n} // namespace " + namespaces + "\n",
    "header_guard_end", ""
  );
  // clang-format on

  // Create folder structure
  auto current_path   = std::filesystem::current_path();
  auto directory_path = utility::build_directories(directories);

  // Generate file paths
  auto header_path = directory_path / header_file_name;
  auto source_path = directory_path / source_file_name;

  // Create & fill header file
  std::ofstream header_file;
  header_file.open(header_path);
  header_file << header;
  header_file.close();
  std::cout << "Created " << header_path.lexically_relative(current_path).string()
            << "\n";

  // Create & fill sourcefile
  std::ofstream source_file;
  source_file.open(source_path);
  source_file << source;
  source_file.close();
  std::cout << "Created " << source_path.lexically_relative(current_path).string()
            << "\n";
}
*/

/*!
 * \brief Checks that 'opt1' and 'opt2' are not specified at the same time.
 */
void conflicting_options(const po::variables_map& vm,
                         const char*              opt1,
                         const char*              opt2) {
  if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted())
    throw std::logic_error(std::string("Conflicting options '") + opt1 + "' and '" +
                           opt2 + "'.");
}

/*!
 * \brief Checks that option opt equals one of these values.
 *
 * \tparam T The type of the option.
 */
template <typename T, typename... Args>
void is_one_of(const po::variables_map& vm, const char* opt, Args... these) {
  auto value = vm[opt].as<T>();
  if (((value != these) && ...)) {
    throw std::domain_error(
        fmt::format("Invalid value '{}' for option '{}'", value, opt));
  }
}

int main(int argc, char** argv) {
  try {
    po::options_description generic("Generic options");
    generic.add_options()
        // Help
        ("help", "prints usage message")
        // Version
        ("version", "prints version information")
        // verbose
        ("verbose",
         po::value<bool>()->zero_tokens()->default_value(false)->implicit_value(true),
         "shows the list of files created")
        // Dry-run
        ("dry-run",
         po::value<bool>()->zero_tokens()->default_value(false)->implicit_value(true),
         "do not actually create files")
        // Dry-run
        ("force",
         po::value<bool>()->zero_tokens()->default_value(false)->implicit_value(true),
         "overwrite existing files");

    po::options_description usage("Creation options");
    usage.add_options()
        // Class
        ("class,c", po::value<std::vector<std::string>>()->multitoken(),
         "creates a class")
        // Header
        ("header,h", po::value<std::vector<std::string>>()->multitoken(),
         "creates a header file")
        // source
        ("source,s", po::value<std::vector<std::string>>()->multitoken(),
         "creates a source file");

    po::options_description options("Options");
    options.add_options()
        // Folder
        ("folder,f", po::value<std::string>(), "creates everything in arg folder")
        // Include
        ("include", po::value<std::vector<std::string>>()->multitoken(),
         "adds #include <arg> to header or source files")
        // Lowercase
        ("lowercase", po::value<bool>()->zero_tokens()->default_value(true),
         "creates lowercase filenames, default is true")
        // Include guard
        ("include-guard", po::value<std::string>()->default_value("pragma"),
         "include guard for header files, one of: pragma, ifndef")
        // Header extension
        ("header-ext", po::value<std::string>()->default_value("hh"),
         "header files extension, default is hh")
        // Source extension
        ("source-ext", po::value<std::string>()->default_value("cpp"),
         "source files extension, default is cpp");

    generic.add(usage).add(options);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).options(generic) /*.positional(p)*/.run(),
        vm);
    po::notify(vm);

    bool class_command  = vm.count("class");
    bool header_command = vm.count("header");
    bool source_command = vm.count("source");

    is_one_of<std::string>(vm, "include-guard", "pragma", "ifndef");

    if (vm.count("version")) {
      std::cout << "cxxh v0.3.0\n";
      return 0;
    }

    if (vm.count("help") || !(class_command || header_command || source_command)) {
      std::cout << "Usage: cxxh [options] <creation option> args\n";
      generic.print(std::cout);
      return 0;
    }

    if (class_command) {
      Creators::Class::create(vm);
    }

    if (header_command) {
      Creators::Header::create(vm);
    }

    if (source_command) {
      Creators::Source::create(vm);
    }

  } catch (std::filesystem::filesystem_error& e) {
    if (e.code() == std::errc::file_exists) {
      std::cout << "File " << e.path1().string() << " already exists\n";
      std::cout << "Run cxxh with --force to overwrite\n";
    }
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    return 1;
  }
  return 0;
}
