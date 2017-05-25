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
            for (auto &vc_cond: *graphs[graph_count_ - i - 1].getConditionVC()) {
                auto item = vc_bitmap_.insert({vc_cond.first, utils::TBitmap(graph_count_)});
                item.first->second.setValue(vc_cond.second, i);
            }
            
            for (auto &const_used: graphs[graph_count_ - i - 1].variable_used) {
                for (auto &sigma_used: graphs[graph_count_ - i - 1].constant_used) {
                    auto item = vc_bitmap_.insert({const_used.first, utils::TBitmap(graph_count_)});
                    item.first->second.setNegativeValue(sigma_used.first, i);
                }
            }
            
            subsequent_[i] = graphs[graph_count_ - i - 1].subsequent;
            widths_[i] = graphs[graph_count_ - i - 1].width;
        }
        
        for (auto &bitmap: vc_bitmap_)
            bitmap.second.compile();
    }
    
    int KShiftingCondition::calculateLongestShifting(utils::injective_map::InjectiveMap &bounds) const {
        auto mask = default_mask_;
        for (auto &vc: bounds) {
            auto exists = vc_bitmap_.find(vc.first);
            if (exists != vc_bitmap_.end())
                mask &= exists->second.getFilter(vc.second);
        }
        
        return utils::bitop::rightmost_one_at(mask);
    }
    
    const std::map<int, int> *KShiftingCondition::getSubsequent(int index) const {
        return &subsequent_[index];
    }
    
    int KShiftingCondition::getWidth(int index) const {
        return widths_[index];
    }
}
