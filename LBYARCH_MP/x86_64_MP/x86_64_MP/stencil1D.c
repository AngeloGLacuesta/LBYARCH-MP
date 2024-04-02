#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define num 10

void compute(int length, double* input, double* output) {
    int index;
    for (index = 3; index < length - 3; index++) {
        output[index] = input[index - 3] + input[index - 2] + input[index - 1] + input[index] + input[index + 1] + input[index + 2] + input[index + 3];
    }
}

double clockDuration(clock_t startTime, clock_t endTime) {
    return ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
}

extern void asmStencil(int length, double* input, double* output);

int main() {
    int dataSize[] = { 1 << 20, 1 << 24, 1 << 28 }; 
    int totalSize = sizeof(dataSize) / sizeof(dataSize[0]);
    int exponentSize[] = { 20, 24, 28 };
    int counter;
    int idx;
    double* inputData = (double*)malloc(dataSize[totalSize - 1] * sizeof(double));
    if (!inputData) {
        fprintf(stderr, "ERROR: Failed to allocate memory\n");
        return EXIT_FAILURE;
    }

    for (counter = 0; counter < dataSize[totalSize - 1]; counter++) {
        inputData[counter] = counter + 1;
    }

    printf("\n\t\t\t\t| Initial Values |\n");
    for (counter = 0; counter < num; counter++) {
        printf("\t\t\t\t inputData[%d] = %.2lf\n", counter, inputData[counter]);
    }

    printf("\n\t\t\t\t| Benchmark for Stencil Computations |\n");
    for (idx = 0; idx < totalSize; idx++) {
        int currentSize = dataSize[idx];
        double* cResult = (double*)malloc(currentSize * sizeof(double));
        double* asmResult = (double*)malloc(currentSize * sizeof(double));
        if (!cResult || !asmResult) {
            fprintf(stderr, "ERROR: Failed to allocate memory for result arrays\n");
            free(cResult);
            free(asmResult);
            continue; 
        }

        printf("\n--> Array Size 2^%d <--\n", exponentSize[idx]);

        clock_t clockC = clock();
        compute(currentSize, inputData, cResult);
        clock_t end_clockC = clock();
        double computationTimeC = clockDuration(clockC, end_clockC);
        clock_t clockASM = clock();
        asmStencil(currentSize, inputData, asmResult);
        clock_t end_clockASM = clock();
        double computationTimeASM = clockDuration(clockASM, end_clockASM);
        
        printf("\n\t\t\t\t| Calculated Values using C |\n");
        for (counter = 3; counter < num + 3 && counter < currentSize - 3; counter++) {
            printf("\t\t\t\t cResult[%d] = %.2lf\n", counter, cResult[counter]);
        }
        printf("\n\t\t| C Program Computation Time for Array Size %d: %lf seconds |\n", currentSize, computationTimeC);

        printf("\n\t\t\t\t| Calculated Values using Assembly |\n");
        for (counter = 3; counter < num + 3 && counter < currentSize - 3; counter++) {
            printf("\t\t asmResult[%d] = %.2lf\n", counter, asmResult[counter]);
        }
        printf("\n\t\t| Assembly Program Computation Time for Array Size %d: %lf seconds |\n", currentSize, computationTimeASM);

        free(cResult);
        free(asmResult);
    }

    free(inputData);

    return 0;
}
