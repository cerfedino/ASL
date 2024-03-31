#include <immintrin.h>
#include "common.h"
#include "quaternion.h"
#include <iostream>

void slow_performance1(quaternion_t x[N], double y[N]) {
    for (int i = 0; i < N; i++) {
        y[i] = norm_sqr(mul(_c,x[i]));
    }
}




const __m256d mc = _mm256_set_pd(0.75, 0.25, 5.0, 2.0);
const __m256d mc_1 = _mm256_set_pd(_c.k, _c.j, _c.i, _c.r);
const __m256d mc_2 = _mm256_set_pd(-_c.j, _c.k, _c.r, -_c.i);
const __m256d mc_3 = _mm256_set_pd(_c.i, _c.r, -_c.k, -_c.j);
const __m256d mc_4 = _mm256_set_pd(_c.r, -_c.i, _c.j, -_c.k);

void maxperformance(quaternion_t x[N], double y[N]) {
  for (int i = 0; i < 1024; i += 4) {
    double *mx1 = (double*)(x + i);
    double *mx2 = (double*)(x + i + 1);
    double *mx3 = (double*)(x + i + 2);
    double *mx4 = (double*)(x + i + 3);

    __m256d mmx_1_1 = _mm256_broadcast_sd(mx1);
    __m256d mmx_1_2 = _mm256_broadcast_sd(mx2);
    __m256d mmx_1_3 = _mm256_broadcast_sd(mx3);
    __m256d mmx_1_4 = _mm256_broadcast_sd(mx4);

    __m256d mmx_2_1 = _mm256_broadcast_sd(mx1 + 1);
    __m256d mmx_2_2 = _mm256_broadcast_sd(mx2 + 1);
    __m256d mmx_2_3 = _mm256_broadcast_sd(mx3 + 1);
    __m256d mmx_2_4 = _mm256_broadcast_sd(mx4 + 1);

    __m256d mmx_3_1 = _mm256_broadcast_sd(mx1 + 2);
    __m256d mmx_3_2 = _mm256_broadcast_sd(mx2 + 2);
    __m256d mmx_3_3 = _mm256_broadcast_sd(mx3 + 2);
    __m256d mmx_3_4 = _mm256_broadcast_sd(mx4 + 2);

    __m256d mmx_4_1 = _mm256_broadcast_sd(mx1 + 3);
    __m256d mmx_4_2 = _mm256_broadcast_sd(mx2 + 3);
    __m256d mmx_4_3 = _mm256_broadcast_sd(mx3 + 3);
    __m256d mmx_4_4 = _mm256_broadcast_sd(mx4 + 3);


    __m256d mmY_1_1 = _mm256_fmadd_pd(mmx_4_1, mc_4, _mm256_fmadd_pd(mmx_3_1, mc_3, _mm256_fmadd_pd(mmx_2_1, mc_2, _mm256_mul_pd(mmx_1_1, mc_1))));
    __m256d mmY_1_2 = _mm256_fmadd_pd(mmx_4_2, mc_4, _mm256_fmadd_pd(mmx_3_2, mc_3, _mm256_fmadd_pd(mmx_2_2, mc_2, _mm256_mul_pd(mmx_1_2, mc_1))));
    __m256d mmY_1_3 = _mm256_fmadd_pd(mmx_4_3, mc_4, _mm256_fmadd_pd(mmx_3_3, mc_3, _mm256_fmadd_pd(mmx_2_3, mc_2, _mm256_mul_pd(mmx_1_3, mc_1))));
    __m256d mmY_1_4 = _mm256_fmadd_pd(mmx_4_4, mc_4, _mm256_fmadd_pd(mmx_3_4, mc_3, _mm256_fmadd_pd(mmx_2_4, mc_2, _mm256_mul_pd(mmx_1_4, mc_1))));


    mmY_1_1 = _mm256_mul_pd(mmY_1_1, mmY_1_1);
    mmY_1_2 = _mm256_mul_pd(mmY_1_2, mmY_1_2);
    mmY_1_3 = _mm256_mul_pd(mmY_1_3, mmY_1_3);
    mmY_1_4 = _mm256_mul_pd(mmY_1_4, mmY_1_4);


    __m256d temp1 = _mm256_permute2f128_pd(mmY_1_1, mmY_1_1, 1);
    temp1 = _mm256_add_pd(temp1, mmY_1_1);
    __m256d temp2 = _mm256_permute2f128_pd(mmY_1_2, mmY_1_2, 1);
    temp2 = _mm256_add_pd(temp2, mmY_1_2);
    __m256d temp3 = _mm256_permute2f128_pd(mmY_1_3, mmY_1_3, 1);
    temp3 = _mm256_add_pd(temp3, mmY_1_3);
    __m256d temp4 = _mm256_permute2f128_pd(mmY_1_4, mmY_1_4, 1);
    temp4 = _mm256_add_pd(temp4, mmY_1_4);

    y[i] = _mm256_cvtsd_f64(_mm256_hadd_pd(temp1, temp1));
    y[i+1] = _mm256_cvtsd_f64(_mm256_hadd_pd(temp2, temp2));
    y[i+2] = _mm256_cvtsd_f64(_mm256_hadd_pd(temp3, temp3));
    y[i+3] = _mm256_cvtsd_f64(_mm256_hadd_pd(temp4, temp4));

  }
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() {
    add_function(&slow_performance1, "slow_performance1",1);
    add_function(&maxperformance, "maxperformance",1);
}