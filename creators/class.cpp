#include "creators/class.hh"
#include "creators/header.hh"
#include "creators/source.hh"
#include "parsers/identifiers.hh"

#include <boost/locale.hpp>
#include <fmt/format.h>

namespace cxxh::Creators::Class
{

constexpr std::string_view header_class_template = "class {class_name} {{\n"
                                                   " public:\n"
                                                   "  {class_name}() = default;\n"
                                                   "  ~{class_name}() = default;\n"
                                                   "}};\n";

bool create(boost::program_options::variables_map &vm)
{
    // Uppercase and lowercase conversions
    auto locale = boost::locale::generator()("en.UTF-8");

    // Options
    const auto &identifiers = vm["class"].as<std::vector<std::string>>();
    const auto &lowercase = vm["lowercase"].as<bool>();
    const auto &header_extension = boost::locale::to_lower(vm["header-ext"].as<std::string>(), locale);

    for (const auto &identifier : identifiers)
    {
        auto [directories, namespaces, name] = Parsers::Identifiers::extract(identifier);

        // Format header class template
        auto header_contents = fmt::format( //
            header_class_template,          //
            fmt::arg("class_name", name)    //
        );

        auto header_filename = (lowercase ? boost::locale::to_lower(name, locale) : name) + "." + header_extension;

        Header::create_from_identifiers(vm, {identifier}, header_contents);
        Source::create_from_identifiers(vm, {identifier}, "", {header_filename});
    }

    return true;
}

} // namespace cxxh::Creators::Class
