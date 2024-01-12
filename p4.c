#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include <math.h>

typedef int ** matrix;

void dijkstra(matrix graph, matrix distances, int sz) {
    int n, i, min, v = 0;

    for (n = 0; n < sz; n++) {
        int *unvisited = malloc(sz * sizeof(int));

        // Initialization
        for (i = 0; i < sz; i++) {
            unvisited[i] = 1;
            distances[n][i] = graph[n][i];
        }
        unvisited[n] = 0;

        // Dijkstra's algorithm
        for (int count = 0; count < sz - 2; count++) {
            // Find the node from unvisited with the minimum distance
            min = INT_MAX;
            for (i = 0; i < sz; i++) {
                if (unvisited[i] && distances[n][i] < min) {
                    min = distances[n][i];
                    v = i;
                }
            }

            unvisited[v] = 0; // Mark the selected node as visited

            // Update distances for neighboring nodes
            for (int w = 0; w < sz; w++) {
                if (unvisited[w] && distances[n][w] > distances[n][v] + graph[v][w]) {
                    distances[n][w] = distances[n][v] + graph[v][w];
                }
            }
        }

        free(unvisited);
    }
}

// Function to print the matrix
void printMatrix(matrix mat, int sz) {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            if (mat[i][j] == INT_MAX) {
                printf("INF\t");
            } else {
                printf("%d\t", mat[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}


#define MAX_SIZE 1000
matrix createMatrix(int n) {
    int i;
    matrix aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

/* Pseudorandom initialization [1..MAX_SIZE] of a complete undirected graph
with n nodes, represented by its adjacency matrix */

void initMatrix(matrix m, int n) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % MAX_SIZE + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}

void freeMatrix(matrix m, int n) {
    int i;
    for (i = 0; i < n; i++)
        free(m[i]);
    free(m);
}

double microseconds ()    // obtains the system hour in microseconds
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
    {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void test_cases ()
{
    int sz1 = 5, sz2 = 4;
    matrix graph1, distances1, graph2, distances2;

    printf("Validation of the correct functioning of Dijkstra's alrorithm:\n");

    // Test case 1
    graph1 = (matrix)malloc(sz1 * sizeof(int *));
    distances1 = (matrix)malloc(sz1 * sizeof(int *));
    for (int i = 0; i < sz1; i++) {
        graph1[i] = (int *)malloc(sz1 * sizeof(int));
        distances1[i] = (int *)malloc(sz1 * sizeof(int));
    }

    //Initialization of graph1
    graph1[0][0] = 0;
    graph1[0][1] = 1;
    graph1[0][2] = 8;
    graph1[0][3] = 4;
    graph1[0][4] = 7;
    graph1[1][0] = 1;
    graph1[1][1] = 0;
    graph1[1][2] = 2;
    graph1[1][3] = 6;
    graph1[1][4] = 5;
    graph1[2][0] = 8;
    graph1[2][1] = 2;
    graph1[2][2] = 0;
    graph1[2][3] = 9;
    graph1[2][4] = 5;
    graph1[3][0] = 4;
    graph1[3][1] = 6;
    graph1[3][2] = 9;
    graph1[3][3] = 0;
    graph1[3][4] = 3;
    graph1[4][0] = 7;
    graph1[4][1] = 5;
    graph1[4][2] = 5;
    graph1[4][3] = 3;
    graph1[4][4] = 0;

    // Test case 2
    graph2 = (matrix)malloc(sz2 * sizeof(int *));
    distances2 = (matrix)malloc(sz2 * sizeof(int *));
    for (int i = 0; i < sz2; i++) {
        graph2[i] = (int *)malloc(sz2 * sizeof(int));
        distances2[i] = (int *)malloc(sz2 * sizeof(int));
    }

    //Initialization of graph2
    graph2[0][0] = 0;
    graph2[0][1] = 1;
    graph2[0][2] = 4;
    graph2[0][3] = 7;
    graph2[1][0] = 1;
    graph2[1][1] = 0;
    graph2[1][2] = 2;
    graph2[1][3] = 8;
    graph2[2][0] = 4;
    graph2[2][1] = 2;
    graph2[2][2] = 0;
    graph2[2][3] = 3;
    graph2[3][0] = 7;
    graph2[3][1] = 8;
    graph2[3][2] = 3;
    graph2[3][3] = 0;

    dijkstra(graph1, distances1, sz1);
    printf("Test Case 1:\n");
    printf("Adjacency matrix:\n");
    printMatrix(graph1, sz1);
    printf("Minimum distances:\n");
    printMatrix(distances1, sz1);

    dijkstra(graph2, distances2, sz2);
    printf("Test Case 2:\n");
    printf("Adjacency matrix:\n");
    printMatrix(graph2, sz2);
    printf("Minimum distances:\n");
    printMatrix(distances2, sz2);

    // Free memory of graph1 and graph2
    for (int i = 0; i < sz1; i++) {
        free (graph1[i]);
        free (distances1[i]);
    }
    free (graph1);
    free (distances1);

    for (int i = 0; i < sz2; i++) {
        free (graph2[i]);
        free (distances2[i]);
    }
    free (graph2);
    free (distances2);
}

double algorithm_time (int size) {
    matrix graph = createMatrix(size);
    matrix distances = createMatrix(size);

    initMatrix(graph, size);

    double start_time = microseconds();

    dijkstra(graph, distances, size);

    double end_time = microseconds();
    double elapsed_time = end_time - start_time;

    if (elapsed_time < 500) {
        int k = 1000;
        int count;

        start_time = microseconds();
        for (count = 0; count < k; count++)
        {
            graph = createMatrix(size);
            distances = createMatrix(size);
            initMatrix(graph, size);
            dijkstra(graph, distances, size);
        }
        end_time = microseconds();
        double smaller_time_1 = end_time - start_time;

        start_time = microseconds();
        for (count = 0; count < k; count++)
        {
            graph = createMatrix(size);
            distances = createMatrix(size);
            initMatrix(graph, size);
        }
        end_time = microseconds();
        double smaller_time_2 = end_time - start_time;
        elapsed_time = (smaller_time_1 - smaller_time_2) / k;
        printf("(*)");
    } else {
        printf("   ");
    }

    freeMatrix(graph, size);
    freeMatrix(distances, size);

    return elapsed_time;
}

void printChart (double cota_sub, double cota_aj, double cota_sob) {
    double time, time_sub, time_aj, time_sob;
    int sizes[] = {10, 50, 100, 200, 500, 1000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n%7s%16s%18s%.2f%15s%.2f%15s%.2f \n", "n", "t(n)", "t(n)/n^", cota_sub, "t(n)/n^", cota_aj, "t(n)/n^", cota_sob);

    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        time = algorithm_time(size);

        time_sub = time / pow (size, cota_sub);
        time_aj = time / pow (size, cota_aj);
        time_sob = time / pow (size, cota_sob);
        printf("%6d%16.3f%18.6f%20.6f%18.6f\n", size, time, time_sub, time_aj, time_sob);
    }
}


int main() {
    test_cases();
    printf("Testing Dijkstra's algorithm with a randomly generated undirected graph:");
    printChart(2.54, 2.74, 2.94);
    return 0;
}
