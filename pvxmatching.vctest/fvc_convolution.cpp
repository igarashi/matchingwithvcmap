#include "stdafx.h"
#include "CppUnitTest.h"

#include "../pvxmatching/naive/fvcmatching.hpp"
#include "../pvxmatching/utils/alphabet.hpp"
#include "../pvxmatching/fvc/convolutionbased.hpp"
#include "../pvxmatching/utils/string.hpp"
#include "../pvxmatching/utils/naive_convolution.hpp"
#include <set>

#define N_TEST_CASE 1000

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fvcmacthing_test {
	
	TEST_CLASS(convolution_test) {

		static void test_impl(int n_case, int text_length, int pattern_length,
			int text_alphabet_size, int pattern_alphabet_size, int variable_ratio) {
			try {
				int count = 0;

				for (auto c = 0; c < n_case; c++) {
					auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
					auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size, variable_ratio);

					auto text_int = utils::alphabet::text_variable_int_reduction(text);
					auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

					auto conv = std::make_shared<utils::NaiveConvolution>();
					auto fvc_conv = fvc::ConvolutionBased(conv);

					auto conv_matches = std::set<int>();
					for (auto p : fvc_conv.get_matches(text_int, pattern_int))
						conv_matches.insert(p);

					auto naive_matches = std::set<int>();

					for (int i = 0; i < text.length(); i++) {
						int start_index = i - pattern.length() + 1;
						if (start_index < 0)
							continue;

						auto naive_match = naive::fvc_matching::match(text_int, pattern_int, start_index);
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
				Logger::WriteMessage(utils::string::strprintf("ConvolutionBased FVC: %d matches found.", count).c_str());
			}
			catch (std::exception e) {
				Assert::Fail(L"Raised Exception");
			}
		}

public:

	TEST_METHOD(pvc_convolution) {
		test_impl(N_TEST_CASE, 1000, 5, 13, 3, 1);
	}

	TEST_METHOD(pvc_convolution_manyvariable) {
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 5);
	}

	TEST_METHOD(pvc_convolution_longpattern) {
		test_impl(N_TEST_CASE, 1000, 10, 5, 5, 1);
	}
	};
}