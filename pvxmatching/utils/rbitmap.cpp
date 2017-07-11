//
// Created by Yuki Igarashi on 2017/05/23.
//

#include "rbitmap.hpp"

namespace utils {
	RBitmap::RBitmap(int size) : size_(size) {
		if (size > MAX_BIT_SIZE)
			throw "Exception: size exceed MAX_BIT_SIZE while creating bitmap. (Use long as Bitmap instead.)";
	}

	void RBitmap::set_value(int symbol, size_t caseId) {
		temp_bitmap_[symbol].set(caseId);
		temp_default_bitmap_.set(caseId);
	}

	void RBitmap::compile() {
		auto default_mask = temp_default_bitmap_;
		default_mask ^= ((1 << size_) - 1);
		default_mask_ = default_mask.to_ulong();

		for (auto symbol : temp_bitmap_) {
			bitmap_[symbol.first] = default_mask_ | symbol.second.to_ulong();
		}
	}

	RBitmap::Bitmap RBitmap::get_filter(int value) const {
		auto exists = bitmap_.find(value);
		if (exists != bitmap_.end())
			return exists->second;

		return default_mask_;
	}
}

