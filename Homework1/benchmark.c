#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

double run(int argc, char **argv);

char *intToString(int n) {
    int length = snprintf(NULL, 0, "%d", n);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", n);
    return str;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <benchmark_name> <output_file>\n", argv[0]);
        return 1;
    }

    char *args[2];
    args[0] = argv[0];

    if (strcmp(argv[1], "ex2c_b1") == 0 || strcmp(argv[1], "ex2c_b2") == 0 ||
        strcmp(argv[1], "ex2c_b3") == 0) {
        const int REPEAT = 10;
        FILE *file = fopen(argv[2], "w");
        for (int n = 100; n <= 2000; n += 100) {
        // for (int n = 100; n <= 200; n += 100) {
            args[1] = intToString(n);
            fprintf(file, "%d ", n);
            for (int i = 1; i <= REPEAT; i++) {
                double cycles = run(2, args);
                printf("\r[%d/%d] n = %d, seconds = %.20f", i, REPEAT, n, cycles);
                fflush(stdout);
                fprintf(file, "%.20f ", cycles);   
            }
            printf("\n");
            fprintf(file, "\n");
            free(args[1]);
        }
        fclose(file);
    } else if (strcmp(argv[1], "ex4a_b1") == 0 ||
               strcmp(argv[1], "ex4c_b1") == 0) {
        const int REPEAT = 30;
        FILE *file = fopen(argv[2], "w");
        int n = 1 << 3;
        for (int ii = 4; ii <= 28; ii += 1) {
            n = n << 1;
            args[1] = intToString(n);
            fprintf(file, "%d ", n);
            for (int i = 1; i <= REPEAT; i++) {
                double cycles = run(2, args);
                printf("\r[%d/%d] n = %d, seconds = %.20f", i, REPEAT, n, cycles);
                fprintf(file, "%.20f ", cycles);
                fflush(stdout);
                fflush(file);
            }
            printf("\n");
            fprintf(file, "\n");
            free(args[1]);
        }
        fclose(file);
    } else {
        printf("Benchmark '%s' not found\n", argv[1]);
        return 1;
    }

    return 0;
}
