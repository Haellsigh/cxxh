#pragma once

#include <boost/program_options.hpp>

namespace cxxh::Creators::Header {

bool create(boost::program_options::variables_map& vm, const std::string& content = "");

}  // namespace cxxh::Creators::Header
