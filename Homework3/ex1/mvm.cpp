#include "common.h"
#include <immintrin.h>
#include <math.h>

void slow_performance1(double *x, double* A, double* y) {
  for (int j = 0; j < NC; j++)
    y[j] = 0;
    
  for (int i = 0; i < NR; i++){
    for (int j = 0; j < NC; j++){
      y[j] += A[i*NC+j] * x[i];
    }
  }
}


void maxperformance(double *x, double* A, double* y) {
  __m256d mmY0 = _mm256_setzero_pd();
  __m256d mmY1 = _mm256_setzero_pd();
  __m256d mmY2 = _mm256_setzero_pd();
  __m256d mmY3 = _mm256_setzero_pd();
  __m256d mmY4 = _mm256_setzero_pd();
  __m256d mmY5 = _mm256_setzero_pd();
  __m256d mmY6 = _mm256_setzero_pd();
  __m256d mmY7 = _mm256_setzero_pd();

  for (int i = 0; i < 64; ++i) {
    __m256d mmXval = _mm256_broadcast_sd(x + i);
    __m256d mmA1 = _mm256_load_pd(A + i * 32);
    __m256d mmA2 = _mm256_load_pd(A + i * 32 + 4);
    __m256d mmA3 = _mm256_load_pd(A + i * 32 + 8);
    __m256d mmA4 = _mm256_load_pd(A + i * 32 + 12);
    __m256d mmA5 = _mm256_load_pd(A + i * 32 + 16);
    __m256d mmA6 = _mm256_load_pd(A + i * 32 + 20);
    __m256d mmA7 = _mm256_load_pd(A + i * 32 + 24);
    __m256d mmA8 = _mm256_load_pd(A + i * 32 + 28);

    mmY0 = _mm256_fmadd_pd(mmA1, mmXval, mmY0);
    mmY1 = _mm256_fmadd_pd(mmA2, mmXval, mmY1);
    mmY2 = _mm256_fmadd_pd(mmA3, mmXval, mmY2);
    mmY3 = _mm256_fmadd_pd(mmA4, mmXval, mmY3);
    mmY4 = _mm256_fmadd_pd(mmA5, mmXval, mmY4);
    mmY5 = _mm256_fmadd_pd(mmA6, mmXval, mmY5);
    mmY6 = _mm256_fmadd_pd(mmA7, mmXval, mmY6);
    mmY7 = _mm256_fmadd_pd(mmA8, mmXval, mmY7);
  }

  _mm256_store_pd(y + 0 * 4, mmY0);
  _mm256_store_pd(y + 1 * 4, mmY1);
  _mm256_store_pd(y + 2 * 4, mmY2);
  _mm256_store_pd(y + 3 * 4, mmY3);
  _mm256_store_pd(y + 4 * 4, mmY4);
  _mm256_store_pd(y + 5 * 4, mmY5);
  _mm256_store_pd(y + 6 * 4, mmY6);
  _mm256_store_pd(y + 7 * 4, mmY7);

}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() 
{
  add_function(&slow_performance1, "slow_performance1",1);
  add_function(&maxperformance, "maxperformance",1);
}