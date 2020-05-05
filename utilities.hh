#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

namespace utility {

namespace detail {

template <typename FirstT, typename SecondT, std::size_t... Is, class Tuple>
std::vector<std::pair<FirstT, SecondT>> pack_pairs_impl(std::index_sequence<Is...>,
                                                        Tuple tuple) {
  return std::vector<std::pair<FirstT, SecondT>>{
      std::make_pair(std::get<2 * Is>(tuple), std::get<2 * Is + 1>(tuple))...};
}

}  // namespace detail

template <typename FirstT, typename SecondT, typename... Args>
std::vector<std::pair<FirstT, SecondT>> pack_pairs(Args... args) {
  return detail::pack_pairs_impl<FirstT, SecondT>(
      std::make_index_sequence<sizeof...(Args) / 2>{}, std::make_tuple(args...));
}

std::vector<std::string> split(std::string string, const std::string& delimiter);

/**
 * \brief Splits a string at the last occurence of delimiter
 * \return A pair of strings.
 */
std::array<std::string, 2> split_last(std::string string, const std::string& delimiter);

void replace_all(std::string& str, const std::string& from, const std::string& to);

std::filesystem::path build_directories(std::string directories);

template <typename... Args>
std::string parse_template(const std::string_view& template_string, Args... args) {
  std::vector<std::pair<std::string, std::string>> pairs =
      utility::pack_pairs<std::string, std::string>(args...);

  std::string contents{template_string};

  for (auto pair : pairs) {
    replace_all(contents, "{" + pair.first + "}", pair.second);
  }
  return contents;
}

}  // namespace utility
