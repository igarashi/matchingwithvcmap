#include <iostream>

#include "pvc/kmpbased.hpp"
#include "naive/pvcmatching.hpp"

int main() {
  std::string pattern = "cbCBAbAa";
  std::string text = "caacccaabbcacaacccacbabbcbaabbaabaaccacbcaabbccbcabbbcabbabcbbabaaccbaccccccbbacbbcabcbcbbacaabcababbabbbcccabbaabcbcabbcaacbaabcacbbbacbcaccaaaaccbaacabbacabaccbcabcabbcbcabaaaabaacccaabbbabbaaccbabcababbbbccccaaacacaaacaabbbaaccbcacabbcbbaaacabcbbcccaabaaacaabbbcbbbabbaccbccacbacbbbcbacaabccbbbaccaaacaaabcacaabcabaabcbbaaaccabababaaabbcaacbbabacbacbcabccabbbbbcababaabbacbccbccaabbbcabcaabccaacbbaccbbcaccbcbbcacbbaacbacabaaabcccbbabcbacaacccbaababbcccabbbcbbccccbaaabcaccaaabcbacccabbbbabbabcccbaaccabaaacacabaacbcccacabaabccbabbcccbcaaccbacbcbabacccbaaabacaacbabccccaabcacaabbaaaaababbbccbcccbbaaacccabbaacaacacabbccaabbcababbbbacabbaccbabababbbcccacbcaaacbbccabcaaabaaabbaaccaacaccbbacaacaacccccccbbbcbacaccccacabaabaacaccbcccaaaacacaaaacccbbbaabcbcccbabbcaccbaaaccabaacccbbcccbacbbbcbbaccabcccaaacccabacaccabcaccababaaacbabcabbcbbaaccbcccbabbbccabbabcbbcaaaccabcbacccaaaacbacabaccaabbcbbaabcbcbccbcaaccabababbcbcabbcccabccbacabaaaabacbaacabacaacacabaccabcaccbcccccbbabbaababbaaaccccaaaccaaabcaccacbccacbaabaaccabcbba";
  auto text_int = utils::alphabet::text_variable_int_reduction(text);
  auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

  auto fvckmp = pvc::KmpBased(pattern_int);

  int j = 0;
  utils::injective_map::InjectiveMap bounds;
  for (int i = 0; i < text.length(); i++) {
    int start_index = i - pattern.length() + 1;

    auto kmp_match = fvckmp.match(text_int[i], j, bounds);

    if (start_index < 0)
      continue;

    auto text_substr = text.substr(start_index, pattern.length());
    auto naive_match = naive::pvc_matching::match(text_int, pattern_int, start_index);

    if (kmp_match) {
      std::cout << "match!" << std::endl;
    }

    if (naive_match != kmp_match) {
      std::cout << "error!";
    }
  }

  return 0;
}