//
// Created by igarashi on 5/24/2017.
//

#include "kshiftingcondition.hpp"

namespace pvc {
KShiftingCondition::KShiftingCondition(std::vector<KjGraph> graphs) : graph_count_(graphs.size()),
                                                                      subsequent_(graphs.size()),
                                                                      default_mask_((1 << graphs.size()) - 1),
                                                                      widths_(graphs.size()) {
  // the rightmost bit corresponds to the longest shifting. (We have to reverse here.)
  for (size_t i = 0; i < graph_count_; i++) {
    // Condition for F_{k,j}(x) \cap \Sigma \nsubseteq {p}
    for (auto& vc_cond: graphs[graph_count_ - i - 1].pi) {
      if (!vc_cond.second->has_sigma())
        continue;

      auto item = vc_bitmap_.insert({vc_cond.first, utils::TBitmap(graph_count_)});
      item.first->second.set_value(vc_cond.second->sigma(), i);
    }

    // Condition for (b')
    for (auto& var: graphs[graph_count_ - i - 1].pi) {
      for (auto& sigma_used: graphs[graph_count_ - i - 1].sigma) {
        auto item = vc_bitmap_.insert({var.first, utils::TBitmap(graph_count_)});
        item.first->second.set_negative_value(sigma_used.first, i);
      }
    }

    subsequent_[i] = graphs[graph_count_ - i - 1].get_subsequent();
    widths_[i] = graphs[graph_count_ - i - 1].width;
  }

  for (auto& bitmap: vc_bitmap_)
    bitmap.second.compile();
}

int KShiftingCondition::calculate_longest_shifting(utils::injective_map::InjectiveMap& bounds) const {
  auto mask = default_mask_;
  for (auto& vc: bounds) {
    auto exists = vc_bitmap_.find(vc.first);
    if (exists != vc_bitmap_.end())
      mask &= exists->second.get_filter(vc.second);
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
