#ifndef PVXMATCHING_FFT_CONVOLUTION_HPP
#define PVXMATCHING_FFT_CONVOLUTION_HPP
#include <vector>
#include "convolution_base.hpp"

namespace utils
{
	class FFTConvolution: public ConvolutionBase
	{

	public:

		std::vector<int> calculate(const std::vector<int>& a, const std::vector<int>& b) const;

	};

}


#endif