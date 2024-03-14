#include "include/microbenchmark.h"
#include "include/tsc_x86.h"
#include "include/foo.h"
#include <stdio.h>

static volatile myInt64 NUM_ITERATIONS;
static volatile myInt64 NUM_ITERATIONS2;
static volatile int a;
static volatile int b;
static volatile int result;
static volatile int result2;
static volatile int result3;
static volatile int result4;
static volatile int result5;
static volatile myInt64 start;
static volatile myInt64 end;
static volatile myInt64 startLoop;
static volatile myInt64 endLoop;

void initialize_microbenchmark_data (microbenchmark_mode_t mode) {
    /* You can use to initialize some data if needed */
    switch (mode) {
        case ADD_LAT:
            NUM_ITERATIONS = 60;
            NUM_ITERATIONS2 = 2;
            a = 1;
            b = 1;
            result = 0;
            result2 = 0;
            result3 = 0;
            break;
        case ADD_REC_TP:
            NUM_ITERATIONS = 65;
            NUM_ITERATIONS2 = 2;
            a = 1;
            b = 1;
            result = 0;
            result2 = 0;
            result3 = 0;
            break;
        case SQT_LAT:
        case SQT_REC_TP:
        case SQT_LAT_MIN:
        case SQT_REC_TP_MIN:
        case FOO_LAT:
        case FOO_REC_TP:
        case FOO_LAT_MIN:
        case FOO_REC_TP_MIN:
        default: break;
    }
}


double microbenchmark_get_add_latency() {
    /* Implement your microbenchmark benchmark here */
    // int n;
    // for(n=0; n<NUM_ITERATIONS; n++) {
    //     result = a + result2;
    //     result2 = a + result3;
    // }
    // start = start_tsc();
    // for(n=0; n<NUM_ITERATIONS; n+=2) {
    //     result = a + result2;
    //     result2 = a + result3;
    // }
    // end = stop_tsc(start);
    
    // WARMUP
    int n;
    for(n=0; n<NUM_ITERATIONS; n++) {
        result = a + result2;
        result2 = a + result3;
    }
    // Measure looping overhead
    startLoop = start_tsc();
    for(n=0; n<NUM_ITERATIONS; n++) {
        result = a + result2;
    }
    endLoop = stop_tsc(startLoop);
    
    // Benchmarking
    start = start_tsc();
    for(n=0; n<NUM_ITERATIONS; n++) {
        result = a + result2;
        result2 = result + result3;
    }
    end = stop_tsc(start);
    double endtime = ((double)(end-endLoop)/(NUM_ITERATIONS*2));
    
    printf("endLoop = %d, end=%d, endtime =%f\n", endLoop, end, endtime);
    return endtime;
}

double microbenchmark_get_add_rec_tp() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}

double microbenchmark_get_sqt_latency() {
        int n;
    for(n=0; n<NUM_ITERATIONS; n++) {
        result = sqrt(result2);
        result2 = sqrt(result3);
    }
    // Measure looping overhead
    startLoop = start_tsc();
    for(n=0; n<NUM_ITERATIONS; n++) {
        result = sqrt(result2);
    }
    endLoop = stop_tsc(startLoop);
    
    // Benchmarking
    start = start_tsc();
    for(n=0; n<NUM_ITERATIONS; n++) {
        result = sqrt(result2);
        result2 = sqrt(result3);
    }
    end = stop_tsc(start);
    double endtime = ((double)(end-endLoop)/(NUM_ITERATIONS*2));
    
    printf("endLoop = %d, end=%d, endtime =%f\n", endLoop, end, endtime);
    return endtime;
}

double microbenchmark_get_sqt_rec_tp() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}

double microbenchmark_get_foo_latency() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}

double microbenchmark_get_foo_rec_tp() {
    /* Implement your microbenchmark benchmark here */
    return 0;
}