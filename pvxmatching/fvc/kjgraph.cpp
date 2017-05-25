//
// Created by Yuki Igarashi on 2017/05/23.
//

#include "kjgraph.hpp"

namespace fvc {
    bool KjGraph::addVCCondition(int var, int constant) {
        auto exists = condition_VC.insert({var, constant});
        if (!exists.second) {
            return exists.first->second == constant;
        }
        return true;
    }

    bool KjGraph::addVCConditionRecursive(int var, int constant) {
        auto rootSymbol = var;
        auto varRoot = inversed_VV.find(var);
        if (varRoot != inversed_VV.end())
            rootSymbol = varRoot->second;

        auto child = condition_VV.find(rootSymbol);
        if (child != condition_VV.end()) {
            for (auto childSymbol: child->second) {
                if (!addVCCondition(childSymbol, constant))
                    return false;
                inversed_VV.erase(childSymbol);
            }
        }

        condition_VV.erase(rootSymbol);
        return addVCCondition(rootSymbol, constant);
    }

    bool KjGraph::addCondition(int a, int b) {

        if (utils::alphabet::is_variable(a)) {
            if (utils::alphabet::is_variable(b)) {
                // join bRoot to children of aRoot
				if (a == b)
					return true;

                auto bRootSymbol = b;
                auto bRoot = inversed_VV.find(b);
                if (bRoot != inversed_VV.end())
                    bRootSymbol = bRoot->second;

                auto aRootSymbol = a;
                auto aRoot = inversed_VV.find(a);
                if (aRoot != inversed_VV.end())
                    aRootSymbol = aRoot->second;

				if (aRootSymbol == bRootSymbol)
					return true;

                auto nextRoot = condition_VV.insert({aRootSymbol, std::set<int>()});

                auto bChild = condition_VV.find(bRootSymbol);
                if (bChild != condition_VV.end()) {
                    for (auto bChildSymbol: bChild->second) {
                        nextRoot.first->second.insert(bChildSymbol);
                        inversed_VV[bChildSymbol] = aRootSymbol;
                    }
                    condition_VV.erase(bChild);
                }
                inversed_VV[bRootSymbol] = aRootSymbol;
                nextRoot.first->second.insert(bRootSymbol);

                return true;
            } else {
                // a: var, b: const
                return addVCConditionRecursive(a, b);
            }
        } else {
            if (utils::alphabet::is_variable(b)) {
                // a: const, b: var
                return addVCConditionRecursive(b, a);
            } else {
                // a: const, b: const
                return (a == b);
            }
        }
    }

    const std::map<int, int>* KjGraph::getConditionVC() const {
        return &condition_VC;
    }

    const std::map<int, std::set<int>>* KjGraph::getConditionVV() const {
        return &condition_VV;
    }

}


