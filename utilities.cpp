#include "utilities.hh"

#include <iostream>

bool utility::build_directories(std::filesystem::path directories) {
  using namespace std::filesystem;

  path final_path{current_path() / directories};

  bool created = create_directories(final_path);

  return created;
}

void utility::replace_all_inplace(std::string&       str,
                                  const std::string& from,
                                  const std::string& to) {
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

std::string utility::replace_all(const std::string& str,
                                 const std::string& from,
                                 const std::string& to) {
  std::string string = str;
  replace_all_inplace(string, from, to);
  return string;
}

std::tuple<std::string, std::string> utility::split_last(std::string        string,
                                                         const std::string& delimiter) {
  size_t pos = string.rfind(delimiter);

  std::string first;

  if (pos != std::string::npos) {
    first = string.substr(0, pos);
    string.erase(0, pos + delimiter.length());
  }

  return {first, string};
}

std::vector<std::string> utility::split(std::string        string,
                                        const std::string& delimiter) {
  std::vector<std::string> tokens;

  size_t pos = 0;
  while ((pos = string.find(delimiter)) != std::string::npos) {
    tokens.emplace_back(string.substr(0, pos));
    string.erase(0, pos + delimiter.length());
  }
  tokens.emplace_back(string);

  return tokens;
}
