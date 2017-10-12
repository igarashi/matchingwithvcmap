//
// Created by igarashi on 5/24/2017.
//

#include "kjgraph.hpp"

namespace pvc {

bool KjGraph::addSubsequent(int var, int symbol) {
  // var(p2) <-> symbol

  std::map<int, int> *target1;
  std::map<int, int> *target2;

  std::map<int, int> *reference;
  bool is_reversed = false;

  if (utils::alphabet::is_variable(symbol)) {
    // if symbol is variable
    // var(p2) <-> var(p)

    target1 = &edge_from_p2_to_p;
    target2 = &edge_from_p_to_p2;

    reference = &edge_from_p2_to_s;
  } else {
    // if symbol is sigma
    // var(p2) <-> symbol(s)

    target1 = &edge_from_p2_to_s;
    target2 = &edge_from_s_to_p2;

    reference = &edge_from_p2_to_p;
    is_reversed = true;
  }

  auto exists = target1->insert({var, symbol});
  if (!exists.second && exists.first->second != symbol) {
    return false; // duplicate
  }

  auto exists2 = target2->insert({symbol, var});
  if (!exists2.second && exists2.first->second != var) {
    return false; // duplicate
  }

  // check if exists condition between p <-> s
  auto exists_c = reference->find(var);
  if (exists_c != reference->end()) {
    if (is_reversed) {
      return addCondition(symbol, exists_c->second);
    } else {
      return addCondition(exists_c->second, symbol);
    }
  }

  return true;
}

bool KjGraph::addCondition(int sigma, int symbol) {

  if (utils::alphabet::is_variable(symbol)) {
    // symbol is a var
    // sigma(s) <-> symbol(p)

    auto exists = edge_from_s_to_p.insert({sigma, symbol});
    if (!exists.second && exists.first->second != symbol) {
      return false; // duplicate
    }

    auto exists2 = edge_from_p_to_s.insert({symbol, sigma});
    if (!exists2.second && exists2.first->second != sigma) {
      return false; // duplicate
    }

    return true;

  } else {
    // symbol is a sigma
    // sigma <-> symbol(sigma)
    return (sigma == symbol);
  }
}

std::map<int, int> KjGraph::get_subsequent() const {
  std::map<int, int> temp;
  for (auto entry:  edge_from_p2_to_p) {
    temp.insert({entry.first, entry.second});
  }

  for (auto entry: edge_from_p2_to_s) {
    auto res = temp.insert({entry.first, entry.second});
    if (!res.second) {
      res.first->second = entry.second; // Update with Sigma value
    }
  }
  return temp;
}

}
