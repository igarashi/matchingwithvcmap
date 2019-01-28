#include "fft_convolution.hpp"
#include <algorithm>
#include <ccomplex>
#include <fftw3.h>
#include <cstring>

const double EPS = 0.5;

std::vector<unsigned long long> utils::FFTConvolution::calculate_sequential(const std::vector<int> &a,
                                                                            const std::vector<int> &b) const {
    const int a_length = a.size();
    const int b_length = b.size();

    const int N = a.size() + b.size() - 1;
    const int CN = N / 2 + 1;

    fftw_complex *ca, *cb;
    double *da, *db;

    // Create input data array
    da = fftw_alloc_real(N);
    db = fftw_alloc_real(N);
    ca = fftw_alloc_complex(CN);
    cb = fftw_alloc_complex(CN);

    fftw_plan pa, pb, presult;
    pa = fftw_plan_dft_r2c_1d(N, da, ca, FFTW_ESTIMATE);
    pb = fftw_plan_dft_r2c_1d(N, db, cb, FFTW_ESTIMATE);
    presult = fftw_plan_dft_c2r_1d(N, ca, da, FFTW_ESTIMATE);

    // Set data
    std::copy(a.begin(), a.end(), da);
    std::reverse_copy(b.begin(), b.end(), db);
    memset(&da[a_length], 0, sizeof(double) * (N - a_length));
    memset(&db[b_length], 0, sizeof(double) * (N - b_length));

    // execute dft
    fftw_execute(pa);
    fftw_execute(pb);

    // Use ca, da for storing result
    double ra, rb, ia, ib;
    for (int i = 0; i < CN; i++) {
        ra = ca[i][0];
        ia = ca[i][1];
        rb = cb[i][0];
        ib = cb[i][1];

        ca[i][0] = (ra * rb - ia * ib);
        ca[i][1] = (ra * ib + ia * rb);
    }

    fftw_execute(presult);
    std::vector<unsigned long long int> result(a.size() - b.size() + 1);
    for (int i = 0; i < result.size(); i++) {
        result[i] = (da[i + b.size() - 1] + EPS) / N;
    }

    fftw_destroy_plan(pa);
    fftw_destroy_plan(pb);
    fftw_destroy_plan(presult);

    fftw_free(ca);
    fftw_free(cb);
    fftw_free(da);
    fftw_free(db);

    return result;
}

std::vector<unsigned long long> utils::FFTConvolution::calculate_partial(const std::vector<int> &a,
                                                                         const std::vector<int> &b) const {
    auto double_a = new double[a.size()];
    std::copy(a.begin(), a.end(), double_a);

    const int a_length = a.size();
    const int b_length = b.size();

    const int W = 2 * b.size();
    const int N = W + b.size() - 1;
    const int CN = N / 2 + 1;

    fftw_complex *ca, *cb;
    double *da, *db;

    // Create input data array
    da = fftw_alloc_real(N);
    db = fftw_alloc_real(N);
    ca = fftw_alloc_complex(CN);
    cb = fftw_alloc_complex(CN);

    fftw_plan pa, pb, presult;
    pa = fftw_plan_dft_r2c_1d(N, da, ca, FFTW_ESTIMATE);
    pb = fftw_plan_dft_r2c_1d(N, db, cb, FFTW_ESTIMATE);
    presult = fftw_plan_dft_c2r_1d(N, ca, da, FFTW_ESTIMATE);

    // Set data B
    std::reverse_copy(b.begin(), b.end(), db);
    memset(&db[b_length], 0, sizeof(double) * (N - b_length));

    // Execute DFT for data B
    fftw_execute(pb);

    int result_index = 0;
    std::vector<unsigned long long int> result(a.size() - b.size() + 1);

    int current = 0;
    while (current < a_length) {
        int target_end = current + W;
        if (target_end > a_length)
            target_end = a_length;

        int next_w = target_end - current;

        // Set data A
        //std::copy(a.begin() + current, a.begin() + next_pos, da);
        memcpy(da, double_a + current, sizeof(double) * next_w);
        memset(&da[next_w], 0, sizeof(double) * (N - next_w));

        // execute dft
        fftw_execute(pa);

        // Use ca, da for storing result
        double ra, rb, ia, ib;
        for (int i = 0; i < CN; i++) {
            ra = ca[i][0];
            ia = ca[i][1];
            rb = cb[i][0];
            ib = cb[i][1];

            ca[i][0] = (ra * rb - ia * ib);
            ca[i][1] = (ra * ib + ia * rb);
        }

        fftw_execute(presult);

        int min_i = b.size() - 1;
        int max_i = next_w - 1;

        for (int i = 0; i < N; i++) {
            if (i < min_i || i > max_i)
                continue;

            result[result_index++] = (da[i] + EPS) / N;
        }

        current += b.size() + 1; // a - b + 1 = 2b - b + 1 = b + 1
    }

    fftw_destroy_plan(pa);
    fftw_destroy_plan(pb);
    fftw_destroy_plan(presult);

    fftw_free(ca);
    fftw_free(cb);
    fftw_free(da);
    fftw_free(db);

    return result;
}

std::vector<unsigned long long> utils::FFTConvolution::calculate(const std::vector<int> &a,
                                                                 const std::vector<int> &b) const {
    if (this->is_sequential_)
        return this->calculate_sequential(a, b);
    else
        return this->calculate_partial(a, b);
}
