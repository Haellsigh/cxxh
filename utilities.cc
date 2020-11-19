#include "utilities.hh"

namespace ch::utilities
{

auto split_last(std::string_view string, std::string_view delimiter) -> std::tuple<std::string_view, std::string_view>
{
    size_t pos = string.rfind(delimiter);

    std::string_view first;

    if (pos != std::string::npos)
    {
        return {string.substr(0, pos), string.substr(pos + delimiter.length())};
    }

    return {first, string};
}

} // namespace ch::utilities
