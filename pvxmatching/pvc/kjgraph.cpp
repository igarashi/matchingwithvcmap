//
// Created by igarashi on 5/24/2017.
//

#include "kjgraph.hpp"

namespace pvc {

bool KjGraph::add_subsequent(int var, int symbol) {
  // var(p2) <-> symbol

  std::map<int, KjGraphPair> *temp;
  KjGraphPair *target;

  auto exists = pi_prime_to_sigma_pi.insert({var, KjGraphPair()});
  target = &exists.first->second;

  if (utils::alphabet::is_variable(symbol)) {
    // if symbol is variable
    // var(p2) <-> var(p)
    temp = &pi_to_sigma_piprime;

    if (!target->set_second(symbol))
      return false;
  } else {
    // if symbol is sigma
    // var(p2) <-> symbol(s)
    temp = &sigma_to_pi_piprime;

    if (!target->set_first(symbol))
      return false;
  }

  // set a reference to var
  auto exists_temp = temp->insert({symbol, KjGraphPair()});
  if (!exists_temp.first->second.set_second(var))
    return false;

  // now, we have a relationship between exists.first.first <-> exists_temp.first.first
  // the following code create the obvious edge between the symbols
  if (utils::alphabet::is_variable(symbol)) {
    // check whether there exists pi' <-> sigma or pi <-> sigma
    auto has_sigma = target->has_first();
    auto has_sigma2 = exists_temp.first->second.has_first();

    int sigma_symbol;

    auto sigma = target->first();
    auto sigma2 = exists_temp.first->second.first();

    if (has_sigma && has_sigma2) {
      return sigma == sigma2; // check injectively invalid
    } else if (has_sigma) {
      sigma_symbol = sigma;
    } else if (has_sigma2) {
      sigma_symbol = sigma2;
    } else {
      return true; // there are no edge
    }

    // insert sigma edge
    auto sigma_edge = sigma_to_pi_piprime.insert({sigma_symbol, KjGraphPair()});
    if (!target->set_first(sigma_symbol) || !sigma_edge.first->second.set_second(var))
      return false;

    if (!exists_temp.first->second.set_first(sigma_symbol) || !sigma_edge.first->second.set_first(symbol))
      return false;

  } else {
    // check whether there exists pi' <-> pi or sigma' <-> pi
    auto has_pi = target->has_second();
    auto has_pi2 = exists_temp.first->second.has_first();

    int pi_symbol;

    auto pi = target->second();
    auto pi2 = exists_temp.first->second.first();

    if (has_pi && has_pi2) {
      return pi == pi2; // check injectively invalid
    } else if (has_pi) {
      pi_symbol = pi;
    } else if (has_pi2) {
      pi_symbol = pi2;
    } else {
      return true; // there are no edge
    }

    // insert pi edge
    auto pi_edge = pi_to_sigma_piprime.insert({pi_symbol, KjGraphPair()});
    if (!target->set_second(pi_symbol) || !pi_edge.first->second.set_second(var))
      return false;

    if (!exists_temp.first->second.set_first(pi_symbol) || !pi_edge.first->second.set_first(symbol))
      return false;
  }

  return true;
}

bool KjGraph::add_condition(int sigma, int symbol) {

  if (utils::alphabet::is_variable(symbol)) {
    // symbol is a var
    // sigma(s) <-> symbol(p)

    std::map<int, KjGraphPair> *temp;
    KjGraphPair *target;

    auto exists = sigma_to_pi_piprime.insert({sigma, KjGraphPair()});
    target = &exists.first->second;

    if (!target->set_first(symbol))
      return false;

    auto exists_pi = pi_to_sigma_piprime.insert({symbol, KjGraphPair()});
    if (!exists_pi.first->second.set_first(sigma))
      return false;

    auto has_piprime = target->has_second();
    auto has_piprime2 = exists_pi.first->second.has_second();

    int piprime_symbol;

    auto piprime = target->second();
    auto piprime2 = exists_pi.first->second.second();

    if (has_piprime && has_piprime2) {
      return piprime == piprime2;
    } else if (has_piprime) {
      piprime_symbol = piprime;
    } else if (has_piprime2) {
      piprime_symbol = piprime2;
    } else {
      return true;
    }

    // insert piprime edge
    auto piprime_edge = pi_prime_to_sigma_pi.insert({piprime_symbol, KjGraphPair()});
    if (!target->set_second(piprime_symbol) || !piprime_edge.first->second.set_first(sigma))
      return false;

    if (!exists_pi.first->second.set_second(piprime_symbol) || !piprime_edge.first->second.set_second(symbol))
      return false;

    return true;

  } else {
    // symbol is a sigma
    // sigma <-> symbol(sigma)
    return (sigma == symbol);
  }
}

std::map<int, int> KjGraph::get_subsequent() const {
  std::map<int, int> temp;
  for (auto entry: pi_prime_to_sigma_pi) {
    if (entry.second.has_first()) {
      temp.insert({entry.first, entry.second.first()});
    } else {
      temp.insert({entry.first, entry.second.second()});
    }
  }
  return temp;
}

}
