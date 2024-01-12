#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>

#define SZ 256000
struct heap{
    int last;
    int vector[SZ];
};

typedef struct heap *pheap;

double microseconds ()    // obtains the system hour in microseconds
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
    {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void swap(pheap h, int i, int j)
{
    int aux = h->vector[i];
    h->vector[i] = h->vector[j];
    h->vector[j] = aux;
}

void percolate_down(int i, pheap h){
    int LeftChild;
    int RightChild;
    int minor;
    while (i < h->last){
        LeftChild = 2 * i + 1;
        RightChild = 2 * i + 2;
        if (h->vector[LeftChild] < h->vector[RightChild]){
            minor = LeftChild;              //If LeftChild is smaller than RightChild, "minor" positions at LeftChild. And viceversa
        } else {
            minor = RightChild;
        }
        if (h->vector[minor] < h->vector[i]){
            swap (h, minor, i);
            i = minor;
        } else {
            break;
        }
    }
}

void create_heap(const int v[], int n, pheap h)
{
    h->last=-1;

    for(int i=0; i<n; i++)
    {
        h->vector[++h->last] = v[i];
    }

    int current = ((h->last)-1)/2;

    while (current > 0) {
        percolate_down(current, h);

        --current;
    }
}


int remove_min(pheap h)
{
    if (h->last == -1) {
        printf("Heap is empty\n");
        return -1; // Or some sentinel value indicating an error
    }

    int min_value = h->vector[0];
    h->vector[0] = h->vector[h->last--];

    // Adjust the heap property
    int current = 0;
    while (1) {
        int left_child = 2 * current + 1;
        int right_child = 2 * current + 2;
        int min_child = current;

        if (left_child <= h->last && h->vector[left_child] < h->vector[min_child]) {
            min_child = left_child;
        }

        if (right_child <= h->last && h->vector[right_child] < h->vector[min_child]) {
            min_child = right_child;
        }

        if (min_child != current) {
            swap(h, current, min_child);
            current = min_child;
        } else {
            break;
        }
    }

    return min_value;
}

void EmptyHeap(pheap h)
{
    while(h->last!=-1) remove_min(h);
}

void inicializar_semilla ()
{
    srand(time(NULL));
}

void randomVector (int v[] , int n)
{
    int i, m=2*n+1;
    for (i=0; i<n; i++)
    {
        v[i] = (rand() % m) - n;
    }
}

void ascendingVector (int v[] , int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        v[i] = i;
    }
}

void descendingVector (int v[] , int n)
{
    int i, j=0;
    for (i = n; i > 0; i--)
    {
        j++;
        v[i - 1] = j;
    }
}

void printVector (int v[], int n)
{
    int i;
    printf("\nVector: ");
    printf("[");
    for (i=0; i<n;i++)
    {
        printf("%4d", v[i]);
    }
    printf(" ]\n");
}

void printHeap(pheap h,int n,int v[])
{
    if(h->last!=-1) printVector(v,n);;

    for(int i=0; i <= h->last; i++)
    {
        printf("Heap position %d : %d\n", i, h->vector[i]);
    }
}

void Heap_sort(pheap h,int v[],int n,bool print)
{
    create_heap(v,n,h);

    if(print==true) printHeap(h,n,v);

    for(int i=0;i<n;i++)
    {
        v[i]= remove_min(h);
        if(print==true) printHeap(h,n,v);
    }

    EmptyHeap(h);
}

void TestCreateHeap(pheap h,int n,int v[])
{
    printf("\nTesting creating a heap:");
    ascendingVector(v,n);
    create_heap(v,n,h);

    if(h->last == -1) printf("heap initialized\n");
    printHeap(h,n,v);
    int x=remove_min(h);

    printf("\n%d has been removed from heap\n",x);
    printHeap(h,n,v);
    EmptyHeap(h);
    printHeap(h,n,v);
}

void TestHeapSort(pheap h,int v[],int n)
{
    printf("\nTesting Heap_sort:");
    randomVector(v,n);
    Heap_sort(h,v,n,true);
    printVector(v,n);
}

double testAlgorithmCreate (pheap h,int v[],double cotaSub)
{
    printf("\nCreating a heap:");
    double ta, tb, t;
    int k=1000,i,max=256000,n;
    printf("\n");
    printf("%7s%16s%18s%.2f%17s%22s\n", "n", "t(n)", "t(n)/n^", cotaSub, "t(n)/n","t(n)/n*log(n)");
    for(n=8000;n<=max;n*=2)
    {
        ascendingVector(v,n);
        ta=microseconds ();
        create_heap(v,n,h);
        tb=microseconds ();
        t=tb-ta;
        if (t<500)
        {
            ta=microseconds ();
            for(i=0; i<k; i++)
            {
                ascendingVector(v,n);
                create_heap(v,n,h);
            }
            tb=microseconds ();
            t=(tb-ta)/k;
            printf("(*)");
        }
        else
        {
            printf("   ");
        }
        printf("%6d%16.3f%18.6f%20.6f%18.6f\n", n, t, t/pow(n,cotaSub),t/n,t/(n*log(n)));
        EmptyHeap(h);
    }
    return t;
}

double testAlgorithmHeapSort (int vector[], int n,pheap h, void (*funGen)(int[] , int))
{
    double ta, tb, t, t1, t2;
    int k=1000;
    int count;

    funGen (vector, n);

    ta=microseconds ();
    Heap_sort(h,vector, n,false);
    tb=microseconds ();
    t=tb-ta;

    if (t<500)
    {
        ta=microseconds ();
        for(count=0; count<k; count++)
        {
            funGen (vector, n);
            Heap_sort(h,vector, n,false);
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

void printChartHeapSort (int v[],pheap h, void (*funGen)(int[] , int), double cotaSub,double cotaSob )
{
    double t;
    double tsub, taj, tsob;
    int n;
    printf("\n");
    printf("%7s%16s%18s%.2f%15s%15s%.2f \n", "n", "t(n)", "t(n)/n^", cotaSub, "t(n)/n*log(n)", "t(n)/n^", cotaSob);

    for (n = 500; n <= 32000; n*=2)
    {
        t = testAlgorithmHeapSort(v, n, h, funGen);

        tsub=t/pow(n,cotaSub);
        taj=t/(n*log(n));
        tsob=t/pow(n,cotaSob);
        printf("%6d%16.3f%18.6f%20.6f%18.6f\n", n, t, tsub, taj, tsob);
    }
}

int main() {
    pheap Heap = malloc(sizeof(struct heap));
    int v[SZ];
    inicializar_semilla();
    TestCreateHeap(Heap,10,v);
    TestHeapSort(Heap,v,10);
    testAlgorithmCreate(Heap,v,0.8);
    printf("\nTesting Heapsort with random vector:");
    printChartHeapSort(v,Heap,randomVector,1,1.2);
    printf("\nTesting Heapsort with ascending vector:");
    printChartHeapSort(v,Heap,ascendingVector,1,1.2);
    printf("\nTesting Heapsort with descending vector:");
    printChartHeapSort(v,Heap,descendingVector,1,1.2);

    free(Heap); //Free the memory allocated to Heap

    return 0;
}