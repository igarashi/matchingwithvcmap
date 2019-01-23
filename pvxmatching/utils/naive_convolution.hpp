#ifndef PVXMATCHING_NAIVE_CONVOLUTION_HPP
#define PVXMATCHING_NAIVE_CONVOLUTION_HPP

#include <vector>
#include "convolution_base.hpp"

namespace utils
{
	class NaiveConvolution : public ConvolutionBase
	{

	public:

		virtual std::vector<int> calculate(const std::vector<int>& a, const std::vector<int>& b) const
		{
			const auto conv_length = int(a.size()) - b.size() + 1;
			auto result = std::vector<int>(conv_length);

			const auto b_size = b.size();

			for (int i = 0; i < conv_length; i++)
			{
				long int temp = 0;
				for (int j = 0; j < b_size; j++)
				{
					temp += a[i + j] * b[j];
				}
				result[i] = temp;
			}

			return result;
		}
	};

}


#endif