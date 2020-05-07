#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace cxxh::Parsers::Identifiers {

bool verify(const std::vector<std::string_view>& identifiers);

/*!
 * \brief Extracts the directory, namespaces and name from an identifier
 */
std::tuple<std::filesystem::path, std::string, std::string> extract(
    const std::string_view identifier);

}  // namespace cxxh::Parsers::Identifiers
