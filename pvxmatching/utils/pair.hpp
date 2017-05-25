//
// Created by igarashi on 5/23/2017.
//

#ifndef PVXMATCHING_PAIR_HPP
#define PVXMATCHING_PAIR_HPP

#include <utility>

namespace utils {
    namespace pair {
        
        template <typename T>
        inline std::pair<T, T> make_ordered_pair(const T &val1, const T &val2) {
            if (val1 > val2) {
                return { val2, val1 };
            } else {
                return { val1, val2 };
            }
        };
    }
}

#endif //PVXMATCHING_PAIR_HPP
