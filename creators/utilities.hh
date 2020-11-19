#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <sys/stat.h>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

namespace cxxh::Creators::utilities
{

namespace detail
{

template <typename FirstT, typename SecondT, std::size_t... Is, class Tuple>
std::vector<std::pair<FirstT, SecondT>> pack_pairs_impl(std::index_sequence<Is...>, Tuple tuple)
{
    return std::vector<std::pair<FirstT, SecondT>>{
        std::make_pair(std::get<2 * Is>(tuple), std::get<2 * Is + 1>(tuple))...};
}

} // namespace detail

template <typename FirstT, typename SecondT, typename... Args>
std::vector<std::pair<FirstT, SecondT>> pack_pairs(Args... args)
{
    return detail::pack_pairs_impl<FirstT, SecondT>(std::make_index_sequence<sizeof...(Args) / 2>{},
                                                    std::make_tuple(args...));
}

bool contains(std::string const &str, std::string const &seq);

std::vector<std::string> split(std::string string, const std::string &delimiter);

/**
 * \brief Splits a string at the last occurence of delimiter
 * \return A pair of strings.
 */
std::tuple<std::string_view, std::string_view> split_last(const std::string_view string,
                                                          const std::string_view delimiter);

void replace_all_inplace(std::string &str, const std::string &from, const std::string &to);
[[nodiscard]] std::string replace_all(const std::string &str, const std::string &from, const std::string &to);

bool file_exists(std::filesystem::path path);

} // namespace cxxh::Creators::utilities
