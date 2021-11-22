//requires additional changes to the code to make it work

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <x86intrin.h>
// #include <emmintrin.h>

static double gtod_ref_time_sec = 0.0;
#define IDX(i, j, n) (((j)+ (i)*(n)))

/* Adapted from the bl2_clock() routine in the BLIS library */

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

int ge(double *A, int SIZE)
{
    register int i, j, k;
    register double multiplier;
    register __m128d tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    register __m128d mm_multiplier;

    for (k = 0; k < SIZE; k++)
    {
        for (i = k + 1; i < SIZE; i++)
        {
            multiplier = (A[IDX(i,k,SIZE)] / A[IDX(k,k,SIZE)]);
            mm_multiplier[0] = multiplier;
            mm_multiplier[1] = multiplier;
            for (j = k + 1; j < SIZE;)
            {
                if(j+7 < SIZE){
                    tmp0 = _mm_loadu_pd(A+IDX(k,j,SIZE));
                    tmp1 = _mm_loadu_pd(A+IDX(k,j+2,SIZE));
                    tmp2 = _mm_loadu_pd(A+IDX(k,j+4,SIZE));
                    tmp3 = _mm_loadu_pd(A+IDX(k,j+6,SIZE));

                    tmp4 = _mm_loadu_pd(A+IDX(i,j,SIZE));
                    tmp5 = _mm_loadu_pd(A+IDX(i,j+2,SIZE));
                    tmp6 = _mm_loadu_pd(A+IDX(i,j+4,SIZE));
                    tmp7 = _mm_loadu_pd(A+IDX(i,j+6,SIZE));

                    tmp0 = _mm_mul_pd(tmp0, mm_multiplier);
                    tmp1 = _mm_mul_pd(tmp1, mm_multiplier);
                    tmp2 = _mm_mul_pd(tmp2, mm_multiplier);
                    tmp3 = _mm_mul_pd(tmp3, mm_multiplier);

                    tmp0 = _mm_sub_pd(tmp4, tmp0);
                    tmp1 = _mm_sub_pd(tmp5, tmp1);
                    tmp2 = _mm_sub_pd(tmp6, tmp2);
                    tmp3 = _mm_sub_pd(tmp7, tmp3);

                    A[IDX(i, j, SIZE)] = tmp0[0];
                    A[IDX(i, j+1, SIZE)] = tmp0[1];
                    A[IDX(i, j+2, SIZE)] = tmp1[0];
                    A[IDX(i, j+3, SIZE)] = tmp1[1];
                    A[IDX(i, j+4, SIZE)] = tmp2[0];
                    A[IDX(i, j+5, SIZE)] = tmp2[1];
                    A[IDX(i, j+6, SIZE)] = tmp3[0];
                    A[IDX(i, j+7, SIZE)] = tmp3[1];

                    // A[IDX(i,j,SIZE)] = A[IDX(i,j,SIZE)] - A[IDX(k,j,SIZE)] * multiplier;
                    // A[IDX(i,j+1,SIZE)] = A[IDX(i,j+1,SIZE)] - A[IDX(k,j+1,SIZE)] * multiplier;
                    // A[IDX(i,j+2,SIZE)] = A[IDX(i,j+2,SIZE)] - A[IDX(k,j+2,SIZE)] * multiplier;
                    // A[IDX(i,j+3,SIZE)] = A[IDX(i,j+3,SIZE)] - A[IDX(k,j+3,SIZE)] * multiplier;
                    // A[IDX(i,j+4,SIZE)] = A[IDX(i,j+4,SIZE)] - A[IDX(k,j+4,SIZE)] * multiplier;
                    // A[IDX(i,j+5,SIZE)] = A[IDX(i,j+5,SIZE)] - A[IDX(k,j+5,SIZE)] * multiplier;
                    // A[IDX(i,j+6,SIZE)] = A[IDX(i,j+6,SIZE)] - A[IDX(k,j+6,SIZE)] * multiplier; 
                    // A[IDX(i,j+7,SIZE)] = A[IDX(i,j+7,SIZE)] - A[IDX(k,j+7,SIZE)] * multiplier; 
                    j+=8;
                }else{
                    A[IDX(i,j,SIZE)] = A[IDX(i,j,SIZE)] - A[IDX(k,j,SIZE)] * multiplier;
                    j++;
                }
            }
        }
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    int i, j, k, iret;
    double dtime;
    int SIZE = 800;

    // double *matrix_;
    // double **matrix;

    // matrix_ = (double *)malloc(SIZE * SIZE * sizeof(double));
    // matrix = (double **)malloc(SIZE * sizeof(double *));

    // for (i = 0; i < SIZE; i++)
    // {
    //     matrix[i] = matrix_ + SIZE * i;
    // }

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
    printf("call GE");
    dtime = dclock();
    iret = ge(matrix, SIZE);
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
    printf("Check: %le \n", check);
    fflush(stdout);

    return iret;
}
