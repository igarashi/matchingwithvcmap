#include "stdafx.h"
#include "CppUnitTest.h"

#include "../pvxmatching/naive/pvcmatching.hpp"
#include "../pvxmatching/utils/alphabet.hpp"
#include "../pvxmatching/pvc/convolutionbased.hpp"
#include "../pvxmatching/utils/string.hpp"
#include "../pvxmatching/utils/naive_convolution.hpp"
#include <set>
#include "../pvxmatching/utils/fft_convolution.hpp"


#define N_TEST_CASE 1000

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pvcmacthing_test {
	TEST_CLASS(convolution_test) {

		static void test_impl(int n_case, int text_length, int pattern_length,
			int text_alphabet_size, int pattern_alphabet_size, int variable_ratio, int conv_method) {
			try {
				int count = 0;

				for (auto c = 0; c < n_case; c++) {
					auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
					auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size, variable_ratio);

					auto text_int = utils::alphabet::text_variable_int_reduction(text);
					auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

					std::shared_ptr<utils::ConvolutionBase> conv;
					switch (conv_method)
					{
					case 0:
						conv = std::make_shared<utils::NaiveConvolution>();
						break;
					case 1:
						conv = std::make_shared<utils::FFTConvolution>(true);
						break;
					case 2:
						conv = std::make_shared<utils::FFTConvolution>(true);
						break;
					default:
						Assert::Fail(L"Invalid arg");
					}

					auto pvc_conv = pvc::ConvolutionBased(conv);

					auto conv_matches = std::set<int>();
					for (auto p : pvc_conv.get_matches(text_int, pattern_int))
						conv_matches.insert(p);

					auto naive_matches = std::set<int>();

					for (int i = 0; i < text.length(); i++) {
						int start_index = i - pattern.length() + 1;
						if (start_index < 0)
							continue;

						auto naive_match = naive::pvc_matching::match(text_int, pattern_int, start_index);
						if (naive_match) {
							naive_matches.insert(i);
							count++;
						}
					}

					if (conv_matches != naive_matches)
					{
						Logger::WriteMessage(text.c_str());
						Logger::WriteMessage(pattern.c_str());
						Assert::Fail(L"Naive doesn't match Convolution Based");
					}
				}
				Logger::WriteMessage(utils::string::strprintf("ConvolutionBased PVC: %d matches found.", count).c_str());
			}
			catch (std::exception e) {
				Assert::Fail(L"Raised Exception");
			}
		}

public:

	TEST_METHOD(pvc_convolution) {
		test_impl(N_TEST_CASE, 1000, 5, 13, 3, 1, 0);
		test_impl(N_TEST_CASE, 1000, 5, 13, 3, 1, 1);
		test_impl(N_TEST_CASE, 1000, 5, 13, 3, 1, 2);
	}

	TEST_METHOD(pvc_convolution_manyvariable) {
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 5, 0);
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 5, 1);
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 5, 2);
	}

	TEST_METHOD(pvc_convolution_longpattern) {
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 1, 0);
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 1, 1);
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 1, 2);
	}
	};
}