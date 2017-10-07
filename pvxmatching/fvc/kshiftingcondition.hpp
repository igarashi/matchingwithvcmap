//
// Created by Yuki Igarashi on 2017/05/23.
//

#ifndef PVXMATCHING_FVC_KSHIFTINGCONDITION_HPP
#define PVXMATCHING_FVC_KSHIFTINGCONDITION_HPP

#include <map>
#include <vector>

#include "kjgraph.hpp"
#include "../utils/rbitmap.hpp"
#include "../utils/sbitmap.hpp"
#include "../utils/pair.hpp"
#include "../utils/bitop.hpp"

namespace fvc {

class KShiftingCondition {
public:
  KShiftingCondition(std::vector<KjGraph> graphs);

  int calculate_longest_shifting(std::map<int, int>& bounds) const;

  const std::map<int, int> *get_subsequent(int index) const;

  int get_width(int index) const;

private:

  int graph_count_;

  std::vector<std::map<int, int>> subsequent_;

  std::vector<int> widths_;

  std::map<int, utils::RBitmap> vc_bitmap_;

  utils::SBitmap vv_bitmap_;

  unsigned long default_mask_;

};

}

#endif //PVXMATCHING_FVC_KSHIFTINGCONDITION_HPP
