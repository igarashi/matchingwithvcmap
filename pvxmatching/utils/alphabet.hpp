//
// Created by Yuki Igarashi on 2017/05/23.
//

#ifndef PVXMATCHING_ALPHABET_HPP
#define PVXMATCHING_ALPHABET_HPP

#include <vector>
#include <map>

#include <string>
#include <random>
#include <sstream>

namespace utils {
namespace alphabet {

inline bool is_variable(const char& c) {
  return ('A' <= c && c <= 'Z');
}

inline bool is_constant(const char& c) {
  return ('a' <= c && c <= 'z');
}

inline bool is_variable(const int& c) {
  return (c < 0);
}

inline bool is_constant(const int& c) {
  return (c > 0);
}

inline std::string generate_test_constant_string(int length, int alphabetSize) {
  std::random_device rnd;
  std::stringstream ss;
  for (auto n = 0; n < length; n++) {
    auto value = rnd() % alphabetSize;
    ss << char('a' + value);
  }
  return ss.str();
}

inline std::string generate_test_variable_string(int length, int alphabetSize) {
  std::random_device rnd;
  std::stringstream ss;
  for (auto n = 0; n < length; n++) {
    auto value = rnd() % alphabetSize;
    ss << char('A' + value);
  }
  return ss.str();
}

inline std::string generate_test_string(int length, int alphabetSize,
                                        unsigned int variableRatio) {
  std::random_device rnd;
  std::stringstream ss;
  variableRatio++;
  for (auto n = 0; n < length; n++) {
    auto is_variable = (rnd() % variableRatio) != 0;
    auto value = rnd() % alphabetSize;
    if (is_variable)
      ss << char('A' + value);
    else
      ss << char('a' + value);
  }
  return ss.str();
}

inline std::vector<bool> x_transform(const std::vector<int>& text, const int& p) {
  std::vector<bool> result(text.size());
  for (auto i = 0; i < text.size(); i++)
    result[i] = (text[i] == p);
  return result;
}

inline std::vector<std::vector<bool>> binary_alphabet_reduction(const std::vector<int>& text) {
  // must be aligned to A = 1
  // log_2(26) < 5
  std::vector<std::vector<bool>> result(5, std::vector<bool>(text.size()));
  for (int n = 0; n < text.size(); n++) {
    int value = text[n];
    for (int i = 0; i < 5; i++) {
      result[i][n] = (value & 1);
      value >>= 1;
    }
  }
  return result;
}

inline std::vector<int>
text_variable_int_reduction(const std::string& str, const char text_base = ('a' - 1), const char var_base = ('A' - 1)) {
  std::vector<int> result(str.length());
  for (int n = 0; n < str.length(); n++) {
    if (is_variable(str[n])) {
      result[n] = (str[n] - var_base) * -1;
    } else {
      result[n] = (str[n] - text_base);
    }
  }
  return result;
}

inline std::vector<int> text_int_reduction(const std::vector<int>& text) {
  // must be aligned to A = 1
  // log_2(26) < 5
  std::vector<int> result(text.size());

  for (int n = 0; n < text.size(); n++) {
    result[n] = text[n];
  }

  return result;
}

inline std::vector<int> aligned_int_reduction(const std::string& text, int origin = 1) {
  std::map<int, int> temp;

  std::vector<int> result(text.length());

  for (int n = 0; n < text.length(); n++) {
    auto res = temp.find(text[n]);
    if (res == temp.end()) {
      temp.insert({text[n], origin});
      result[n] = origin;
      origin++;
    } else {
      result[n] = res->second;
    }
  }

  return result;
}


}
}

#endif //PVXMATCHING_ALPHABET_HPP
