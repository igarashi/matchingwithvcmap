//
// Created by Yuki Igarashi on 2017/05/23.
//

#include "kmpbased.hpp"

#include <string>

namespace fvc {

    KmpBased::KmpBased(std::vector<int> pattern):
            pattern_length_(pattern.size()), pattern_(pattern), conditions_() {
        calculateTable();
    }

    void KmpBased::calculateTable() {
        auto borders = std::vector<std::vector<KjGraph>>(pattern_length_ + 1);
        borders[0].push_back(KjGraph());
        conditions_.push_back(KShiftingCondition(borders[0]));
        borders[1].push_back(KjGraph());
        conditions_.push_back(KShiftingCondition(borders[1]));

        for (int step = 2; step < pattern_length_ + 1; step++) {
            // always true
            borders[step].push_back(KjGraph());

            for (auto &prev : borders[step - 1]) {
                auto current = prev; // make copy

                int original_pos = step - 1; // P[k]
                int subseq_pos = current.width; // P[j]

                int original_char = pattern_[original_pos];
                int subseq_char = pattern_[subseq_pos];

                if (utils::alphabet::is_variable(subseq_char)) {
                    // subsequent V<-V/C
                    auto exists = current.subsequent.insert({subseq_char, original_char});
                    if (!exists.second) {
                        if (!current.addCondition(exists.first->second, original_char)) {
                            continue;
                        }
                        if (utils::alphabet::is_variable(exists.first->second)) {
                            if (utils::alphabet::is_constant(original_char)) {
                                exists.first->second = original_char;
                            }
                        }
                    }
                } else {
                    // condition C->V/C
                    if (!current.addCondition(subseq_char, original_char)) {
                        continue;
                    }
                }
    
                current.width++;
                borders[step].push_back(current);
            }

            // create k-shifting condition
            conditions_.push_back(KShiftingCondition(borders[step]));
        }
    }
    
    void KmpBased::calculateNextBounds(int position, int index, std::map<int, int> &bounds) const {
        std::map<int, int> before(bounds);
        bounds.clear();

        for (auto& symbol: *conditions_[position].getSubsequent(index)) {
            if (utils::alphabet::is_variable(symbol.second)) {
                bounds[symbol.first] = before[symbol.second];
            } else {
                bounds[symbol.first] = symbol.second;
            }
        }
    }
    
    bool KmpBased::match(int current, int &pos, std::map<int, int> &bounds) const {
        bool match = false;
        
        while (true) {
            bool mismatch = false;

            if (pos < pattern_length_) {
                if (utils::alphabet::is_variable(pattern_[pos])) {
                    auto exists = bounds.insert({pattern_[pos], current});
                    if (!exists.second) {
                        // already exists
                        if (exists.first->second != current) {
                            mismatch = true;
                        }
                    }
                } else {
                    // pattern_[pos] \in \Sigma
                    if (pattern_[pos] != current) {
                        mismatch = true;
                    }
                }
            }
            
            if (!mismatch)
                pos++;
            
            if (pos == pattern_length_)
                match = true;

            if (mismatch || match) {
                if (pos == 0) // we can't move more.
                    return match;

                // calculate next position
                auto rightmost = conditions_[pos].calculateLongestShifting(bounds);
                // replace with subsequent
                calculateNextBounds(pos, rightmost, bounds);
                pos = conditions_[pos].getWidth(rightmost);
                
                if (match)
                    return match;
            } else {
                return match;
            }
        }
    }
}
