#include "stdafx.h"
#include "CppUnitTest.h"

#include "../pvxmatching/naive/pvcmatching.hpp"
#include "../pvxmatching/utils/alphabet.hpp"
#include "../pvxmatching/pvc/kmpbased.hpp"
#include "../pvxmatching/utils/string.hpp"
#include "../pvxmatching/utils/injective_map.hpp"


#define N_TEST_CASE 1000

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pvcmacthing_test
{
	TEST_CLASS(kmpbased_test)
	{
		static void test_impl(int n_case, int text_length, int pattern_length,
			int text_alphabet_size, int pattern_alphabet_size, int variable_ratio)
		{
			try
			{
				int count = 0;

				for (auto c = 0; c < n_case; c++)
				{
					auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
					auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size, variable_ratio);

					auto text_int = utils::alphabet::text_variable_int_reduction(text);
					auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

					auto fvckmp = pvc::KmpBased(pattern_int);

					auto j = 0;
					utils::injective_map::InjectiveMap bounds;

					for (int i = 0; i < text.length(); i++)
					{
						auto kmp_match = fvckmp.match(text_int[i], j, bounds);
						int start_index = i - pattern.length() + 1;
						if (start_index < 0)
							continue;

						auto text_substr = text.substr(start_index, pattern.length());
						auto naive_match = naive::pvc_matching::match(text_int, pattern_int, start_index);
						Assert::AreEqual(kmp_match, naive_match, L"Not Equals to Naive Result.");
						if (naive_match) count++;
					}
				}
				Logger::WriteMessage(utils::string::strprintf("KmpBasedUssMatching(type=2): %d matches found.", count).c_str());
			}
			catch (std::exception e)
			{
				Assert::Fail(L"Raised Exception");
			}
		}

	public:

		TEST_METHOD(pvc_kmpbased)
		{
			test_impl(N_TEST_CASE, 1000, 5, 13, 3, 1);
		}

		TEST_METHOD(pvc_kmpbased_manyvariable)
		{
			test_impl(N_TEST_CASE, 1000, 10, 5, 5, 5);
		}

		TEST_METHOD(pvc_kmpbased_longpattern)
		{
			test_impl(N_TEST_CASE, 1000, 10, 5, 5, 1);
		}
	};
}