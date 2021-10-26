#include <stdio.h>
#include <stdlib.h>
#define SIZE 500
#include <sys/time.h>
#include <time.h>
static double gtod_ref_time_sec = 0.0;
/* Adapted from the bl2_clock() routine in the
BLIS library from Robert van de Geijn*/
double dclock()
{
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (gtod_ref_time_sec == 0.0)
        gtod_ref_time_sec = (double)tv.tv_sec;
    norm_sec =
        (double)tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;
    return the_time;
}
int mm(double **first, double **second, double **multiply)
{
    int i, j, k;
    double sum = 0;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            for (k = 0; k < SIZE; k++)
            {
                sum = sum + first[i][k] * second[k][j];
            }
            multiply[i][j] = sum;
            sum = 0;
        }
    }
    return 0;
}
int main(int argc, const char *argv[])
{
    int i, j, iret;
    double dtime;

    // double * multiply_;
    // double dtime;
    // SIZE = 1000;
    double *first_, *second_, *multiply_;
    double **first, **second, **multiply;

    first_ = (double*) malloc(SIZE*SIZE*sizeof(double));
    second_ = (double*) malloc(SIZE*SIZE*sizeof(double));
    multiply_ = (double*) malloc(SIZE*SIZE*sizeof(double));

    first = (double**) malloc(SIZE*sizeof(double));
    second = (double**) malloc(SIZE*sizeof(double));
    multiply = (double**) malloc(SIZE*sizeof(double));

    for(i=0;i<SIZE;i++){
        first[i] = first_ + i*SIZE;
        second[i] = second_ + i*SIZE;
        multiply[i] = multiply_ + i*SIZE;
    }
    
    for (i = 0; i < SIZE; i++)
    { //rows in first
        for (j = 0; j < SIZE; j++)
        { //columns in first
            first[i][j] = i + j;
            second[i][j] = i - j;
        }
    }

    dtime = dclock();
    iret = mm(first, second, multiply);
    dtime = dclock() - dtime;
    printf("Time: %le \n", dtime);
    fflush(stdout);
    return iret;
}