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
            for(int j=1; j<length-1; j+=8)
            {
                for(int k = j; k < j + 8 && k < length - 1; k++) {
                    if ( ((i == length/2-1) || (i== length/2))
                         && ((k == length/2-1) || (k == length/2)) )
                        continue;

                    OUTPUT(i,k) = (INPUT(i-1,k-1) + INPUT(i-1,k) + INPUT(i-1,k+1) +
                                   INPUT(i,k-1)   + INPUT(i,k)   + INPUT(i,k+1)   +
                                   INPUT(i+1,k-1) + INPUT(i+1,k) + INPUT(i+1,k+1) )/9;
                }

            }
        }

        temp = input;
        input = output;
        output = temp;
    }
    }
}
