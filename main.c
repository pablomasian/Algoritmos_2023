#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

void init_seed(){  /* set the seed of a new sequence of pseudo-random integers */
    srand(time(NULL));
}

void random_init(int v [], int n){  /* generate pseudo-random numbers between -n and +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

double microseconds(){  /* obtains the system time in microseconds */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int maxSubSum1(int v[], int n){  /* first algorithm */
    int MaxSum = 0;
    for (int i = 0; i < n; i++) {
        int ThisSum = 0;
        for (int j = i; j < n; j++) {
            ThisSum = ThisSum + v[j];
            if (ThisSum > MaxSum) {
                MaxSum = ThisSum;
            }
        }
    }
    return MaxSum;
}

int maxSubSum2(int v[], int n){  /* second algorithm */
    int ThisSum = 0, MaxSum = 0;
    for (int j = 0; j < n; j++) {
        ThisSum = ThisSum + v[j];
        if (ThisSum > MaxSum) {
            MaxSum = ThisSum;
        } else if (ThisSum < 0) {
            ThisSum = 0;
        }
    }
    return MaxSum;
}

void print_array(int v[], int n){  /* prints the array */
    printf("[");
    for (int i=0; i<n; i++)
    {
        printf("%d ", v[i]);
        printf("\t");
    }
    printf("\b\b\b\b\b\b]");
}

void test1(){  /* proves that first algorithm works */
    int i, a, b;
    int v[6][5]={
            {-9,2,-5,-4,6},
            {4,0,9,2,5},
            {-2,-1,-9,-7,-1},
            {9,-2,1,-7,8},
            {15,-2,-5,-4,16},
            {7,-5,6,7,-7}
    };
    printf("\nTest Algorithm 1\n");
    printf("%40s%15s%15s\n","","maxSubSum1","maxSubSum2");
    for (i=0; i<6; i++){ //Inside each array
        print_array(v[i],5);
        a=maxSubSum1(v[i],5);
        b=maxSubSum2(v[i],5);
        printf("%15d%15d\n",a,b);
    }
}

void test2(){  /* proves that second algorithm works */
    int i, a, b;
    int v[9];
    printf("\n Test Algorithm2\n");
    printf("%92s%15s\n", "maxSubSum1", "maxSubSum2");
    for (i=0; i<10; i++) {
        random_init(v, 9);
        print_array(v, 9);
        a = maxSubSum1(v, 9);
        b = maxSubSum2(v, 9);
        printf(" %20d %15d\n", a, b);
    }
}

void print_header(double sub, double aj, double sup){  /* prints the header of the chart of times */
    printf("\n\t\t\t"
           "\tUnderestimed Cote"
           "\tAdjusted Cote"
           "\tOverestimed Cote\n");
    printf("\t\tn"
           "\tt(n)"
           "\t\tt(n)/n^%1.1lf"
           "\tt(n)/n^%1.1lf"
           "\tt(n)/n^%1.1lf\n\n", sub, aj, sup);
}

double Algorithm_time(int vector[], int n, int (func)(int[], int)){  /* calculates the time the algorithm needs to run */
    double ta,tb,t,t1,t2;
    int k=10000;
    int count;

    random_init(vector,n);

    ta=microseconds();
    func(vector, n);
    tb=microseconds();
    t=tb-ta;

    if(t<500){
        ta=microseconds();
        for(count=0;count<k;count++){
            func(vector, n);
        }
        tb=microseconds();
        t1=tb-ta;

        ta=microseconds();
        for(count=0;count<k;count++) {
        }
        tb=microseconds();
        t2=tb-ta;
        t=(t1-t2)/k;
        printf("(*)\t");
    }else printf("\t");
    return t;
}

void print_chart (int (funct)(int [], int), double SubCote, double AjCote, double SobCote){  /* prints the chart of times */
    double t;
    double tsub, taj, tsob;
    int vector [32000];
    int n;

    printf("\n");
    print_header(SubCote, AjCote, SobCote);
    for (n = 500; n <= 32000; n*=2)
    {
        t = Algorithm_time(vector, n, funct);

        tsub=t/pow(n,SubCote);
        taj=t/pow(n,AjCote);
        tsob=t/pow(n,SobCote);
        printf("%6d%16.3f%18.10f%20.10f%18.10f\n", n, t, tsub, taj, tsob);
    }
}

int main()
{
    init_seed();

    test1();
    test2();

    printf("\nFirst Algorithm: ");
    print_chart(maxSubSum1, 1.8, 2, 2.2);

    printf("\nSecond Algorithm: ");
    print_chart(maxSubSum2, 0.8, 1, 1.2);
    return 0;
}