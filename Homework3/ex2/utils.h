#pragma once
#include <random>

template<typename T>
void rands(T * m, size_t row, size_t col, double lo = -1.0, double up = 1.0)
{
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<T> dist(lo, up);
    for (size_t i = 0; i < row*col; ++i)  
        m[i] = dist(gen);
}

template<typename T>
void zeros(T * m, size_t row, size_t col)
{
    for (size_t i = 0; i < row*col; ++i)  
        m[i] = 0;
}

template<typename T>
void build(T **a, int m, int n, double lo = -1.0, double up = 1.0)
{
    *a = static_cast<T *>(aligned_alloc(32, m * n * sizeof(T)));
    rands(*a, m, n, lo, up);
}

template<typename T>
void destroy(T* m)
{
    free(m);
}

template<typename T>
T nrm_sqr_diff(T *x, T *y, int n) {
    T nrm_sqr = 0.0;
    for(int i = 0; i < n; i++) {
        nrm_sqr += (x[i] - y[i]) * (x[i] - y[i]);
    }
    
    if (isnan(nrm_sqr)) {
      nrm_sqr = INFINITY;
    }
    
    return nrm_sqr;
}