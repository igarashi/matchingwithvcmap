//
// Created by Yuki Igarashi on 2017/05/23.
//

#ifndef PVXMATCHING_VALUEBITMAP_HPP
#define PVXMATCHING_VALUEBITMAP_HPP

#include <map>
#include <bitset>

#define MAX_BIT_SIZE 63

namespace utils {

class RBitmap {
  typedef unsigned long Bitmap;

public:
  RBitmap(int size);

  Bitmap get_filter(int value) const;

  void set_value(int symbol, size_t caseId);

  void compile();

private:
  std::map<int, Bitmap> bitmap_;
  Bitmap default_mask_;

  std::map<int, std::bitset<MAX_BIT_SIZE>> temp_bitmap_;
  std::bitset<MAX_BIT_SIZE> temp_default_bitmap_;

  int size_;
};

}


#endif //PVXMATCHING_VALUEBITMAP_HPP
