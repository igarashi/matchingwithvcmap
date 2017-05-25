//
// Created by igarashi on 5/23/2017.
//

#ifndef PVXMATCHING_BITOP_HPP
#define PVXMATCHING_BITOP_HPP

#include <math.h>

namespace utils {
    namespace bitop {
        inline int rightmost_one_at(unsigned long n)
        {
            //if (n == 0)
            //	return -1;
            return log2l(n & -n);
        }
    }
}
#endif //PVXMATCHING_BITOP_HPP
