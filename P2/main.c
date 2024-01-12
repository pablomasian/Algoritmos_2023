#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

void inicializar_semilla ()
{
    srand(time(NULL));
}

void randm (int v[] , int n)
{
    int i, m=2*n+1;
    for (i=0; i<n; i++)
    {
        v[i] = (rand() % m) - n;
    }
}

double microseconds ()   /* obtains the system hour in microseconds */
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
    {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void ins_sort (int v[], int n)
{
    int i, x, j;
    for (i=1; i<n; i++)
    {
        x = v[i];
        j = i-1;
        while ((j >= 0) & (v[j] > x))
        {
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1] = x;
    }
}

void shell_sort (int v[], int n)
{
    int increment = n;
    int i, j, tmp;
    do
    {
        increment = increment / 2;
        for (i = increment; i < n; i++)
        {
            tmp = v[i];
            j = i;
            int keepgoing = 1;
            while (j - increment >= 0 && keepgoing)
            {
                if (tmp < v[j - increment])
                {
                    v[j] = v[j - increment];
                    j = j - increment;
                }
                else
                {
                    keepgoing = 0;
                }
            }
            v[j] = tmp;
        }
    }
    while (increment != 1);
}

void descending (int v[] , int n)
{
    int i, j=0;
    for (i=n; i>0; i--)
    {
        v[i-1] = j;
        j++;  //CAMBIO
    }
}

void ascending (int v[] , int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        v[i] = i;
    }
}

double Algorithm_time (int vector[], int n, void (*func)(int[] , int), void (*funGen)(int[] , int))
{
    double ta, tb, t, t1, t2;
    int k=1000;
    int count;

    funGen (vector, n);

    ta=microseconds ();
    func (vector, n);
    tb=microseconds ();
    t=tb-ta;

    if (t<500)
    {
        ta=microseconds ();
        for(count=0; count<k; count++)
        {
            funGen (vector, n);
            func (vector, n);
        }
        tb=microseconds ();
        t1=tb-ta;

        ta=microseconds ();
        for(count=0; count<k; count++)
        {
            funGen (vector, n);
        }
        tb=microseconds ();
        t2=tb-ta;
        t=(t1-t2)/k;
        printf("(*)");
    }
    else
    {
        printf("   ");
    }
    return t;
}

void printChart (void (*func)(int[] , int), void (*funGen)(int[] , int), double SubCote, double AjCote, double SobCote, char * Sort)
//"Sort" represents the sorting algorithm
{
    double t;
    double tsub, taj, tsob;
    int vector [64000];
    int n;

    printf("\n");

    if (!strcmp(Sort, "ins")){                    //Insertion sort, all cases
        printf("%7s%16s%18s%.2f%15s%.2f%15s%.2f \n", "n", "t(n)", "t(n)/n^", SubCote, "t(n)/n^", AjCote, "t(n)/n^", SobCote);
        for (n = 500; n <= 64000; n*=2){
            t = Algorithm_time(vector, n, func, funGen);
            tsub=t/pow(n,SubCote);
            taj=t/pow(n,AjCote);
            tsob=t/pow(n,SobCote);
            printf("%6d%16.3f%18.10f%20.10f%18.10f\n", n, t, tsub, taj, tsob);
        }
    } else if (!strcmp(Sort, "sh")){            //Shell sort, all cases
        printf("%7s%16s%24s%.2f%21s%.2f%21s%.2f \n", "n", "t(n)", "t(n)/n*log(n)^", SubCote, "t(n)/n*log(n)^", AjCote, "t(n)/n*log(n)^", SobCote);
        for (n = 500; n <= 64000; n*=2){
            t = Algorithm_time(vector, n, func, funGen);
            tsub=t/(n*(pow(log(n), SubCote)));
            taj=t/(n*(pow(log(n), AjCote)));
            tsob=t/(n*(pow(log(n), SobCote)));
            printf("%6d%16.3f%24.10f%26.10f%24.10f\n", n, t, tsub, taj, tsob);
        }
    }
}

void printVector (int v[], int n)
{
    int i;
    printf("[");
    for (i=0; i<n;i++)
    {
        printf("%4d", v[i]);
    }
    printf(" ]\n");
}

void is_ordered (int v[], int n)
{
    int ascendent=1;
    int last = v[0];
    for(int i=1; i<n; i++)
    {
        if(v[i] < last)
        {
            ascendent=0;
        }
        last = v[i];
    }
    printf("Ordered?: %d\n", ascendent);
}

void testWithVector (void (*func)(int[] , int), void (*funGen)(int[] , int), int vector[], int n)
{
    funGen (vector, n);
    printf("Input: ");
    printVector (vector, n);
    is_ordered (vector, n);
    printf("Output:");
    func (vector, n);
    printVector (vector, n);
    is_ordered (vector, n);
}

void printRandomTests (void (*func)(int[] , int), int vector[], int n)
{
    int i;
    for (i=0; i<4; i++)
    {
        testWithVector (func, randm, vector, n);
        printf("\n");
    }
}

void printAscenDescen (void (*func)(int[] , int), int vector[], int n){
    testWithVector(func, descending, vector, n);
    printf("\n");
    testWithVector(func, ascending, vector, n);
    printf("\n");
}

void validate(void (*func)(int[] , int), int vector[], int n){
    printRandomTests(func, vector, n);
    printAscenDescen(func, vector, n);
}

void print_results(void (*func)(int[] , int), int vector[], int n,double AscCote,double DescCote, double RandCote, char * Sort){
    printf("\nOrdenation of ascending vector:");
    printChart(func, ascending, (AscCote - 0.2), AscCote, (AscCote + 0.2), Sort);
    printf("\nOrdenation of descending vector:");
    printChart(func, descending, (DescCote - 0.2), DescCote, (DescCote + 0.2), Sort);
    printf("\nOrdenation of random vector:");
    printChart(func, randm, (RandCote - 0.2), RandCote, (RandCote + 0.2), Sort);
}

int main (){
    int n=10;
    int v[n];
    inicializar_semilla ();
    printf("INSERTION SORT:\n\n");
    validate(ins_sort, v, n);
    print_results(ins_sort, v, n,1,2,2, "ins");
    printf("\n\nSHELL SORT:\n\n");
    validate(shell_sort, v, n);
    print_results(shell_sort, v, n,1.1,1.15,1.55, "sh");
    return 0;
}