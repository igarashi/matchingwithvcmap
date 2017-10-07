//
// Created by igarashi on 5/24/2017.
//

#include "kjgraph.hpp"

namespace pvc {

bool KjGraph::addVCCondition(int var, int constant) {
  auto exists = condition_VC_.insert({var, constant});
  if (!exists.second) {
    return exists.first->second == constant;
  }

  auto var_exists = variable_used.find(var);
  if (var_exists != variable_used.end()) {
    // there are edge between \Pi \to \Pi'
    // check if constant is already mapped.
    auto const_exists = constant_used.insert({constant, var_exists->second});
    if (!const_exists.second) {
      if (const_exists.first->second != var_exists->second) {
        // there are distinct nodes x,y \in \Pi'
        //  belong to the same connected component (via \Sigma).
        return false;
      }
    }
  }
  return true;
}

bool KjGraph::addSubsequent(int var, int symbol) {
  if (utils::alphabet::is_variable(symbol)) {
    // if symbol is variable
    auto var_exists = variable_used.insert({symbol, var});
    if (!var_exists.second) {
      if (var_exists.first->second != var) {
        // there are distinct nodes x,y \in \Pi'
        //  belong to the same connected component (via \Pi).
        return false;
      }
    }

    auto cond_exists = condition_VC_.find(symbol);
    if (cond_exists != condition_VC_.end()) {
      auto const_exists = constant_used.insert({cond_exists->second, var});
      if (!const_exists.second) {
        if (const_exists.first->second != var) {
          // there are distinct nodes x,y \in \Pi'
          //  belong to the same connected component (via \Sigma).
          return false;
        }
      }
    }
  } else {
    auto const_exists = constant_used.insert({symbol, var});
    if (!const_exists.second) {
      if (const_exists.first->second != var) {
        // there are distinct nodes x,y \in \Pi'
        //  belong to the same connected component (via \Sigma).
        return false;
      }
    }
  }

  // then, add ordinal subsequent.
  auto exists = subsequent.insert({var, symbol});
  if (!exists.second) {
    if (!addCondition(exists.first->second, symbol)) {
      return false;
    }

    if (utils::alphabet::is_variable(exists.first->second)) {
      if (utils::alphabet::is_constant(symbol)) {
        exists.first->second = symbol;
      }
    }
  }

}

bool KjGraph::addCondition(int a, int b) {

  if (utils::alphabet::is_variable(a)) {
    if (utils::alphabet::is_variable(b)) {
      if (a == b)
        return true;

      // there is no distinct nodes x,y \in \Pi belong to the same connected component.
      // (because pre-function u is injection.)
      return false;
    } else {
      // a: var, b: const
      return addVCCondition(a, b);
    }
  } else {
    if (utils::alphabet::is_variable(b)) {
      // a: const, b: var
      return addVCCondition(b, a);
    } else {
      // a: const, b: const
      return (a == b);
    }
  }
}

const std::map<int, int> *KjGraph::getConditionVC() const {
  return &condition_VC_;
}

}
