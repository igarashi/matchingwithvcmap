//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_TBITMAP_HPP
#define PVXMATCHING_TBITMAP_HPP

#include <map>
#include <bitset>

#define MAX_BIT_SIZE 63

namespace utils {
    
    class TBitmap {
        typedef unsigned long Bitmap;
    
    public:
        TBitmap(int size);
        
        Bitmap getFilter(int value) const;
        
        void setValue(int symbol, size_t caseId);
        
        void setNegativeValue(int symbol, size_t caseId);
        
        void compile();
    
    private:
        std::map<int, Bitmap> bitmap_;
        Bitmap default_mask_;
        
        std::map<int, std::bitset<MAX_BIT_SIZE>> temp_bitmap_;
        std::map<int, std::bitset<MAX_BIT_SIZE>> temp_neg_bitmap_;
        
        std::bitset<MAX_BIT_SIZE> temp_default_bitmap_;
        
        int size_;
    };
    
}




#endif //PVXMATCHING_TBITMAP_HPP
