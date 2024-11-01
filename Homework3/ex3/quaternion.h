#pragma once

#define N (1024)

typedef struct {
    double r;
    double i;
    double j;
    double k;
} quaternion_t;

const quaternion_t _c = {2.0, 5.0, 0.25, 0.75};

static quaternion_t add(quaternion_t a, quaternion_t b) {
    quaternion_t r;
    r.r = a.r + b.r;
    r.i = a.i + b.i;
    r.j = a.j + b.j;
    r.k = a.k + b.k;
    return r;
}

static quaternion_t mul(quaternion_t a, quaternion_t b) {
    quaternion_t r;
    r.r = a.r*b.r - a.i*b.i - a.j*b.j - a.k*b.k;
    r.i = a.r*b.i + a.i*b.r + a.j*b.k - a.k*b.j;
    r.j = a.r*b.j - a.i*b.k + a.j*b.r + a.k*b.i;
    r.k = a.r*b.k + a.i*b.j - a.j*b.i + a.k*b.r;
    return r;
}

static double norm_sqr(quaternion_t a) {
    return ( a.r*a.r + a.i*a.i + a.j*a.j + a.k*a.k);
}