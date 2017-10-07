//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_PVC_KMPBASED_HPP
#define PVXMATCHING_PVC_KMPBASED_HPP

#include <vector>
#include <map>
#include "kjgraph.hpp"
#include "kshiftingcondition.hpp"
#include "../utils/injective_map.hpp"

namespace pvc {

class KmpBased {
private:
  void calculateTable();

  const std::vector<int> pattern_;
  const int pattern_length_;

  std::vector<KShiftingCondition> conditions_;

  void calculateNextBounds(int position, int width, utils::injective_map::InjectiveMap& bounds) const;

public:
  KmpBased(std::vector<int> pattern);

  bool match(int current, int& pos, utils::injective_map::InjectiveMap& bounds) const;
};


}


#endif //PVXMATCHING_PVC_KMPBASED_HPP
