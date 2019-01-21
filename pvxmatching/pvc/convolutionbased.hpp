#ifndef PVXMATCHING_PVC_CONVOLUTION_HPP
#define PVXMATCHING_PVC_CONVOLUTION_HPP
#include <vector>
#include <map>
#include "../utils/convolution_base.hpp"
#include <memory>

namespace pvc
{

	class ConvolutionBased
	{
	private:

		std::shared_ptr<utils::ConvolutionBase> _convolution;

	public:

		ConvolutionBased(std::shared_ptr<utils::ConvolutionBase> convolutionMethod) : _convolution(convolutionMethod) {};

		std::vector<int> get_matches(const std::vector<int>& text, const std::vector<int>& pattern) const;

	};

}

#endif