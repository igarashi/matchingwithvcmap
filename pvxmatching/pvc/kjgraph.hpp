//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_PVC_KJGRAPH_HPP
#define PVXMATCHING_PVC_KJGRAPH_HPP


#include <vector>
#include <map>
#include <set>

#include "../utils/alphabet.hpp"

namespace pvc {

class KjGraph {

public:
  int width;

  std::map<int, int> subsequent;

  bool addCondition(int a, int b);

  bool addSubsequent(int var, int symbol);

  const std::map<int, int> *getConditionVC() const;

  std::map<int, int> constant_used;

  std::map<int, int> variable_used;

private:

  std::map<int, int> condition_VC_;

  bool addVCCondition(int var, int constant);

};

};

#endif //PVXMATCHING_PVC_KJGRAPH_HPP
