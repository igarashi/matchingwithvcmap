//
// Created by igarashi on 5/23/2017.
//

#ifndef PVXMATCHING_NAIVE_FVCMATCHING_HPP
#define PVXMATCHING_NAIVE_FVCMATCHING_HPP

#include <string>
#include <map>
#include "../utils/alphabet.hpp"

namespace naive {
namespace fvc_matching {

inline bool match(const std::vector<int> text, const std::vector<int> pattern, int i) {
  if (text.size() < (i + pattern.size()))
    return false;

  std::map<int, int> variable;
  for (auto m = 0; m < pattern.size(); m++) {
    if (utils::alphabet::is_variable(pattern[m])) {
      // insert if not bounded
      auto result = variable.insert({pattern[m], text[i + m]});
      if (!result.second) // if bounded
      {
        if (result.first->second != text[i + m]) // and not equals to text
          return false;
      }
    } else {
      if (pattern[m] != text[i + m]) // if pattern is sigma, and not equals to text
        return false;
    }
  }
  return true;
}
}
}


#endif //PVXMATCHING_FVCMATCHING_HPP
