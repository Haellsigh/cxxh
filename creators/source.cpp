#include "creators/source.hh"
#include "creators/utilities.hh"
#include "parsers/identifiers.hh"

#include <boost/locale.hpp>

#include <fstream>
#include <string_view>

namespace po = boost::program_options;

namespace cxxh::Creators::Source
{

constexpr std::string_view source_template = "{includes}"         // 1 \n after
                                             "{namespaces_begin}" // 1 \n before, 2 \n after
                                             "{contents}"
                                             "{namespaces_end}" // 2 \n before
                                             "\n";

bool create_from_identifiers(boost::program_options::variables_map &vm, std::vector<std::string> const &identifiers,
                             std::string const &contents, std::vector<std::string> const &include_list)
{
    // Uppercase and lowercase conversions
    auto locale = boost::locale::generator()("en.UTF-8");

    const auto &verbose = vm["verbose"].as<bool>();
    const auto &dry_run = vm["dry-run"].as<bool>();
    const auto &lowercase = vm["lowercase"].as<bool>();
    const auto &force = vm["force"].as<bool>();
    const auto &extension = boost::locale::to_lower(vm["source-ext"].as<std::string>(), locale);

    for (const auto &identifier : identifiers)
    {
        auto [directories, namespaces, name] = Parsers::Identifiers::extract(identifier);

        // Prepend folder
        if (vm.count("folder"))
        {
            directories = vm["folder"].as<std::string>() / directories;
        }
        directories = std::filesystem::current_path() / directories;

        if (verbose)
        {
            std::cout << "Parsing identifier: " << identifier << "\n";
            std::cout << "  - directory: " << directories << "\n";
            std::cout << "  - namespace: " << namespaces << "\n";
            std::cout << "  - filename:  " << name << "\n";
        }

        // Includes
        std::string includes;
        if (include_list.size())
        {
            for (auto include : include_list)
                includes.append("#include \"" + include + "\"\n");
        }

        // Namespaces
        std::string namespaces_begin, namespaces_end;
        if (!namespaces.empty())
        {
            namespaces_begin = fmt::format("\nnamespace {} {{\n\n", namespaces);
            namespaces_end = fmt::format("\n\n}} // namespace {}", namespaces);
        }

        // Format source template
        auto source_contents = fmt::format(                 //
            source_template,                                //
            fmt::arg("includes", includes),                 //
            fmt::arg("namespaces_begin", namespaces_begin), //
            fmt::arg("namespaces_end", namespaces_end),     //
            fmt::arg("contents", contents)                  //
        );

        auto filename = (lowercase ? boost::locale::to_lower(name, locale) : name) + "." + extension;

        if (!dry_run)
            std::filesystem::create_directories(directories);
        auto source_path = directories / filename;
        source_path.make_preferred();

        if (!dry_run)
        {
            if (!force && utilities::file_exists(source_path))
            {
                throw std::filesystem::filesystem_error("File creation error", source_path,
                                                        std::make_error_code(std::errc::file_exists));
            }
            else
            {
                std::ofstream source_file(source_path);
                source_file << source_contents;
                source_file.close();
            }
        }

        std::cout << "Created file " << source_path.lexically_relative(std::filesystem::current_path()).string()
                  << "\n";
    }

    return true;
}

bool create(po::variables_map &vm, const std::string &contents)
{
    // Options
    const auto &identifiers = vm["source"].as<std::vector<std::string>>();

    return create_from_identifiers(vm, identifiers, contents);
}

} // namespace cxxh::Creators::Source
