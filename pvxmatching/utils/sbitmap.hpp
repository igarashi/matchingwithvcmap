//
// Created by igarashi on 5/23/2017.
//

#ifndef PVXMATCHING_VV_BITMAP_HPP
#define PVXMATCHING_VV_BITMAP_HPP

#include <map>
#include <bitset>
#include <utility>

#define MAX_BIT_SIZE 63


namespace utils {

class SBitmap {
  typedef unsigned long Bitmap;

public:
  SBitmap(int size);

  const std::map<std::pair<int, int>, SBitmap::Bitmap> *get_bitmap() const;

  void set_value(std::pair<int, int> symbol, size_t caseId);

  void compile();

private:
  std::map<std::pair<int, int>, SBitmap::Bitmap> bitmap_;
  SBitmap::Bitmap default_mask_;

  std::map<std::pair<int, int>, std::bitset<MAX_BIT_SIZE>> temp_bitmap_;

  int size_;

};
}


#endif //PVXMATCHING_VV_BITMAP_HPP
