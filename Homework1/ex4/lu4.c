//#error Please comment out the next two lines under linux, then comment this error
//#include "stdafx.h"  //Visual studio expects this line to be the first one, comment out if different compiler
//#include <windows.h> // Include if under windows

#ifndef WIN32
#include <sys/time.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


#ifdef __x86_64__
#include "tsc_x86.h"
#endif

#ifdef TEST
#include <assert.h>
#endif

#ifdef EXTERNAL
#define printf(fmt, ...) (0)
#endif


#define NUM_RUNS 5
#define CYCLES_REQUIRED 1e8
#define FREQUENCY 2.53e9 // TODO: Say disclaimer about calculated frequency
#define CALIBRATE




/*
 *	Initialize the input
 */

void fill_matrix(double * A, int n) {
    for(int i=0; i < n; i++) {
        for(int j=0; j < n; j++) {
            A[n*i+j] = (double) rand() / RAND_MAX;
        }
        A[i*n+i] += 1;
    }
}

void fill_identity(double * A, int n) {
    memset(A, 0, sizeof(double) * n*n);
    for(int i=0; i < n; i++) {
        A[i*n+i] = 1.0;
    }
}

void fill_vector(double * x, int n) {
    for(int i=0; i < n; i++) {
        x[i] = (double) rand() / RAND_MAX;
    }
}


void compute(double *x, double *y, double *z, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 2; k++) {
            z[k] += x[i + 1 - k] * y[i + k];
        }
    }
}

/*
 * Timing function based on the TimeStep Counter of the CPU.
 */
#ifdef __x86_64__
double rdtsc(double *x, double *y, double *z, int n) {
    int i, num_runs;
    myInt64 cycles = 0;
    myInt64 start;
    double *z2 = (double *)malloc(2*sizeof(double));

    num_runs = NUM_RUNS;
    /*
     * The CPUID instruction serializes the pipeline.
     * Using it, we can create execution barriers around the code we want to time.
     * The calibrate section is used to make the computation large enough so as to
     * avoid measurements bias due to the timing overhead.
     */
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        for (i = 0; i < num_runs; ++i) {
            memcpy(z2, z, sizeof(double) * 2);
            start = start_tsc();
            compute(x,y,z2,n);
            cycles += stop_tsc(start);
        }

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    cycles = 0;
    for (i = 0; i < num_runs; ++i) {
        memcpy(z2, z, sizeof(double) * 2);
        start = start_tsc();
        compute(x,y,z2,n);
        cycles += stop_tsc(start);
    }

    free(z2);
    cycles = cycles/num_runs;
    return (double) cycles;
}
#endif

double c_clock(double *x, double *y, double *z, int n) {
    int i, num_runs;
    double cycles = 0;
    clock_t start, end;
    double *z2 = (double *)malloc(2*sizeof(double));

    if (z2 == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        for (i = 0; i < num_runs; ++i) {
            memcpy(z2, z, sizeof(double) * 2);
            start = clock();
            compute(x,y,z2,n);
            end = clock();
            cycles += (double)(end - start);
        }

        // Same as in c_clock: CYCLES_REQUIRED should be expressed accordingly to the order of magnitude of CLOCKS_PER_SEC
        if(cycles >= CYCLES_REQUIRED/(FREQUENCY/CLOCKS_PER_SEC)) break;

        num_runs *= 2;
    }
#endif
    cycles = 0;
    for(i=0; i<num_runs; ++i) {
        memcpy(z2, z, sizeof(double) * 2);
        start = clock();
        compute(x,y,z2,n);
        end = clock();
        cycles += (double)(end - start);
    }
    free(z2);
    return (cycles)/num_runs;
}

#ifndef WIN32
double timeofday(double *x, double *y, double *z, int n) {
    int i, num_runs;
    double cycles = 0;
    struct timeval start, end;
    double *z2 = (double *)malloc(2*sizeof(double));

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        for (i = 0; i < num_runs; ++i) {
            memcpy(z2, z, sizeof(double) * 2);
            gettimeofday(&start, NULL);
            compute(x,y,z2,n);
            gettimeofday(&end, NULL);
            cycles += (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6)*FREQUENCY;
        }

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    double seconds = 0;
    for(i=0; i < num_runs; ++i) {
        memcpy(z2, z, sizeof(double) * 2);
        gettimeofday(&start, NULL);
        compute(x,y,z2,n);
        gettimeofday(&end, NULL);
        seconds += (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6);
    }
    free(z2);
    return seconds/num_runs;
}

#else

double gettickcount(double *x, double *y, double *z, int n) {
    int i, num_runs;
    double cycles, start, end;
    double *z2 = (double *)malloc(2*sizeof(double));

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {

        for (i = 0; i < num_runs; ++i) {
            memcpy(z2, z, sizeof(double) * 2);
            start = (double)GetTickCount();
            compute(x,y,z2,n);
            end = (double)GetTickCount();
            cycles += (double)(end - start);
        }

        cycles = cycles*FREQUENCY/1e3; // end-start provides a measurement in the order of milliseconds

        if(cycles >= CYCLES_REQUIRED) break;

        num_runs *= 2;
    }
#endif

    cycles = 0;
    for(i=0; i < num_runs; ++i) {
        memcpy(z2, z, sizeof(double) * 2);
        start = (double)GetTickCount();
        compute(x,y,z2,n);
        end = (double)GetTickCount();
        cycles += (double)(end - start);
    }

    free(z2);
    return cycles/num_runs;
}

double queryperfcounter(double *x, double *y, double *z, int n, LARGE_INTEGER f) {
    int i, num_runs;
    double cycles = 0;
    LARGE_INTEGER start, end;
    double *z2 = (double *)malloc(2*sizeof(double));

    num_runs = NUM_RUNS;
#ifdef CALIBRATE
    while(num_runs < (1 << 14)) {
        for (i = 0; i < num_runs; ++i) {
            memcpy(z2, z, sizeof(double) * 2);
            QueryPerformanceCounter(&start);
            compute(x,y,z2,n);
            QueryPerformanceCounter(&end);
            cycles += (double)(end.QuadPart - start.QuadPart);
        }

        // Same as in c_clock: CYCLES_REQUIRED should be expressed accordingly to the order of magnitude of f
        if(cycles >= CYCLES_REQUIRED/(FREQUENCY/f.QuadPart)) break;

        num_runs *= 2;
    }
#endif

    cycles = 0;
    for(i=0; i < num_runs; ++i) {
		memcpy(z2, z, sizeof(double) * 2);
        QueryPerformanceCounter(&start);
        compute(x,y,z2,n);
        QueryPerformanceCounter(&end);
        cycles += (double)(end.QuadPart - start.QuadPart);
    }
    free(z2);
    return cycles/num_runs;
}

#endif

#ifndef EXTERNAL
int main(int argc, char **argv) {
#else
double run(int argc, char **argv) {
#endif
    printf("Benchmarking '%s'\n", __FILE__);

    if (argc!=2) {printf("usage: FW <n>\n"); return -1;}
    int n = atoi(argv[1]);
    printf("n=%d \n",n);

    double* x = (double *)malloc((n+1)*sizeof(double));
    double* y = (double *)malloc((n+1)*sizeof(double));
    double* z = (double *)malloc(2*sizeof(double));

    if (x == NULL || y == NULL || z == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    fill_vector(x, n+1);
    fill_vector(y, n+1);
    fill_vector(z, 2);

#ifdef __x86_64__
    double r = rdtsc(x,y,z,n);
    printf("RDTSC instruction:\n %lf cycles measured => %lf seconds, assuming frequency is %lf MHz. (change in source file if different)\n\n", r, r/(FREQUENCY), (FREQUENCY)/1e6);
#endif

    double c = c_clock(x,y,z,n);

    printf("C clock() function:\n %lf cycles measured. On some systems, this number seems to be actually computed "
           "from a timer in seconds then transformed into clock ticks using the variable CLOCKS_PER_SEC. Unfortunately, "
           "it appears that CLOCKS_PER_SEC is sometimes set improperly. (According to this variable, your computer should "
           "be running at %lf MHz). In any case, dividing by this value should give a correct timing: %lf seconds. \n\n",c, (double) CLOCKS_PER_SEC/1e6, c/CLOCKS_PER_SEC);
    

#ifndef WIN32
    double t = timeofday(x,y,z,n);
    printf("C gettimeofday() function:\n %.20f seconds measured\n\n",t);
#else
    LARGE_INTEGER f;
    double t = gettickcount(x,y,z,n);
    printf("Windows getTickCount() function:\n %lf milliseconds measured\n\n",t);

    QueryPerformanceFrequency((LARGE_INTEGER *)&f);

    double p = queryperfcounter(x,y,z,n, f);
    printf("Windows QueryPerformanceCounter() function:\n %lf cycles measured => %lf seconds, with reported CPU frequency %lf MHz\n\n",p,p/f.QuadPart,(double)f.QuadPart/1000);
#endif    

    free(x);
    free(y);
    free(z);
    #ifndef EXTERNAL
    return 0;
    #else
    return t;
    #endif
}

