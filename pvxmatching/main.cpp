#include <iostream>

#include "fvc/kmpbased.hpp"
#include "pvc/kmpbased.hpp"
#include "naive/pvcmatching.hpp"
#include "naive/fvcmatching.hpp"
#include "fvc/convolutionbased.hpp"
#include "utils/naive_convolution.hpp"
#include "pvc/convolutionbased.hpp"

std::string pattern = "AaBbA";
std::string text = "adacacbcdaabddcdaccabccbddbcddcdabccccdcdacccaddabaabbcacaaaabdbcadbbcabaabbcdbcbdddddaccadbbbcccaaaddbcbaaaacbbddccaaaabcabbddddcbdcacdbbcbcadcdccbbcdccbbbddccddddacdddaccdbbaccbbdcbbdccbcbdbabdbdcabacbadbacdadcaababddbdcdbbcadbadaabccaddacccacdbdcddbdabcaccaacccddcbcbbdcabcaddbdbbdbcdbddcdbddbdaaaacbbcccdadadbccdcbbbdcaccccbaaacbccaabccdcdcadaabbaacbdbacdbddcadbbdbcacbabadccabbbadabbbbbbaddbdcbcbaddccdbbbdbabcacbdaadbcaaaadcddacbbcadbaacbbacabdcdacadbccdbddbabdddccbadcccaacddaabccacdbabbbbbdbddcccacccbdcabbdbcadbabdbcbadcdacdaabccbcbacbbaadaabcdbddbcdddaadabdbccbbcacccacabbadcdbcaaaadbadaaabaadaacddbcaadadcaddadcadbbabdbcabbdbabbabcabccbddcadaaaacbdcddaacacdddbdabcddccccddbbadabcaddddabcbbddadbacbbbaaccddcabdabaadbdbcdcabdddcaadabdcabbbaccaaaccdabddacccabbbcbccadaacbbadacbbbdaaaadbaaacbaadabaccbdcacbadbbbbababdabadaaabcccccdbddabacdbacbcbdadccadcbaabcbcbddcbcacdacdcaabaddbbaababbddaccbcadabaaaaddbaabacdbdbbdbdbaacdabbdbcdbdbcadcbbccddcdddacbaabdcbaacaababddabaadbdacabbbbcddcbcbdadaacbcdcbababddababcacbdddca";

void test_fvcmatching()
{
	auto text_int = utils::alphabet::text_variable_int_reduction(text);
	auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

	auto fvckmp = fvc::KmpBased(pattern_int);

	auto conv = std::make_shared<utils::NaiveConvolution>();
	auto fvc_conv = fvc::ConvolutionBased(conv);
	auto conv_pos = std::set<int>();
	for (auto match : fvc_conv.get_matches(text_int, pattern_int))
	{
		conv_pos.insert(match);
	}
	auto naive_pos = std::set<int>();

	int j = 0;
	std::map<int, int> bounds;
	for (int i = 0; i < text.length(); i++) {
		int start_index = i - pattern.length() + 1;

		auto kmp_match = fvckmp.match(text_int[i], j, bounds);

		if (start_index < 0)
			continue;

		auto text_substr = text.substr(start_index, pattern.length());
		auto naive_match = naive::fvc_matching::match(text_int, pattern_int, start_index);

		if (kmp_match) {
			std::cout << "match!" << std::endl;
		}

		if (naive_match) {
			naive_pos.insert(i);
		}

		if (naive_match != kmp_match) {
			std::cout << "error in KMP!";
		}
	}

	if (naive_pos != conv_pos) {

		for (auto x : naive_pos)
		{
			std::cout << x << " ";
		}
		std::cout << std::endl;

		for (auto x : conv_pos)
		{
			std::cout << x << " ";
		}
		std::cout << std::endl;

		std::cout << "error in ConvBase!";
	}
}

void test_pvcmatching()
{
	auto text_int = utils::alphabet::text_variable_int_reduction(text);
	auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

	auto pvckmp = pvc::KmpBased(pattern_int);

	auto pvc_conv = pvc::ConvolutionBased(std::shared_ptr<utils::NaiveConvolution>());
	auto conv_pos = std::set<int>();
	for (auto match : pvc_conv.get_matches(text_int, pattern_int))
	{
		conv_pos.insert(match);
	}
	auto naive_pos = std::set<int>();

	int j = 0;
	utils::injective_map::InjectiveMap bounds;
	for (int i = 0; i < text.length(); i++) {
		int start_index = i - pattern.length() + 1;

		auto kmp_match = pvckmp.match(text_int[i], j, bounds);
		if (start_index < 0)
			continue;

		auto text_substr = text.substr(start_index, pattern.length());
		auto naive_match = naive::pvc_matching::match(text_int, pattern_int, start_index);

		if (kmp_match) {
			std::cout << "match!" << std::endl;
		}

		if (naive_match) {
			naive_pos.insert(i);
		}

		if (naive_match != kmp_match) {
			std::cout << "error!";
		}
	}

	if (naive_pos != conv_pos) {
		std::cout << "error in ConvBase!";
	}
}

int main() {
	// test_pvcmatching();
	test_fvcmatching();

	return 0;
}