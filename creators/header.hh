#pragma once

#include <boost/program_options.hpp>

namespace cxxh::Creators::Header
{

bool create_from_identifiers(boost::program_options::variables_map &vm, std::vector<std::string> const &identifiers,
                             std::string const &contents = "");
bool create(boost::program_options::variables_map &vm, const std::string &contents = "");

} // namespace cxxh::Creators::Header
