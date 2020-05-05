#pragma once

#include <boost/program_options.hpp>

namespace cxxh::Creators {

class Header {
 public:
  static bool create(boost::program_options::variables_map& vm);
};

}  // namespace cxxh::Creators
