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

class KjGraphComponent {
public:

  int pi() {
    return pi_;
  }

  int pi_prime() {
    return pi_prime_;
  }

  int sigma() {
    return sigma_;
  }

  int get_subsequent() {
    if (sigma_set_)
      return sigma_;

    if (!pi_set_)
      throw std::string("Invalid operation.");

    return pi_;
  }

  bool has_sigma() {
    return sigma_set_;
  }

  bool set_pi(int pi) {
    if (pi_set_ && pi_ != pi)
      return false;

    pi_ = pi;
    pi_set_ = true;
    return true;
  }

  bool set_pi_prime(int pi_prime) {
    if (pi_prime_set_ && pi_prime_ != pi_prime)
      return false;

    pi_prime_ = pi_prime;
    pi_prime_set_ = true;
    return true;
  }

  bool set_sigma(int sigma) {
    if (sigma_set_ && sigma_ != sigma)
      return false;

    sigma_ = sigma;
    sigma_set_ = true;
    return true;
  }

  bool merge(std::shared_ptr<KjGraphComponent> rhs) {
    if (pi_set_) {
      if (rhs->pi_set_ && pi_ != rhs->pi_)
        return false;
    } else {
      pi_ = rhs->pi_;
      pi_set_ = rhs->pi_set_;
    }

    if (pi_prime_set_) {
      if (rhs->pi_prime_set_ && pi_prime_ != rhs->pi_prime_)
        return false;
    } else {
      pi_prime_ = rhs->pi_prime_;
      pi_prime_set_ = rhs->pi_prime_set_;
    }

    if (sigma_set_) {
      if (rhs->sigma_set_ && sigma_ != rhs->sigma_)
        return false;
    } else {
      sigma_ = rhs->sigma_;
      sigma_set_ = rhs->sigma_set_;
    }

    return true;
  }

private:
  int pi_;
  bool pi_set_ = false;

  int pi_prime_;
  bool pi_prime_set_ = false;

  int sigma_;
  bool sigma_set_ = false;
};

class KjGraph {

public:
  int width;

  std::map<int, int> get_subsequent() const;

  bool add_condition(int sigma, int symbol);

  bool add_subsequent(int var, int symbol);

  std::map<int, std::shared_ptr<KjGraphComponent>> sigma;

  std::map<int, std::shared_ptr<KjGraphComponent>> pi;

  std::map<int, std::shared_ptr<KjGraphComponent>> pi_prime;

  KjGraph() {}

  // for copy constructor
  KjGraph(const KjGraph& rhs) {
    this->width = rhs.width;
    for (auto symbol: rhs.sigma) {
      this->sigma.insert({symbol.first, std::make_shared<KjGraphComponent>(*symbol.second)});
    }
    for (auto symbol: rhs.pi) {
      this->sigma.insert({symbol.first, std::make_shared<KjGraphComponent>(*symbol.second)});
    }
    for (auto symbol: rhs.pi_prime) {
      this->sigma.insert({symbol.first, std::make_shared<KjGraphComponent>(*symbol.second)});
    }
  }

};

};

#endif //PVXMATCHING_PVC_KJGRAPH_HPP
