//
// Created by igarashi on 5/23/2017.
//

#ifndef PVXMATCHING_STRING_HPP
#define PVXMATCHING_STRING_HPP

#include <cstdio>
#include <string>
#include <stdexcept>

#ifdef __MINGW32__
#define vxx_printf _snprintf
#else
#define vxx_printf std::snprintf
#endif

namespace utils {
namespace string {

namespace detail {

template<typename T>
T to_printable(T arg) {
  return arg;
}

template<typename CharT>
const CharT *to_printable(const std::basic_string<CharT> &arg) {
  return arg.c_str();
}

}

template<typename... Args>
std::string strprintf(const std::string &format, const Args &... args) {
  std::string str;
  str.resize(1024);

  auto size = vxx_printf(&str[0], str.size(), format.c_str(), detail::to_printable(args)...);
  if (size < 0)
    throw std::invalid_argument("Invalid format");

  if (size > str.size()) {
    str.resize(size + 1);
    size = vxx_printf(&str[0], str.size(), format.c_str(), detail::to_printable(args)...);
  }

  str.resize(size);
  return str;
}

}
}

#endif //PVXMATCHING_STRING_HPP
