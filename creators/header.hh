#pragma once

#include <boost/program_options.hpp>

namespace cxxh::Creators {

class Header {
 public:
  static bool create(boost::program_options::variables_map& vm,
                     const std::string&                     content = "");
};

}  // namespace cxxh::Creators
