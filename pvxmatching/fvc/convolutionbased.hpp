#ifndef PVXMATCHING_FVC_CONVOLUTION_HPP
#define PVXMATCHING_FVC_CONVOLUTION_HPP
#include <vector>
#include <map>

namespace fvc
{

	class ConvolutionBased
	{
	public:

		ConvolutionBased();

		std::vector<int> get_matches(int current, const std::vector<int>& pattern, const std::vector<int>& text) const;

	};

}

#endif