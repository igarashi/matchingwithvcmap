#include <chrono>
#include <iostream>
#include <vector>

#include "cmdline.h"

#include <fvc/kmpbased.hpp>
#include <fvc/convolutionbased.hpp>
#include <pvc/convolutionbased.hpp>
#include <naive/fvcmatching.hpp>
#include <naive/pvcmatching.hpp>
#include <pvc/kmpbased.hpp>
#include <utils/string.hpp>
#include <utils/fft_convolution.hpp>


#define BEGIN_PERF {auto __start = std::chrono::system_clock::now();
#define END_PERF(vec) auto __end = std::chrono::system_clock::now();\
vec.push_back(std::chrono::duration_cast<std::chrono::microseconds>(__end - __start).count());}


inline void progress(int current, int total, std::ostream& of = std::cerr) {
  const int bar_width = 50;
  float per = float(current) / total;
  int bar = int(bar_width * per);

  of << "\r[";
  for (int i = 0; i < bar_width; i++) {
    if (i < bar) of << '=';
    else if (i == bar) of << '>';
    else of << ' ';
  }
  of << utils::string::strprintf("] %.2f%% %d/%d", per * 100.0, current, total);
}

void test_convbased_pvc(int n_case, int text_length, int pattern_length,
                        int text_alphabet_size, int pattern_alphabet_size, int variable_ratio,
                        std::vector<long long int>& preprocessing_time, std::vector<long long int>& query_time,
                        std::vector<long long int>& naive_time, std::vector<int>& match_count) {

    for (auto c = 0; c < n_case; c++) {
        progress(c + 1, n_case);

        int conv_count = 0;
        int naive_count = 0;

        auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
        auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size,
                                                             (unsigned int) (variable_ratio));

        auto text_int = utils::alphabet::text_variable_int_reduction(text);
        auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

        auto conv = std::make_shared<utils::FFTConvolution>(false);
        pvc::ConvolutionBased *pvc_conv;
        BEGIN_PERF
            pvc_conv = new pvc::ConvolutionBased(conv);
        END_PERF(preprocessing_time)

        BEGIN_PERF
            auto conv_match = pvc_conv->get_matches(text_int, pattern_int);
            conv_count = conv_match.size();
        END_PERF(query_time)

        BEGIN_PERF
            for (int i = 0; i < text.length(); i++) {
                auto naive_match = naive::fvc_matching::match(text_int, pattern_int, i);
                if (naive_match) naive_count++;
            }
        END_PERF(naive_time)

        delete pvc_conv;

        if (conv_count != naive_count) {
            std::cerr << "[*] Detect conv_count != naive_count" << std::endl;
        }

        match_count.push_back(naive_count);
    }
    std::cerr << " Done." << std::endl;
}

void test_convbased_fvc(int n_case, int text_length, int pattern_length,
                       int text_alphabet_size, int pattern_alphabet_size, int variable_ratio,
                       std::vector<long long int>& preprocessing_time, std::vector<long long int>& query_time,
                       std::vector<long long int>& naive_time, std::vector<int>& match_count) {

    for (auto c = 0; c < n_case; c++) {
        progress(c + 1, n_case);

        int conv_count = 0;
        int naive_count = 0;

        auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
        auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size,
                                                             (unsigned int) (variable_ratio));

        auto text_int = utils::alphabet::text_variable_int_reduction(text);
        auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

        auto conv = std::make_shared<utils::FFTConvolution>(false);
        fvc::ConvolutionBased *fvc_conv;
        BEGIN_PERF
            fvc_conv = new fvc::ConvolutionBased(conv);
        END_PERF(preprocessing_time)

        BEGIN_PERF
            auto pvc_match = fvc_conv->get_matches(text_int, pattern_int);
            conv_count = pvc_match.size();
        END_PERF(query_time)

        BEGIN_PERF
            for (int i = 0; i < text.length(); i++) {
                auto naive_match = naive::fvc_matching::match(text_int, pattern_int, i);
                if (naive_match) naive_count++;
            }
        END_PERF(naive_time)

        delete fvc_conv;

        if (conv_count != naive_count) {
            std::cerr << "[*] Detect conv_count != naive_count" << std::endl;
        }

        match_count.push_back(naive_count);
    }
    std::cerr << " Done." << std::endl;
}

void test_kmpbased_fvc(int n_case, int text_length, int pattern_length,
                       int text_alphabet_size, int pattern_alphabet_size, int variable_ratio,
                       std::vector<long long int>& preprocessing_time, std::vector<long long int>& query_time,
                       std::vector<long long int>& naive_time, std::vector<int>& match_count) {

  for (auto c = 0; c < n_case; c++) {
    progress(c + 1, n_case);

    int kmp_count = 0;
    int naive_count = 0;

    auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
    auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size,
                                                         (unsigned int) (variable_ratio));

    auto text_int = utils::alphabet::text_variable_int_reduction(text);
    auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

    fvc::KmpBased *fvckmp;
    BEGIN_PERF
      fvckmp = new fvc::KmpBased(pattern_int);
    END_PERF(preprocessing_time)

    auto j = 0;
    std::map<int, int> bounds;

    BEGIN_PERF
      for (int i = 0; i < text.length(); i++) {
        auto kmp_match = fvckmp->match(text_int[i], j, bounds);
        if (kmp_match) kmp_count++;
      }
    END_PERF(query_time)

    BEGIN_PERF
      for (int i = 0; i < text.length(); i++) {
        auto naive_match = naive::fvc_matching::match(text_int, pattern_int, i);
        if (naive_match) naive_count++;
      }
    END_PERF(naive_time)

    delete fvckmp;

    if (kmp_count != naive_count) {
      std::cerr << "[*] Detect kmp_count != naive_count" << std::endl;
    }

    match_count.push_back(naive_count);
  }
  std::cerr << " Done." << std::endl;
}


void test_kmpbased_pvc(int n_case, int text_length, int pattern_length,
                       int text_alphabet_size, int pattern_alphabet_size, int variable_ratio,
                       std::vector<long long int>& preprocessing_time, std::vector<long long int>& query_time,
                       std::vector<long long int>& naive_time, std::vector<int>& match_count) {

  for (auto c = 0; c < n_case; c++) {
    progress(c + 1, n_case);

    int kmp_count = 0;
    int naive_count = 0;

    auto text = utils::alphabet::generate_test_constant_string(text_length, text_alphabet_size);
    auto pattern = utils::alphabet::generate_test_string(pattern_length, pattern_alphabet_size,
                                                         (unsigned int) (variable_ratio));

    auto text_int = utils::alphabet::text_variable_int_reduction(text);
    auto pattern_int = utils::alphabet::text_variable_int_reduction(pattern);

    pvc::KmpBased *pvckmp;
    BEGIN_PERF
      pvckmp = new pvc::KmpBased(pattern_int);
    END_PERF(preprocessing_time)

    auto j = 0;
    utils::injective_map::InjectiveMap bounds;

    BEGIN_PERF
      for (int i = 0; i < text.length(); i++) {
        auto kmp_match = pvckmp->match(text_int[i], j, bounds);
        if (kmp_match) kmp_count++;
      }
    END_PERF(query_time)

    BEGIN_PERF
      for (int i = 0; i < text.length(); i++) {
        auto naive_match = naive::pvc_matching::match(text_int, pattern_int, i);
        if (naive_match) naive_count++;
      }
    END_PERF(naive_time)

    delete pvckmp;

    if (kmp_count != naive_count) {
      std::cerr << "[*] Detect kmp_count != naive_count" << std::endl;
    }

    match_count.push_back(naive_count);
  }
  std::cerr << " Done." << std::endl;
}


int main(int argc, char *argv[]) {
  cmdline::parser p;
  p.add<std::string>("mode", 'm', "test mode", true, "fvckmp", cmdline::oneof<std::string>("fvckmp", "pvckmp", "fvcconv", "pvcconv"));
  p.add<int>("case", 'n', "number of test case", false, 100);
  p.add<int>("tlen", 't', "length of text", false, 10000);
  p.add<int>("plen", 'p', "length of pattern", false, 5);
  p.add<int>("talpha", 'a', "alphabet size of text", false, 13);
  p.add<int>("palpha", 'b', "alphabet size of pattern", false, 3);
  p.add<int>("vratio", 'v', "variable ratio of pattern (variable:constant=[ratio]:1)", false, 1);

  p.parse_check(argc, argv);

  std::vector<long long int> preprocessing;
  std::vector<long long int> query;
  std::vector<long long int> naive;
  std::vector<int> match;

  std::string mode = p.get<std::string>("mode");
  std::cerr << "[*] mode=" << mode
            << utils::string::strprintf(" (case=%d, tlen=%d, plen=%d, talpha=%d, palpha=%d, vratio=%d)",
                                        p.get<int>("case"),
                                        p.get<int>("tlen"),
                                        p.get<int>("plen"),
                                        p.get<int>("talpha"),
                                        p.get<int>("palpha"),
                                        p.get<int>("vratio")) << std::endl;
  if (mode == "fvckmp") {
    try {
      test_kmpbased_fvc(p.get<int>("case"),
                        p.get<int>("tlen"),
                        p.get<int>("plen"),
                        p.get<int>("talpha"),
                        p.get<int>("palpha"),
                        p.get<int>("vratio"),
                        preprocessing,
                        query,
                        naive,
                        match);
    } catch (std::string e) {
      std::cerr << "\n An error has occured: " << e << std::endl;
      exit(-1);
    }
  } else if (mode == "pvckmp") {
    try {
      test_kmpbased_pvc(p.get<int>("case"),
                        p.get<int>("tlen"),
                        p.get<int>("plen"),
                        p.get<int>("talpha"),
                        p.get<int>("palpha"),
                        p.get<int>("vratio"),
                        preprocessing,
                        query,
                        naive,
                        match);
    } catch (std::string e) {
      std::cerr << "\n An error has occured: " << e << std::endl;
      exit(-1);
    }
  } else if (mode == "pvcconv")  {
      try {
          test_convbased_pvc(p.get<int>("case"),
                            p.get<int>("tlen"),
                            p.get<int>("plen"),
                            p.get<int>("talpha"),
                            p.get<int>("palpha"),
                            p.get<int>("vratio"),
                            preprocessing,
                            query,
                            naive,
                            match);
      } catch (std::string e) {
          std::cerr << "\n An error has occured: " << e << std::endl;
          exit(-1);
      }
  } else if (mode == "fvcconv")  {
      try {
          test_convbased_fvc(p.get<int>("case"),
                            p.get<int>("tlen"),
                            p.get<int>("plen"),
                            p.get<int>("talpha"),
                            p.get<int>("palpha"),
                            p.get<int>("vratio"),
                            preprocessing,
                            query,
                            naive,
                            match);
      } catch (std::string e) {
          std::cerr << "\n An error has occured: " << e << std::endl;
          exit(-1);
      }
  } else {
    std::cerr << "mode:" << mode << " is currently not supported." << std::endl;
    std::terminate();
  }

  if (preprocessing.size() > 0) {
    for (int i = 0; i < query.size(); i++) {
      std::cout << utils::string::strprintf("%lld\t%lld\t%lld\t%d", preprocessing[i], query[i], naive[i], match[i])
                << std::endl;
    }
  } else {
    for (int i = 0; i < query.size(); i++) {
      std::cout << utils::string::strprintf("0\t%lld\t%lld\t%d", query[i], naive[i], match[i])
                << std::endl;
    }
  }
}
