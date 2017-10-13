//
// Created by igarashi on 5/24/2017.
//

#include "kjgraph.hpp"

namespace pvc {

bool KjGraph::add_subsequent(int var, int symbol) {
  // var(p2) <-> symbol

  std::map<int, std::shared_ptr<KjGraphComponent>> *temp;
  std::shared_ptr<KjGraphComponent> target;

  auto exists = pi_prime.find(var);
  if (exists != pi_prime.end()) {
    target = exists->second;
  } else {
    target = std::make_shared<KjGraphComponent>();
    target->set_pi_prime(var);

    pi_prime.insert({var, target});
  }

  if (utils::alphabet::is_variable(symbol)) {
    // if symbol is variable
    // var(p2) <-> var(p)
    temp = &pi;

    if (!target->set_pi(symbol))
      return false;
  } else {
    // if symbol is sigma
    // var(p2) <-> symbol(s)
    temp = &sigma;

    if (!target->set_sigma(symbol))
      return false;
  }

  // insert target to another symbol
  auto exists_temp = temp->insert({symbol, target});
  if (exists_temp.first->second != target) {
    // merge
    if (!target->merge(exists_temp.first->second))
      return false;

    exists_temp.first->second = target;
  }

  return true;
}

bool KjGraph::add_condition(int sigma, int symbol) {

  if (utils::alphabet::is_variable(symbol)) {
    // symbol is a var
    // sigma(s) <-> symbol(p)

    std::map<int, std::shared_ptr<KjGraphComponent>> *temp;
    std::shared_ptr<KjGraphComponent> target;

    auto exists = this->sigma.find(sigma);
    if (exists != this->sigma.end()) {
      target = exists->second;
    } else {
      target = std::make_shared<KjGraphComponent>();
      target->set_sigma(sigma);

      this->sigma.insert({sigma, target});
    }

    if (!target->set_pi(symbol))
      return false;

    auto exists_pi = pi.insert({symbol, target});
    if (exists_pi.first->second != target) {
      if (!target->merge(exists_pi.first->second))
        return false;

      exists_pi.first->second = target;
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
  for (auto entry: pi_prime) {
    auto res = temp.insert({entry.first, entry.second->get_subsequent()});
  }
  return temp;
}

}
