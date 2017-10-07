//
// Created by Yuki Igarashi on 2017/05/23.
//

#ifndef PVXMATCHING_FVC_KJGRAPH_HPP
#define PVXMATCHING_FVC_KJGRAPH_HPP

#include <vector>
#include <map>
#include <set>

#include "../utils/alphabet.hpp"

namespace fvc {

class KjGraph {

public:
  int width;

  std::map<int, int> subsequent;

  bool add_condition(int a, int b);

  const std::map<int, int> *get_condition() const;

  const std::map<int, int> *get_sigma() const;

  // Basic operation of Union find.

  int find_parent(int x);

  bool unite(int a, int b);

  bool set_sigma(int var, int constant);

private:

  std::map<int, int> parent_;
  std::map<int, int> rank_;
  std::map<int, int> sigma_;

};

};


#endif //PVXMATCHING_FVC_KJGRAPH_HPP
