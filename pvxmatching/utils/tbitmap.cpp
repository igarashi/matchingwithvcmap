//
// Created by igarashi on 5/24/2017.
//

#include "tbitmap.hpp"

namespace utils {
TBitmap::TBitmap(int size) : size_(size) {
  if (size > MAX_BIT_SIZE)
    throw "Exception: size exceed MAX_BIT_SIZE while creating bitmap. (Use long as Bitmap instead.)";
}

void TBitmap::set_value(int symbol, size_t caseId) {
  temp_bitmap_[symbol].set(caseId);
  temp_default_bitmap_.set(caseId);
}

void TBitmap::set_negative_value(int symbol, size_t caseId) {
  temp_neg_bitmap_[symbol].set(caseId);
}

void TBitmap::compile() {
  auto mask = ((1 << size_) - 1);

  auto default_mask = temp_default_bitmap_;
  default_mask ^= mask;
  default_mask_ = default_mask.to_ulong();

  auto sigmastrict_it = temp_bitmap_.begin();
  auto bound_it = temp_neg_bitmap_.begin();

  // Note that we supposed that std::map<>::iterator traverse elements in ascending order.
  while (sigmastrict_it != temp_bitmap_.end() || bound_it != temp_neg_bitmap_.end()) {
    if (bound_it == temp_neg_bitmap_.end()) {
      // sigma is stricted, but there is no bounds
      bitmap_[sigmastrict_it->first] = default_mask_ | sigmastrict_it->second.to_ulong();
      sigmastrict_it++;
    } else if (sigmastrict_it == temp_bitmap_.end()) {
      // there is bound, but sigma is not stricted.
      bitmap_[bound_it->first] = default_mask_ & (mask ^ bound_it->second.to_ulong());
      bound_it++;
    } else {
      if (bound_it->first == sigmastrict_it->first) {
        // there is bounds and sigmastrict to same symbol.
        bitmap_[bound_it->first] =
          (default_mask_ & (mask ^ bound_it->second.to_ulong())) | sigmastrict_it->second.to_ulong();
        bound_it++;
        sigmastrict_it++;
      } else if (bound_it->first < sigmastrict_it->first) {
        // there is bound, but sigma is not stricted.
        bitmap_[bound_it->first] = default_mask_ & (mask ^ bound_it->second.to_ulong());
        bound_it++;
      } else {
        // sigma is stricted, but there is no bounds
        bitmap_[sigmastrict_it->first] = default_mask_ | sigmastrict_it->second.to_ulong();
        sigmastrict_it++;
      }
    }
  }
}

TBitmap::Bitmap TBitmap::get_filter(int value) const {
  auto exists = bitmap_.find(value);
  if (exists != bitmap_.end())
    return exists->second;

  return default_mask_;
}

}
