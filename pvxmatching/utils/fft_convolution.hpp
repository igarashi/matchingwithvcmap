#ifndef PVXMATCHING_FFT_CONVOLUTION_HPP
#define PVXMATCHING_FFT_CONVOLUTION_HPP

#include <vector>
#include "convolution_base.hpp"

namespace utils
{
	class FFTConvolution : public ConvolutionBase
	{
	private:
		std::vector<unsigned long long int> calculate_sequential(const std::vector<int>& a, const std::vector<int>& b) const;

		std::vector<unsigned long long int> calculate_partial(const std::vector<int>&a, const std::vector<int>& b) const;

		bool is_sequential_;

	public:
		std::vector<unsigned long long int> calculate(const std::vector<int>& a, const std::vector<int>& b) const override;

		FFTConvolution(bool is_sequential = false) : is_sequential_(is_sequential) { }

	};

}

#endif