#include "include/microbenchmark.h"

#include <stdio.h>

#include "include/foo.h"
#include "include/tsc_x86.h"

static myInt64 NUM_ITERATIONS;
static double result;
static myInt64 start;
static myInt64 end;

void initialize_microbenchmark_data(microbenchmark_mode_t mode) {
    /* You can use to initialize some data if needed */
    switch (mode) {
        case ADD_LAT:
            NUM_ITERATIONS = 1000000;
            result = 0;
            break;
        case ADD_REC_TP:
            NUM_ITERATIONS = 1000000;
            result = 0;
            break;
        case SQT_LAT:
            NUM_ITERATIONS = 30;
            result = 100;
            break;
        case SQT_REC_TP:
            NUM_ITERATIONS = 30;
            result = 100;
            break;
        case SQT_LAT_MIN:
            NUM_ITERATIONS = 500;
            result = -1;
            break;
        case SQT_REC_TP_MIN:
            NUM_ITERATIONS = 1000;
            result = -1;
            break;
        case FOO_LAT:
            NUM_ITERATIONS = 9;
            result = 100;
            break;
        case FOO_REC_TP:
            NUM_ITERATIONS = 7;
            result = 100;
            break;
        case FOO_LAT_MIN:
            NUM_ITERATIONS = 1000;
            result = -1;
            break;
        case FOO_REC_TP_MIN:
            NUM_ITERATIONS = 100000;
            result = -1;
            break;
        default:
            break;
    }
}

double microbenchmark_get_add_latency() {
    double add = rand();
    /* Implement your microbenchmark benchmark here */
    int n;
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result += add;
        result += add;
        result += add;
    }
    // start = start_tsc();
    // for(n=0; n<NUM_ITERATIONS; n+=2) {
    //     result = a + result2;
    //     result2 = a + result3;
    // }
    // end = stop_tsc(start);

    // WARMUP

    start = start_tsc();
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result += add;
    }
    end = stop_tsc(start);

    // printf("endLoop = %d, end=%d, endtime =%f\n", end, endtime);
    return (double)end / NUM_ITERATIONS;
}

double microbenchmark_get_add_rec_tp() {
    double add = rand();
    /* Implement your microbenchmark benchmark here */
    int n;
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result += add;
        result += add;
        result += add;
    }
    // start = start_tsc();
    // for(n=0; n<NUM_ITERATIONS; n+=2) {
    //     result = a + result2;
    //     result2 = a + result3;
    // }
    // end = stop_tsc(start);

    // WARMUP

    start = start_tsc();
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result += add;
    }
    end = stop_tsc(start);

    // printf("endLoop = %d, end=%d, endtime =%f\n", end, endtime);
    return (double)end / (NUM_ITERATIONS * 8);
}

double microbenchmark_get_sqt_latency() {
    // global double result = rand();
    /* Implement your microbenchmark benchmark here */

    // WARMUP
    int n;
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = sqrt(result);
    }

    start = start_tsc();
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = sqrt(result);
    }
    end = stop_tsc(start);

    return (double)end / NUM_ITERATIONS;
}

double microbenchmark_get_sqt_rec_tp() {
    // WARMUP
    int n;
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = sqrt(result);
    }

    start = start_tsc();
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = sqrt(result);
    }
    end = stop_tsc(start);

    return (double)end / (NUM_ITERATIONS * 3);
}

double microbenchmark_get_foo_latency() {
    // global double result = rand();
    /* Implement your microbenchmark benchmark here */

    // WARMUP
    int n;
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = foo(result);
    }

    start = start_tsc();
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = foo(result);
    }
    end = stop_tsc(start);

    return (double)end / NUM_ITERATIONS;
}

double microbenchmark_get_foo_rec_tp() {
    int n;
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = foo(result);
    }

    start = start_tsc();
    for (n = 0; n < NUM_ITERATIONS; n++) {
        result = foo(result);
    }
    end = stop_tsc(start);

    return (double)end / (NUM_ITERATIONS * 5);
}