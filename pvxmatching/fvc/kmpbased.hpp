//
// Created by Yuki Igarashi on 2017/05/23.
//

#ifndef PVXMATCHING_FVC_KMPBASED_HPP
#define PVXMATCHING_FVC_KMPBASED_HPP

#include <vector>
#include <map>
#include <string>

#include "../utils/alphabet.hpp"
#include "kjgraph.hpp"
#include "kshiftingcondition.hpp"

namespace fvc {

	class KmpBased {

	private:
		void calculate_table();

		const std::vector<int> pattern_;
		const int pattern_length_;

		std::vector<KShiftingCondition> conditions_;

		void calculate_nextbounds(int position, int width, std::map<int, int> &bounds) const;

	public:

		KmpBased(std::vector<int> pattern);

		bool match(int current, int &pos, std::map<int, int> &bounds) const;

	};

}



#endif //PVXMATCHING_FVC_KMPBASED_HPP
