#include "identifiers.hh"

#include <stdexcept>

#include <creators/utilities.hh>

namespace cxxh::Parsers::Identifiers
{

template <typename... Args> bool contains_any(const std::string_view string, const Args... patterns)
{
    return ((string.find(patterns) != std::string_view::npos) || ...);
}

bool verify(const std::vector<std::string_view> &identifiers)
{
    for (const auto &identifier : identifiers)
    {
        if (contains_any(identifier, ":", "?", "<", ">", "\"", "|", "*", ":"))
        {
            return false;
        }
    }

    return true;
}

std::tuple<std::filesystem::path, std::string, std::string> extract(const std::string_view identifier)
{
    // Extract (directories, nameIdentifier) from identifier
    auto [directories_string, nameIdentifier] = Creators::utilities::split_last(identifier, "/");
    // Extract (namespaces, name) from nameIdentifier
    auto [namespaces, name] = Creators::utilities::split_last(nameIdentifier, "::");

    return {directories_string, std::string{namespaces}, std::string{name}};
}

} // namespace cxxh::Parsers::Identifiers
