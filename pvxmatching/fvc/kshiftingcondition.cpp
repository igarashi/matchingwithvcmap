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
            for (auto &vc_cond: *graphs[graph_count_ - i - 1].getConditionVC()) {
                auto item = vc_bitmap_.insert({vc_cond.first, utils::RBitmap(graph_count_)});
                item.first->second.setValue(vc_cond.second, i);
            }
            
            for (auto &vv_cond: *graphs[graph_count_ - i - 1].getConditionVV()) {
                for (auto vv_cond_s: vv_cond.second) {
                    vv_bitmap_.setValue(utils::pair::make_ordered_pair(vv_cond.first, vv_cond_s), i);
                }
            }
            
            subsequent_[i] = graphs[graph_count_ - i - 1].subsequent;
            widths_[i] = graphs[graph_count_ - i - 1].width;
        }
        
        for (auto &bitmap: vc_bitmap_)
            bitmap.second.compile();
        
        vv_bitmap_.compile();
    }
    
    int KShiftingCondition::calculateLongestShifting(std::map<int, int> &bounds) const {
        auto mask = default_mask_;
        for (auto &vc: bounds) {
            auto exists = vc_bitmap_.find(vc.first);
            if (exists != vc_bitmap_.end())
                mask &= exists->second.getFilter(vc.second);
        }
        
        for (auto &vv: *vv_bitmap_.getBitmap()) {
            if (bounds[vv.first.first] != bounds[vv.first.second]) {
                mask &= vv.second;
            }
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
