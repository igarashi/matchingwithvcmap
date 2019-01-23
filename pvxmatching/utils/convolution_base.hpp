#ifndef PVXMATCHING_CONVOLUTIONBASE_HPP
#define PVXMATCHING_CONVOLUTIONBASE_HPP

#include <vector>

namespace utils
{
	class ConvolutionBase
	{

	public:

		virtual std::vector<int> calculate(const std::vector<int>& a, const std::vector<int>& b) const = 0;

	};

}

#endif