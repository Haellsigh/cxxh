#include <creators/utilities.hh>

#include <iostream>

namespace cxxh::Creators::utilities
{

bool contains(const std::string &str, const std::string &seq)
{
    return str.find(seq) != std::string::npos;
}

void replace_all_inplace(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string replace_all(const std::string &str, const std::string &from, const std::string &to)
{
    std::string string = str;
    replace_all_inplace(string, from, to);
    return string;
}

std::tuple<std::string_view, std::string_view> split_last(const std::string_view string,
                                                          const std::string_view delimiter)
{
    size_t pos = string.rfind(delimiter);

    std::string_view first;

    if (pos != std::string::npos)
    {
        return {string.substr(0, pos), string.substr(pos + delimiter.length())};
    }

    return {first, string};
}

std::vector<std::string> split(std::string string, const std::string &delimiter)
{
    std::vector<std::string> tokens;

    size_t pos = 0;
    while ((pos = string.find(delimiter)) != std::string::npos)
    {
        tokens.emplace_back(string.substr(0, pos));
        string.erase(0, pos + delimiter.length());
    }
    tokens.emplace_back(string);

    return tokens;
}

bool file_exists(std::filesystem::path path)
{
    return std::filesystem::status(path).type() != std::filesystem::file_type::not_found;
}

} // namespace cxxh::Creators::utilities
