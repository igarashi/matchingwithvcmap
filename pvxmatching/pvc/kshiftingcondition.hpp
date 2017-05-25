//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_PVC_KSHIFTINGCONDITION_HPP
#define PVXMATCHING_PVC_KSHIFTINGCONDITION_HPP

#include <map>
#include <vector>
#include "../utils/tbitmap.hpp"
#include "kjgraph.hpp"
#include "../utils/bitop.hpp"
#include "../utils/injective_map.hpp"

namespace pvc {
    class KShiftingCondition {
    public:
        KShiftingCondition(std::vector <KjGraph> graphs);
        
        int calculateLongestShifting(utils::injective_map::InjectiveMap &bounds) const;
        
        const std::map<int, int> *getSubsequent(int index) const;
        
        int getWidth(int index) const;
    
    private:
        
        int graph_count_;
        
        std::vector <std::map<int, int>> subsequent_;
        
        std::vector<int> widths_;
        
        std::map<int, utils::TBitmap> vc_bitmap_;
        
        unsigned long default_mask_;
        
    };
}

#endif //PVXMATCHING_PVC_KSHIFTINGCONDITION_HPP
