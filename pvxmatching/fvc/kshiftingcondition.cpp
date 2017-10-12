//
// Created by Yuki Igarashi on 2017/05/23.
//

#include "kshiftingcondition.hpp"

namespace fvc {

KShiftingCondition::KShiftingCondition(std::vector<KjGraph> graphs) : graph_count_(graphs.size()),
                                                                      vv_bitmap_(graphs.size()),
                                                                      subsequent_(graphs.size()),
                                                                      default_mask_((1 << graphs.size()) - 1),
                                                                      widths_(graphs.size()) {
  // the rightmost bit corresponds to the longest shifting. (We have to reverse here.)
  for (size_t i = 0; i < graph_count_; i++) {
    auto sigma_ref = graphs[graph_count_ - i - 1].get_subsequent();

    for (auto& cond_pi : *graphs[graph_count_ - i - 1].get_condition()) {
      auto parent = graphs[graph_count_ - i - 1].find_parent(cond_pi.first);
      auto sigma_parent = sigma_ref->find(parent);
      if (sigma_parent != sigma_ref->end()) {
        // vc_cond.first is mapped to sigma. (V->C)
        auto item = vc_bitmap_.insert({cond_pi.first, utils::RBitmap(graph_count_)});
        item.first->second.set_value(sigma_parent->second, i);
      } else {
        if (parent == cond_pi.first) // parent
          continue;

        // vc_cond.first is not mapped to sigma. (V->V)
        vv_bitmap_.set_value(utils::pair::make_ordered_pair(parent, cond_pi.first), i);
      }
    }

    subsequent_[i] = graphs[graph_count_ - i - 1].subsequent;
    widths_[i] = graphs[graph_count_ - i - 1].width;
  }

  for (auto& bitmap : vc_bitmap_)
    bitmap.second.compile();

  vv_bitmap_.compile();
}

int KShiftingCondition::calculate_longest_shifting(std::map<int, int>& bounds) const {
  auto mask = default_mask_;
  for (auto& vc : bounds) {
    auto exists = vc_bitmap_.find(vc.first);
    if (exists != vc_bitmap_.end())
      mask &= exists->second.get_filter(vc.second);
  }

  for (auto& vv : *vv_bitmap_.get_bitmap()) {
    if (bounds[vv.first.first] != bounds[vv.first.second]) {
      mask &= vv.second;
    }
  }

  return utils::bitop::rightmost_one_at(mask);
}

const std::map<int, int> *KShiftingCondition::get_subsequent(int index) const {
  return &subsequent_[index];
}

int KShiftingCondition::get_width(int index) const {
  return widths_[index];
}
}
