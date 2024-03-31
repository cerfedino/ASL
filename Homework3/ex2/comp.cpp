#include <immintrin.h>
#include "common.h"

using namespace std;
#include <iostream>

void slow_performance1(double *x, double* y, double* z, int n) {
  for(int i = 0; i < n; i++){
    double a = i % 2 ? x[i-1] : x[i+1];
    double b = y[i];
    if(round(a) > round(b))
      z[i] = a + a*b;
    else
      z[i] = b - 3*i;
  }
}

__m256d mi_increment = _mm256_set_pd(4.0,4.0,4.0,4.0);
__m256d m3 = _mm256_set_pd(3.0,3.0,3.0,3.0);
void maxperformance(double *x, double* y, double* z, int n) {
  // printf("=== NEW RUN ===\n");
  __m256d mi = _mm256_set_pd(3.0,2.0,1.0,0.0);
  for(int i = 0; i < 1024; i+=4){
    
    __m256d mA1 = _mm256_load_pd(x + i);
    __m256d mB1 = _mm256_load_pd(y + i);
    
    mA1 = _mm256_permute_pd(mA1,0x5);
    
    __m256d mA1R = _mm256_round_pd(mA1, _MM_FROUND_TO_NEAREST_INT);
    __m256d mB1R = _mm256_round_pd(mB1, _MM_FROUND_TO_NEAREST_INT);
    
    __m256d mMask1 = _mm256_cmp_pd(mA1R,mB1R, _CMP_GT_OS);
    __m256d mNMask1 =  _mm256_cmp_pd(mA1R,mB1R, _CMP_LE_OS);
    
    
    __m256d mZ1a = _mm256_fmadd_pd(mA1, mB1, mA1);
    mZ1a = _mm256_and_pd(mZ1a, mMask1);
    
    
    __m256d mZ1b = _mm256_mul_pd(mi, m3);
    mZ1b = _mm256_sub_pd(mB1, mZ1b);
    mZ1b = _mm256_and_pd(mZ1b, mNMask1);
    
    __m256d mZ1 = _mm256_add_pd(mZ1a, mZ1b);
    
    _mm256_store_pd(z + i, mZ1);
    
    
    mi = _mm256_add_pd(mi, mi_increment);
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