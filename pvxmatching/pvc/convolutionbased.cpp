#include "convolutionbased.hpp"
#include "../utils/alphabet.hpp"
#include <set>

namespace pvc
{
	std::vector<int> ConvolutionBased::get_matches(const std::vector<int>& text, const std::vector<int>& pattern) const
	{
		const int M = pattern.size() - 1;

		auto pi_occ = std::map<int, int>();
		auto sigma_occ = std::map<int, int>();

		for (int i = 0; i < pattern.size(); i++)
		{
			std::map<int, int>* target;
			if (utils::alphabet::is_constant(pattern[i]))
			{
				target = &sigma_occ;
			}
			else
			{
				target = &pi_occ;
			}

			auto result = target->insert({ pattern[i], 1 });
			if (!result.second)
			{
				result.first->second++;
			}
		}

		auto text_powed = utils::alphabet::pow_elements(text);

		auto pi_lhs = std::vector < std::vector<unsigned long long int >>(pi_occ.size());
		auto pi_rhs = std::vector<std::vector<unsigned long long int>>(pi_occ.size());

		auto sigma_lhs = std::vector<std::vector<unsigned long long int>>(sigma_occ.size());
		auto sigma_rhs = std::vector<std::vector<unsigned long long int>>(sigma_occ.size());

		// for checking function match
		int i = 0;
		for (auto s : pi_occ)
		{
			auto pattern_s = utils::alphabet::x_transform(pattern, s.first);
			pi_lhs[i] = this->_convolution->calculate(text_powed, pattern_s);
			pi_rhs[i++] = this->_convolution->calculate(text, pattern_s);
		}

		// for checking wildcard match
		i = 0;
		for (auto s : sigma_occ)
		{
			auto pattern_s = utils::alphabet::x_transform(pattern, s.first);
			sigma_lhs[i] = this->_convolution->calculate(text_powed, pattern_s);
			sigma_rhs[i++] = this->_convolution->calculate(text, pattern_s);
		}

		auto conv_length = int(text.size()) - pattern.size() + 1;
		auto result = std::vector<int>();

		for (int i = 0; i < conv_length; i++)
		{
			// Check function match
			bool failed = false;
			std::set<int> used;
			int j = 0;
			for (auto s : pi_occ)
			{
				if ((s.second * pi_lhs[j][i]) != pow(pi_rhs[j][i], 2))
				{
					failed = true;
					break;
				}
				auto variable_use = used.insert(pi_rhs[j][i] / s.second);
				if (!variable_use.second)
				{
					// variable already used
					failed = true;
					break;
				}
				j++;
			}

			// Check wildcard match
			if (!failed)
			{
				j = 0;
				for (auto s : sigma_occ)
				{
					auto symbol = sigma_rhs[j][i] / s.second;
					if (s.second * sigma_lhs[j][i] != pow(sigma_rhs[j][i], 2) ||
						(sigma_rhs[j][i] / s.second) != s.first)
					{
						failed = true;
						break;
					}
					j++;
				}

				if (!failed)
				{
					result.push_back(i + M);
				}
			}
		}

		return result;
	}
}
