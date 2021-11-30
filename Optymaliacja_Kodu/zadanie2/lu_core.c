#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
// #include <x86intrin.h>
// #include <immintrin.h>

#define IDX(i, j, n) (((j)+ (i)*(n)))
int SIZE = 1000;
static double gtod_ref_time_sec = 0.0;



int LUPDecompose(double *A, int N) {

    int i, j, k; 

    for (i = 0; i < N; i++) {
        for (j = i + 1; j < N; j++) {
            A[IDX(j,i,SIZE)] /= A[IDX(i,i,SIZE)];

            for (k = i + 1; k < N; k++)
                A[IDX(j,k,SIZE)] -= A[IDX(j,i,SIZE)] * A[IDX(i,k,SIZE)];
        }
    }

    return 0;  //decomposition done 
}

double dclock()
{
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (gtod_ref_time_sec == 0.0)
        gtod_ref_time_sec = (double)tv.tv_sec;
    norm_sec = (double)tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;
    return the_time;
}

int main(){
    int i,j, iret;

    double dtime;

    double* matrix;

    matrix = malloc(SIZE*SIZE*sizeof(double));

    srand(1);

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            matrix[IDX(i, j, SIZE)] = rand();
        }
    }

    printf("call LU-decomposition\n");
    dtime = dclock();
    iret = LUPDecompose(matrix, SIZE);
    dtime = dclock() - dtime;
    printf("Time: %le \n", dtime);

    double check = 0.0;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            check = check + matrix[IDX(i,j,SIZE)];
        }
    }
    printf("Check-> rv: %d, sum:%le \n", iret, check);
    fflush(stdout);

    FILE *out_file = fopen("wyniki.txt", "a");
    fprintf(out_file, "%le,%s\n", dtime, "core");
    fclose(out_file);
}