//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_PVCMATCHING_HPP
#define PVXMATCHING_PVCMATCHING_HPP

#include <string>
#include <map>
#include "../utils/alphabet.hpp"
#include "../utils/injective_map.hpp"

namespace naive {
    namespace pvc_matching {

		bool match(const std::vector<int> text, const std::vector<int> pattern, int i) {
			if (text.size() < (i + pattern.size()))
                return false;
            
            utils::injective_map::InjectiveMap variable;
            for (auto m = 0; m < pattern.size(); m++) {
                if (utils::alphabet::is_variable(pattern[m])) {
                    // insert if not bounded
                    auto exists = variable.insert(pattern[m], text[i + m]);
                    if (exists.second == utils::injective_map::KeyDuplicated ||
                        exists.second == utils::injective_map::ValueDuplicated) {
                        
                        // already exists another key or value
                        return false;
                    }
                } else {
                    if (pattern[m] != text[i + m]) // if pattern is sigma, and not equals to text
                        return false;
                }
            }
            return true;
        }
    }
}

#endif //PVXMATCHING_PVCMATCHING_HPP
