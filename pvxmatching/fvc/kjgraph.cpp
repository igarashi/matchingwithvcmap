//
// Created by Yuki Igarashi on 2017/05/23.
//

#include "kjgraph.hpp"

namespace fvc {

bool KjGraph::set_sigma(int var, int constant) {
  // insert to parent if not exists
  auto parent = find_parent(var);
  auto x = sigma_.insert({parent, constant});
  if (!x.second) {
    return x.first->second == constant;
  }
  return true;
}

int KjGraph::find_parent(int x) {
  auto temp = parent_.insert({x, x});
  if (temp.second) {
    // not inserted yet.
    rank_.insert({x, 0});
    return x;
  } else {
    // already inserted.
    if (temp.first->second == x) {
      return x;
    } else {
      return temp.first->second = find_parent(temp.first->second);
    }
  }
}

bool KjGraph::unite(int a, int b) {
  auto x = find_parent(a);
  auto y = find_parent(b);

  if (x == y) return true;

  auto sigma_x = sigma_.find(x);
  auto sigma_y = sigma_.find(y);
  if (sigma_x != sigma_.end() && sigma_y != sigma_.end() && sigma_x->second != sigma_y->second) {
    return false; // can't merge two connected components which have different sigma.
  }

  bool has_sigma = false;
  int sigma_xy = 0;
  if (sigma_x != sigma_.end()) {
    has_sigma = true;
    sigma_xy = sigma_x->second;
  }
  if (sigma_y != sigma_.end()) {
    has_sigma = true;
    sigma_xy = sigma_y->second;
  }

  if (rank_[x] < rank_[y]) {
    parent_[x] = y;
    if (has_sigma)
      sigma_.insert({y, sigma_xy});

  } else {
    parent_[y] = x;
    if (has_sigma)
      sigma_.insert({x, sigma_xy});

    if (rank_[x] == rank_[y]) rank_[x]++;
  }
  return true;
}

bool KjGraph::add_condition(int a, int b) {

  if (utils::alphabet::is_variable(a)) {
    if (utils::alphabet::is_variable(b)) {
      // join bRoot to children of aRoot
      if (a == b)
        return true;

      return unite(a, b);
    } else {
      // a: var, b: const
      return set_sigma(a, b);
    }
  } else {
    if (utils::alphabet::is_variable(b)) {
      // a: const, b: var
      return set_sigma(b, a);
    } else {
      // a: const, b: const
      return (a == b);
    }
  }
}

const std::map<int, int> *KjGraph::get_condition() const {
  return &parent_;
}

const std::map<int, int> *KjGraph::get_subsequent() const {
  return &sigma_;
}

}


