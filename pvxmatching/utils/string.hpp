//
// Created by igarashi on 5/23/2017.
//

#ifndef PVXMATCHING_STRING_HPP
#define PVXMATCHING_STRING_HPP

#include <string>
#include <cstdarg>

namespace utils {
namespace string {
inline std::string strvprintf(const char *format, std::va_list arg) {
  std::va_list copy;
  va_copy(copy, arg);

  std::string ret;
  ret.resize(64);
  auto n = static_cast<size_t>(vsnprintf(&ret[0], ret.size(), format, arg));

#ifdef _MSC_VER
  if (n == size_t(-1))
  {
      n = _vscprintf(format, arg) + 1;
#else
  if (n > ret.size()) {
#endif
    ret.resize(n + 1);
    n = static_cast<size_t>(vsnprintf(&ret[0], ret.size(), format, copy));
  }
  ret.resize(n);
  return ret;
}

inline std::string strprintf(const char *format, ...) {
  std::va_list arg;
  va_start(arg, format);
  char *alloc;
  auto ret = strvprintf(format, arg);
  va_end(arg);
  return ret;
}
}
}
#endif //PVXMATCHING_STRING_HPP
