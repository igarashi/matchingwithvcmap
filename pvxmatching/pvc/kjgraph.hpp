//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_PVC_KJGRAPH_HPP
#define PVXMATCHING_PVC_KJGRAPH_HPP

#include <vector>
#include <map>
#include <memory>
#include <set>

#include "../utils/alphabet.hpp"

namespace pvc {

class KjGraphPair {
 public:

  int first() {
    return first_;
  }

  int second() {
    return second_;
  }

  bool has_first() {
    return first_set_;
  }

  bool has_second() {
    return second_set_;
  }

  bool set_first(int value) {
    if (first_set_ && first_ != value)
      return false;

    first_ = value;
    first_set_ = true;
    return true;
  }

  bool set_second(int value) {
    if (second_set_ && second_ != value)
      return false;

    second_ = value;
    second_set_ = true;
    return true;
  }

 private:
  bool first_set_ = false;
  bool second_set_ = false;

  int first_;
  int second_;
};

class KjGraph {

 public:
  int width;

  std::map<int, int> get_subsequent() const;

  bool add_condition(int sigma, int symbol);

  bool add_subsequent(int var, int symbol);

  std::map<int, KjGraphPair> sigma_to_pi_piprime;

  std::map<int, KjGraphPair> pi_to_sigma_piprime;

  std::map<int, KjGraphPair> pi_prime_to_sigma_pi;

};

};

#endif //PVXMATCHING_PVC_KJGRAPH_HPP
