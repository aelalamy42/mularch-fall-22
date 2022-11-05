/*
============================================================================
Filename    : algorithm.c
Author      : Ahmed Elalamy
SCIPER      : 324610

============================================================================
*/

#define INPUT(I,J) input[(I)*length+(J)]
#define OUTPUT(I,J) output[(I)*length+(J)]

void simulate(double *input, double *output, int threads, int length, int iterations)
{
    double *temp;
    omp_set_num_threads(threads);
    // Parallelize this!!
    int n, i, j;
#pragma omp parallel private(n,i,j)
    {
    for (n = 0; n < iterations; n++) {
    #pragma omp for
        for (i = 1; i < length - 1 ; i++) {
            for (j = 1; j < length - 1; j+=2) {
                if (((i == length / 2 - 1) || (i == length / 2))
                    && (j == length / 2 - 1))
                    continue;


                OUTPUT(i, j) = (INPUT(i - 1, j - 1) + INPUT(i - 1, j) + INPUT(i - 1, j + 1) +
                                INPUT(i, j - 1) + INPUT(i, j) + INPUT(i, j + 1) +
                                INPUT(i + 1, j - 1) + INPUT(i + 1, j) + INPUT(i + 1, j + 1)) / 9;

                 OUTPUT(i, j + 1) = (INPUT(i - 1, j) + INPUT(i - 1, j + 1) + INPUT(i - 1, j + 2) +
                                 INPUT(i, j) + INPUT(i, j + 1) + INPUT(i, j + 2) +
                                 INPUT(i + 1, j) + INPUT(i + 1, j + 1) + INPUT(i + 1, j + 2)) / 9;


            }
        }

        temp = input;
        input = output;
        output = temp;
    }
    }
}

