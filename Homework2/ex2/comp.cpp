#include "common.h"
#include "mat.h"

#define C (1.0 / 3.0)
#define M_PI3 M_PI / 3
#define ALPHA 110.0 / 41.0

using namespace std;


void v3(mat* x, mat* y, mat* z) {
    double t[12];

    double recipCosines[6];
    recipCosines[0] = 1;
    recipCosines[1] = 1 / cos(M_PI3);
    recipCosines[2] = 1 / cos(M_PI3 * 2);
    recipCosines[3] = -1;
    recipCosines[4] = recipCosines[2];
    recipCosines[5] = recipCosines[1];

    double outerCosine[3];
    outerCosine[0] = cos(x->data[0]);
    outerCosine[1] = cos(x->data[1]);
    outerCosine[2] = cos(x->data[2]);

    double outerSqrts[6];
    outerSqrts[0] = 1 / sqrt(x->data[0]);
    outerSqrts[1] = 1 / sqrt(x->data[1]);
    outerSqrts[2] = 1 / sqrt(x->data[2]);
    outerSqrts[3] = 1 / sqrt(x->data[3]);
    outerSqrts[4] = 1 / sqrt(x->data[4]);
    outerSqrts[5] = 1 / sqrt(x->data[5]);

    // ## 11 flop

    // 2x loop unrolling
    for (int i = 0; i < 100; i += 2) {
        double sqrt1 = 1 / sqrt(y->data[i * 100]);

        double recipInnerSqrts[6];
        recipInnerSqrts[0] = sqrt1 * outerSqrts[0];
        recipInnerSqrts[1] = sqrt1 * outerSqrts[1];
        recipInnerSqrts[2] = sqrt1 * outerSqrts[2];
        recipInnerSqrts[3] = sqrt1 * outerSqrts[3];
        recipInnerSqrts[4] = sqrt1 * outerSqrts[4];
        recipInnerSqrts[5] = sqrt1 * outerSqrts[5];

        int j = 0;
        // 6x loop unrolling
        for (; j + 6 < 100; j += 6) {
            int t1_idx = i * 100 + j;
            int t2_idx = (i + 1) * 100 + j;

            t[0] = (z->data[t1_idx] * recipInnerSqrts[0] + y->data[t1_idx]) * outerCosine[0];
            t[1] = (z->data[t2_idx] * recipCosines[0] + x->data[t2_idx]) * outerCosine[0];
            t[2] = (z->data[t1_idx + 1] * recipInnerSqrts[1] + y->data[t1_idx + 1]) * outerCosine[1];
            t[3] = (z->data[t2_idx + 1] * recipCosines[1] + x->data[t2_idx + 1]) * outerCosine[1];
            t[4] = (z->data[t1_idx + 2] * recipInnerSqrts[2] + y->data[t1_idx + 2]) * outerCosine[2];
            t[5] = (z->data[t2_idx + 2] * recipCosines[2] + x->data[t2_idx + 2]) * outerCosine[2];
            t[6] = (z->data[t1_idx + 3] * recipInnerSqrts[3] + y->data[t1_idx + 3]) * outerCosine[0];
            t[7] = (z->data[t2_idx + 3] * recipCosines[3] + x->data[t2_idx + 3]) * outerCosine[0];
            t[8] = (z->data[t1_idx + 4] * recipInnerSqrts[4] + y->data[t1_idx + 4]) * outerCosine[1];
            t[9] = (z->data[t2_idx + 4] * recipCosines[4] + x->data[t2_idx + 4]) * outerCosine[1];
            t[10] = (z->data[t1_idx + 5] * recipInnerSqrts[5] + y->data[t1_idx + 5]) * outerCosine[2];
            t[11] = (z->data[t2_idx + 5] * recipCosines[5] + x->data[t2_idx + 5]) * outerCosine[2];
            // 39
            
            t[0] = t[0] > 0 ? t[0] * ALPHA + 1 : abs(t[0]) + 1;
            t[1] = t[1] > 0 ? t[1] * ALPHA + 1 : abs(t[1]) + 1;
            t[2] = t[2] > 0 ? t[2] * ALPHA + 1 : abs(t[2]) + 1;
            t[3] = t[3] > 0 ? t[3] * ALPHA + 1 : abs(t[3]) + 1;
            t[4] = t[4] > 0 ? t[4] * ALPHA + 1 : abs(t[4]) + 1;
            t[5] = t[5] > 0 ? t[5] * ALPHA + 1 : abs(t[5]) + 1;
            t[6] = t[6] > 0 ? t[6] * ALPHA + 1 : abs(t[6]) + 1;
            t[7] = t[7] > 0 ? t[7] * ALPHA + 1 : abs(t[7]) + 1;
            t[8] = t[8] > 0 ? t[8] * ALPHA + 1 : abs(t[8]) + 1;
            t[9] = t[9] > 0 ? t[9] * ALPHA + 1 : abs(t[9]) + 1;
            t[10] = t[10] > 0 ? t[10] * ALPHA + 1 : abs(t[10]) + 1;
            t[11] = t[11] > 0 ? t[11] * ALPHA + 1 : abs(t[11]) + 1;


            z->data[t1_idx] = t[0];
            z->data[t2_idx] = t[1];
            z->data[t1_idx + 1] = t[2];
            z->data[t2_idx + 1] = t[3];
            z->data[t1_idx + 2] = t[4];
            z->data[t2_idx + 2] = t[5];
            z->data[t1_idx + 3] = t[6];
            z->data[t2_idx + 3] = t[7];
            z->data[t1_idx + 4] = t[8];
            z->data[t2_idx + 4] = t[9];
            z->data[t1_idx + 5] = t[10];
            z->data[t2_idx + 5] = t[11];
        }

        // Finish off last 4 elements
        for (; j < 100; j += 4) {
            int t1_idx = i * 100 + j;
            int t2_idx = (i + 1) * 100 + j;

            t[0] = (z->data[t1_idx] * recipInnerSqrts[0] + y->data[t1_idx]) * outerCosine[0];
            t[1] = (z->data[t2_idx] * recipCosines[0] + x->data[t2_idx]) * outerCosine[0];
            t[2] = (z->data[t1_idx + 1] * recipInnerSqrts[1] + y->data[t1_idx + 1]) * outerCosine[1];
            t[3] = (z->data[t2_idx + 1] * recipCosines[1] + x->data[t2_idx + 1]) * outerCosine[1];
            t[4] = (z->data[t1_idx + 2] * recipInnerSqrts[2] + y->data[t1_idx + 2]) * outerCosine[2];
            t[5] = (z->data[t2_idx + 2] * recipCosines[2] + x->data[t2_idx + 2]) * outerCosine[2];
            t[6] = (z->data[t1_idx + 3] * recipInnerSqrts[3] + y->data[t1_idx + 3]) * outerCosine[0];
            t[7] = (z->data[t2_idx + 3] * recipCosines[3] + x->data[t2_idx + 3]) * outerCosine[0];

            t[0] = t[0] > 0 ? t[0] * ALPHA + 1 : abs(t[0]) + 1;
            t[1] = t[1] > 0 ? t[1] * ALPHA + 1 : abs(t[1]) + 1;
            t[2] = t[2] > 0 ? t[2] * ALPHA + 1 : abs(t[2]) + 1;
            t[3] = t[3] > 0 ? t[3] * ALPHA + 1 : abs(t[3]) + 1;
            t[4] = t[4] > 0 ? t[4] * ALPHA + 1 : abs(t[4]) + 1;
            t[5] = t[5] > 0 ? t[5] * ALPHA + 1 : abs(t[5]) + 1;
            t[6] = t[6] > 0 ? t[6] * ALPHA + 1 : abs(t[6]) + 1;
            t[7] = t[7] > 0 ? t[7] * ALPHA + 1 : abs(t[7]) + 1;

            z->data[t1_idx] = t[0];
            z->data[t2_idx] = t[1];
            z->data[t1_idx + 1] = t[2];
            z->data[t2_idx + 1] = t[3];
            z->data[t1_idx + 2] = t[4];
            z->data[t2_idx + 2] = t[5];
            z->data[t1_idx + 3] = t[6];
            z->data[t2_idx + 3] = t[7];
        }
    }
}



void v2(mat* x, mat* y, mat* z) {
    double t1;
    double t2;
    double t3;
    double t4;
    double t5;
    double t6;
    double t7;
    double t8;
    double t9;
    double t10;
    double t11;
    double t12;

    double outerSqrts[6];
    outerSqrts[0] = 1 / sqrt(x->data[0]);
    outerSqrts[1] = 1 / sqrt(x->data[1]);
    outerSqrts[2] = 1 / sqrt(x->data[2]);
    outerSqrts[3] = 1 / sqrt(x->data[3]);
    outerSqrts[4] = 1 / sqrt(x->data[4]);
    outerSqrts[5] = 1 / sqrt(x->data[5]);

    for (int i = 0; i < 100; i += 2) {
        double sqrt1 = 1 / sqrt(y->data[i * 100]);

        double recipInnerSqrts[6];
        recipInnerSqrts[0] = sqrt1 * outerSqrts[0];
        recipInnerSqrts[1] = sqrt1 * outerSqrts[1];
        recipInnerSqrts[2] = sqrt1 * outerSqrts[2];
        recipInnerSqrts[3] = sqrt1 * outerSqrts[3];
        recipInnerSqrts[4] = sqrt1 * outerSqrts[4];
        recipInnerSqrts[5] = sqrt1 * outerSqrts[5];

        int j=0;
        for (; j+6 < 100; j+=6) {
            int t1_idx = i * 100 + j;
            int t2_idx = (i + 1) * 100 + j;
            
            t1 = z->data[t1_idx] * recipInnerSqrts[0] + y->data[t1_idx];
            t2 = z->data[t2_idx] / cos(C * M_PI * j) + x->data[t2_idx];
            t3 = z->data[t1_idx+1] * recipInnerSqrts[1] + y->data[t1_idx+1];
            t4 = z->data[t2_idx+1] / cos(C * M_PI * (j+1)) + x->data[t2_idx+1];
            t5 = z->data[t1_idx+2] * recipInnerSqrts[2] + y->data[t1_idx+2];
            t6 = z->data[t2_idx+2] / cos(C * M_PI * (j+2)) + x->data[t2_idx+2];
            t7 = z->data[t1_idx+3] * recipInnerSqrts[3] + y->data[t1_idx+3];
            t8 = z->data[t2_idx+3] / cos(C * M_PI * (j+3)) + x->data[t2_idx+3];
            t9 = z->data[t1_idx+4] * recipInnerSqrts[4] + y->data[t1_idx+4];
            t10 = z->data[t2_idx+4] / cos(C * M_PI * (j+4)) + x->data[t2_idx+4];
            t11 = z->data[t1_idx+5] * recipInnerSqrts[5] + y->data[t1_idx+5];
            t12 = z->data[t2_idx+5] / cos(C * M_PI * (j+5)) + x->data[t2_idx+5];

            z->data[t1_idx] = t1 * cos(x->data[0]);
            z->data[t2_idx] = t2 * cos(x->data[0]);
            z->data[t1_idx+1] = t3 * cos(x->data[1]);
            z->data[t2_idx+1] = t4 * cos(x->data[1]);
            z->data[t1_idx+2] = t5 * cos(x->data[2]);
            z->data[t2_idx+2] = t6 * cos(x->data[2]);
            z->data[t1_idx+3] = t7 * cos(x->data[0]);
            z->data[t2_idx+3] = t8 * cos(x->data[0]);
            z->data[t1_idx+4] = t9 * cos(x->data[1]);
            z->data[t2_idx+4] = t10 * cos(x->data[1]);
            z->data[t1_idx+5] = t11 * cos(x->data[2]);
            z->data[t2_idx+5] = t12 * cos(x->data[2]);
        }
        
        for (; j < 100; j+=4) {
            int t1_idx = i * 100 + j;
            int t2_idx = (i + 1) * 100 + j;
            
            t1 = z->data[t1_idx] * recipInnerSqrts[0] + y->data[t1_idx];
            t2 = z->data[t2_idx] / cos(C * M_PI * j) + x->data[t2_idx];
            t3 = z->data[t1_idx+1] * recipInnerSqrts[1] + y->data[t1_idx+1];
            t4 = z->data[t2_idx+1] / cos(C * M_PI * (j+1)) + x->data[t2_idx+1];
            t5 = z->data[t1_idx+2] * recipInnerSqrts[2] + y->data[t1_idx+2];
            t6 = z->data[t2_idx+2] / cos(C * M_PI * (j+2)) + x->data[t2_idx+2];
            t7 = z->data[t1_idx+3] * recipInnerSqrts[3] + y->data[t1_idx+3];
            t8 = z->data[t2_idx+3] / cos(C * M_PI * (j+3)) + x->data[t2_idx+3];

            z->data[t1_idx] = t1 * cos(x->data[0]);
            z->data[t2_idx] = t2 * cos(x->data[0]);
            z->data[t1_idx+1] = t3 * cos(x->data[1]);
            z->data[t2_idx+1] = t4 * cos(x->data[1]);
            z->data[t1_idx+2] = t5 * cos(x->data[2]);
            z->data[t2_idx+2] = t6 * cos(x->data[2]);
            z->data[t1_idx+3] = t7 * cos(x->data[0]);
            z->data[t2_idx+3] = t8 * cos(x->data[0]);
        }
    }
    for (int j = 0; j < 10000; j += 4) {
        double t0 = z->data[j];
        double t1 = z->data[j + 1];
        double t2 = z->data[j + 2];
        double t3 = z->data[j + 3];

        t0 = t0 > 0 ? t0 * ALPHA + 1 : abs(t0) + 1;
        t1 = t1 > 0 ? t1 * ALPHA + 1 : abs(t1) + 1;
        t2 = t2 > 0 ? t2 * ALPHA + 1 : abs(t2) + 1;
        t3 = t3 > 0 ? t3 * ALPHA + 1 : abs(t3) + 1;

        z->data[j] = t0;
        z->data[j + 1] = t1;
        z->data[j + 2] = t2;
        z->data[j + 3] = t3;
    }
}



void v1(mat* x, mat* y, mat* z) {
    double t1;
    double t2;
    double t3;
    double t4;
    double t5;
    double t6;
    double t7;
    double t8;
    double t9;
    double t10;
    double t11;
    double t12;
    for (int i = 0; i < 100; i += 2) {
        int j=0;
        for (; j+6 < 100; j+=6) {
            int t1_idx = i * 100 + j;
            int t2_idx = (i + 1) * 100 + j;
            
            t1 = z->data[t1_idx] / (sqrt(y->data[i * 100]) * sqrt(x->data[j % 6])) + y->data[t1_idx];
            t2 = z->data[t2_idx] / cos(C * M_PI * j) + x->data[t2_idx];
            t3 = z->data[t1_idx+1] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+1) % 6])) + y->data[t1_idx+1];
            t4 = z->data[t2_idx+1] / cos(C * M_PI * (j+1)) + x->data[t2_idx+1];
            t5 = z->data[t1_idx+2] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+2) % 6])) + y->data[t1_idx+2];
            t6 = z->data[t2_idx+2] / cos(C * M_PI * (j+2)) + x->data[t2_idx+2];
            t7 = z->data[t1_idx+3] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+3) % 6])) + y->data[t1_idx+3];
            t8 = z->data[t2_idx+3] / cos(C * M_PI * (j+3)) + x->data[t2_idx+3];
            t9 = z->data[t1_idx+4] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+4) % 6])) + y->data[t1_idx+4];
            t10 = z->data[t2_idx+4] / cos(C * M_PI * (j+4)) + x->data[t2_idx+4];
            t11 = z->data[t1_idx+5] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+5) % 6])) + y->data[t1_idx+5];
            t12 = z->data[t2_idx+5] / cos(C * M_PI * (j+5)) + x->data[t2_idx+5];

            z->data[t1_idx] = t1 * cos(x->data[j % 3]);
            z->data[t2_idx] = t2 * cos(x->data[j % 3]);
            z->data[t1_idx+1] = t3 * cos(x->data[(j+1) % 3]);
            z->data[t2_idx+1] = t4 * cos(x->data[(j+1) % 3]);

            z->data[t1_idx+2] = t5 * cos(x->data[(j+2) % 3]);
            z->data[t2_idx+2] = t6 * cos(x->data[(j+2) % 3]);

            z->data[t1_idx+3] = t7 * cos(x->data[(j+3) % 3]);
            z->data[t2_idx+3] = t8 * cos(x->data[(j+3) % 3]);

            z->data[t1_idx+4] = t9 * cos(x->data[(j+4) % 3]);
            z->data[t2_idx+4] = t10 * cos(x->data[(j+4) % 3]);

            z->data[t1_idx+5] = t11 * cos(x->data[(j+5) % 3]);
            z->data[t2_idx+5] = t12 * cos(x->data[(j+5) % 3]);
        }
        
        for (; j < 100; j+=4) {
            int t1_idx = i * 100 + j;
            int t2_idx = (i + 1) * 100 + j;
            
            t1 = z->data[t1_idx] / (sqrt(y->data[i * 100]) * sqrt(x->data[j % 6])) + y->data[t1_idx];
            t2 = z->data[t2_idx] / cos(C * M_PI * j) + x->data[t2_idx];
            
            t3 = z->data[t1_idx+1] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+1) % 6])) + y->data[t1_idx+1];
            t4 = z->data[t2_idx+1] / cos(C * M_PI * (j+1)) + x->data[t2_idx+1];
            
            t5 = z->data[t1_idx+2] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+2) % 6])) + y->data[t1_idx+2];
            t6 = z->data[t2_idx+2] / cos(C * M_PI * (j+2)) + x->data[t2_idx+2];
            
            t7 = z->data[t1_idx+3] / (sqrt(y->data[i * 100]) * sqrt(x->data[(j+3) % 6])) + y->data[t1_idx+3];
            t8 = z->data[t2_idx+3] / cos(C * M_PI * (j+3)) + x->data[t2_idx+3];
            

            z->data[t1_idx] = t1 * cos(x->data[j % 3]);
            z->data[t2_idx] = t2 * cos(x->data[j % 3]);

            z->data[t1_idx+1] = t3 * cos(x->data[(j+1) % 3]);
            z->data[t2_idx+1] = t4 * cos(x->data[(j+1) % 3]);

            z->data[t1_idx+2] = t5 * cos(x->data[(j+2) % 3]);
            z->data[t2_idx+2] = t6 * cos(x->data[(j+2) % 3]);

            z->data[t1_idx+3] = t7 * cos(x->data[(j+3) % 3]);
            z->data[t2_idx+3] = t8 * cos(x->data[(j+3) % 3]);
        }
    }
    for (int j = 0; j < 100*100; j+=4) {
        z->data[j] = abs(fmax(z->data[j], ALPHA * z->data[j])) + 1;
        z->data[j+1] = abs(fmax(z->data[j+1], ALPHA * z->data[j+1])) + 1;
        z->data[j+2] = abs(fmax(z->data[j+2], ALPHA * z->data[j+2])) + 1;
        z->data[j+3] = abs(fmax(z->data[j+3], ALPHA * z->data[j+3])) + 1;
    }
}



void slow_performance(mat* x, mat* y, mat* z) {
    double t1;
    for (int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2; j++) {
            if (i % 2) {
                t1 = mat_get(z, i, j) / cos(C * M_PI * j) + mat_get(x, i, j);
            } else {
                t1 = mat_get(z, i, j) /
                         (sqrt(mat_get(y, i, 0)) * sqrt(mat_get(x, 0, j % 6))) +
                     mat_get(y, i, j);
            }
            mat_set(z, i, j, t1 * cos(mat_get(x, 0, j % 3)));
        }
    }
    for (int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2; j++) {
            mat_activate(z, i, j, ALPHA);
        }
    }
}

void maxperformance(mat* x, mat* y, mat* z) { v3(x, y, z); }

/*
 * Called by the driver to register your functions
 * Use add_function(func, description) to add your own functions
 */
void register_functions() {
  add_function(&slow_performance, "slow_performance1", 1);
  add_function(&v1, "v1", 1);
  add_function(&v2, "v2", 1);
  add_function(&v3, "v3", 1);
}