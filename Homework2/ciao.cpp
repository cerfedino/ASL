#include "common.h"
#include "mat.h"


#define C (1.0/3.0)
#define ALPHA 110.0/41.0

using namespace std;

void slow_performance1(mat* x, mat* y, mat*z) {
    double t1;
    for (int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2; j++) {
            if (i % 2) {
                t1 = mat_get(z,i,j)/cos(C*M_PI*j) + mat_get(x,i,j);
            } else {
                t1 = mat_get(z,i,j)/(sqrt(mat_get(y,i,0)) * sqrt(mat_get(x,0,j%6))) + mat_get(y,i,j);
            }
            mat_set(z,i,j,t1*cos(mat_get(x,0,j%3)));
        }
    }
    for (int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2; j++) {
          mat_activate(z, i, j, ALPHA);
        }
    }
}



inline void mmat_set(mat* m, int ij, double val) {
    m->data[ij] = val;
}

/* retrieve vector element and store at val */
inline double mmat_get(mat* m, int ij) {
    return m->data[ij];
}

void mmat_activate(mat* m, int ij, double a) {
    double d = m->data[ij];
    m->data[ij] = fabs(fmax(d, a * d)) + 1;
}


void b2(mat* x, mat* y, mat* z) {
    double t10_0;
    double t11_0;
    double t10_1;
    double t11_1;
    double t10_2;
    double t11_2;   
    double t10_3;
    double t11_3;
    double t10_4;
    double t11_4;
    double t10_5;
    double t11_5;
    
    const float CM_PI = C * M_PI;

    double x0j30_0 = cos(mmat_get(x, 0));
    double y0j60_0 = sqrt(mmat_get(x, 0));
    double x0j30_1 = cos(mmat_get(x, 1));
    double y0j60_1 = sqrt(mmat_get(x, 1));
    double x0j30_2 = cos(mmat_get(x, 2));
    double y0j60_2 = sqrt(mmat_get(x, 2));
    double y0j60_3 = sqrt(mmat_get(x, 3));
    double y0j60_4 = sqrt(mmat_get(x, 4));
    double y0j60_5 = sqrt(mmat_get(x, 5));

    for (int i = 0; i < 100; i += 2) {
        int i100 = i * 100;
        int i200 = (i+1) * 100;
        double y00_0 = sqrt(mmat_get(y, i100));
        double y01_0 = sqrt(mmat_get(y, i200));

        int j=0;
        for (; j+5 < 100; j+=6) {
            
            double yj0_0 = mmat_get(y, i100+j);
            double zj0_0 = mmat_get(z, i100+j);
            double xj1_0 = mmat_get(x, i200+j);
            double zj1_0 = mmat_get(z, i200+j);

            double yj0_1 = mmat_get(y, i100+j+1);
            double zj0_1 = mmat_get(z, i100+j+1);
            double xj1_1 = mmat_get(x, i200+j+1);
            double zj1_1 = mmat_get(z, i200+j+1);

            double yj0_2 = mmat_get(y, i100+j+2);
            double zj0_2 = mmat_get(z, i100+j+2);
            double xj1_2 = mmat_get(x, i200+j+2);
            double zj1_2 = mmat_get(z, i200+j+2);

            double yj0_3 = mmat_get(y, i100+j+3);
            double zj0_3 = mmat_get(z, i100+j+3);
            double xj1_3 = mmat_get(x, i200+j+3);
            double zj1_3 = mmat_get(z, i200+j+3);

            double yj0_4 = mmat_get(y, i100+j+4);
            double zj0_4 = mmat_get(z, i100+j+4);
            double xj1_4 = mmat_get(x, i200+j+4);
            double zj1_4 = mmat_get(z, i200+j+4);

            double yj0_5 = mmat_get(y, i100+j+5);
            double zj0_5 = mmat_get(z, i100+j+5);
            double xj1_5 = mmat_get(x, i200+j+5);
            double zj1_5 = mmat_get(z, i200+j+5);


            t10_0 = zj0_0 / (y00_0 * y0j60_0) + yj0_0;
            t11_0 = zj1_0 / cos(CM_PI * j) + xj1_0;
            
            t10_1 = zj0_1 / (y00_0 * y0j60_1) + yj0_1;
            t11_1 = zj1_1 / cos(CM_PI * (j+1)) + xj1_1;

            t10_2 = zj0_2 / (y00_0 * y0j60_2) + yj0_2;
            t11_2 = zj1_2 / cos(CM_PI * (j+2)) + xj1_2;

            t10_3 = zj0_3 / (y00_0 * y0j60_3) + yj0_3;
            t11_3 = zj1_3 / cos(CM_PI * (j+3)) + xj1_3;

            t10_4 = zj0_4 / (y00_0 * y0j60_4) + yj0_4;
            t11_4 = zj1_4 / cos(CM_PI * (j+4)) + xj1_4;

            t10_5 = zj0_5 / (y00_0 * y0j60_5) + yj0_5;
            t11_5 = zj1_5 / cos(CM_PI * (j+5)) + xj1_5;

            

            mmat_set(z, i100+j, t10_0 * x0j30_0);
            mmat_set(z, i200+j, t11_0 * x0j30_0);

            mmat_set(z, i100+(j+1), t10_1 * x0j30_1);
            mmat_set(z, i200+(j+1), t11_1 * x0j30_1);

            mmat_set(z, i100+(j+2), t10_2 * x0j30_2);
            mmat_set(z, i200+(j+2), t11_2 * x0j30_2);

            mmat_set(z, i100+(j+3), t10_3 * x0j30_0);
            mmat_set(z, i200+(j+3), t11_3 * x0j30_0);

            mmat_set(z, i100+(j+4), t10_4 * x0j30_1);
            mmat_set(z, i200+(j+4), t11_4 * x0j30_1);

            mmat_set(z, i100+(j+5), t10_5 * x0j30_2);
            mmat_set(z, i200+(j+5), t11_5 * x0j30_2);
        }
        j = j > 100 ? j-6 : j;
        for (; j < 100; j+=4) {
            double x0j30_0 = cos(mmat_get(x, 0+j % 3));
            double y0j60_0 = sqrt(mmat_get(x, 0+j % 6));

            double x0j30_1 = cos(mmat_get(x, 0+(j+1) % 3));
            double y0j60_1 = sqrt(mmat_get(x, 0+(j+1) % 6));
            
            double x0j30_2 = cos(mmat_get(x, 0+(j+2) % 3));
            double y0j60_2 = sqrt(mmat_get(x, 0+(j+2) % 6));
            
            // double x0j30_0 = cos(mmat_get(x, 0+(j+3) % 3));
            double y0j60_3 = sqrt(mmat_get(x, 0+(j+3) % 6));
            
            
            double yj0_0 = mmat_get(y, i100+j);
            double zj0_0 = mmat_get(z, i100+j);
            double xj1_0 = mmat_get(x, i200+j);
            double zj1_0 = mmat_get(z, i200+j);

            double yj0_1 = mmat_get(y, i100+j+1);
            double zj0_1 = mmat_get(z, i100+j+1);
            double xj1_1 = mmat_get(x, i200+j+1);
            double zj1_1 = mmat_get(z, i200+j+1);

            double yj0_2 = mmat_get(y, i100+j+2);
            double zj0_2 = mmat_get(z, i100+j+2);
            double xj1_2 = mmat_get(x, i200+j+2);
            double zj1_2 = mmat_get(z, i200+j+2);

            double yj0_3 = mmat_get(y, i100+j+3);
            double zj0_3 = mmat_get(z, i100+j+3);
            double xj1_3 = mmat_get(x, i200+j+3);
            double zj1_3 = mmat_get(z, i200+j+3);


            t10_0 = zj0_0 / (y00_0 * y0j60_0) + yj0_0;
            t11_0 = zj1_0 / cos(CM_PI * j) + xj1_0;
            
            t10_1 = zj0_1 / (y00_0 * y0j60_1) + yj0_1;
            t11_1 = zj1_1 / cos(CM_PI * (j+1)) + xj1_1;

            t10_2 = zj0_2 / (y00_0 * y0j60_2) + yj0_2;
            t11_2 = zj1_2 / cos(CM_PI * (j+2)) + xj1_2;

            t10_3 = zj0_3 / (y00_0 * y0j60_3) + yj0_3;
            t11_3 = zj1_3 / cos(CM_PI * (j+3)) + xj1_3;

            

            mmat_set(z, i100+j, t10_0 * x0j30_0);
            mmat_set(z, i200+j, t11_0 * x0j30_0);

            mmat_set(z, i100+(j+1), t10_1 * x0j30_1);
            mmat_set(z, i200+(j+1), t11_1 * x0j30_1);

            mmat_set(z, i100+(j+2), t10_2 * x0j30_2);
            mmat_set(z, i200+(j+2), t11_2 * x0j30_2);

            mmat_set(z, i100+(j+3), t10_3 * x0j30_0);
            mmat_set(z, i200+(j+3), t11_3 * x0j30_0);
        }
    }
    for (int i = 0; i < 100; i++) {
        int i100 = i * 100;
        for (int j = 0; j < 100; j++) {
            mmat_activate(z, i100+j, ALPHA);
        }
    }
}


void maxperformance(mat* x, mat* y, mat* z) {
  b2(x,y,z);
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() {
  add_function(&slow_performance1, "slow_performance1",1);
  add_function(&maxperformance, "maxperformance",1);
}