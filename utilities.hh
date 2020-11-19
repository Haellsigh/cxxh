#pragma once

#include <algorithm>
#include <array>
#include <string_view>

namespace ch::utilities {

/**
 * \brief Splits a string_view at the last occurence of delimiter.
 * \return A pair of string_views containing the parts before and after the delimiter.
 */
auto split_last(std::string_view string, std::string_view delimiter) -> std::tuple<std::string_view, std::string_view>;

template <typename... Delimiters>
auto split_at_every(std::string_view string, Delimiters... delimiters)
    -> std::pair<std::string_view, std::array<std::string_view, sizeof...(Delimiters)>>
{
    std::array<std::string_view, sizeof...(Delimiters)> parts;
    std::size_t index = 0;
    std::size_t offset = 0;

    for (std::string_view delimiter : {delimiters...})
    {
        auto substr = string.substr(offset);
        std::size_t position = substr.rfind(delimiter);

        if (position != std::string_view::npos)
        {
            // auto start = offset + delimiter.length();
            parts[index] = string.substr(offset, position);
            offset += position + delimiter.length();
        }
        index++;
    }

    auto rest = string.substr(offset);

    return std::make_pair(rest, parts);
}

} // namespace ch::utilities
