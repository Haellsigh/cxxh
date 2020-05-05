#pragma once

#include <boost/program_options.hpp>

namespace cxxh::Creators {

class Source {
 public:
  static bool create(boost::program_options::variables_map& vm);
};

}  // namespace cxxh::Creators
