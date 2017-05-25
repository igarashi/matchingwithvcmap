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

        bool addCondition(int a, int b);

        const std::map<int, int>* getConditionVC() const;

        const std::map<int, std::set<int>>* getConditionVV() const;

    private:

        std::map<int, int> condition_VC;

        std::map<int, std::set<int>> condition_VV;
        std::map<int, int> inversed_VV;

        bool addVCConditionRecursive(int var, int constant);

        bool addVCCondition(int var, int constant);

    };


};



#endif //PVXMATCHING_FVC_KJGRAPH_HPP
