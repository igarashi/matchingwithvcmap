//
// Created by igarashi on 5/23/2017.
//

#include "sbitmap.hpp"

namespace utils {

SBitmap::SBitmap(int size)
  : size_(size), default_mask_(Bitmap((1 << size) - 1)) {
  if (size > MAX_BIT_SIZE)
    throw "Exception: size exceed MAX_BIT_SIZE while creating bitmap. (Use long as Bitmap instead.)";
}

void SBitmap::set_value(std::pair<int, int> symbol, size_t caseId) {
  temp_bitmap_[symbol].set(caseId);
}

void SBitmap::compile() {
  for (auto symbol : temp_bitmap_) {
    bitmap_[symbol.first] = default_mask_ ^ symbol.second.to_ulong();
  }
}


const std::map<std::pair<int, int>, SBitmap::Bitmap> *SBitmap::get_bitmap() const {
  return &bitmap_;
}
}
