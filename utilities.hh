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

std::vector<std::string> split(std::string string, const std::string& delimiter) {
  std::vector<std::string> tokens;

  size_t pos = 0;
  while ((pos = string.find(delimiter)) != std::string::npos) {
    tokens.emplace_back(string.substr(0, pos));
    string.erase(0, pos + delimiter.length());
  }
  tokens.emplace_back(string);

  return tokens;
}

/**
 * \brief Splits a string at the last occurence of delimiter
 * \return A pair of strings.
 */
std::array<std::string, 2> split_last(std::string string, const std::string& delimiter) {
  size_t pos = string.rfind(delimiter);

  std::string first;

  if (pos != std::string::npos) {
    first = string.substr(0, pos);
    string.erase(0, pos + delimiter.length());
  }

  return {first, string};
}

void replace_all(std::string& str, const std::string& from, const std::string& to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

std::filesystem::path build_directories(std::string directories) {
  using namespace std::filesystem;

  path final_path(current_path());
  final_path.append(directories);

  create_directories(final_path);

  return final_path;
}

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
