#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../includes/core.h"

#define N_TESTS 20
#define SIZE 500

// void calcAndSaveTests (double *time, int N) {
//     int i;
//     double sum = 0.0, average, sd;

//     // Calc average
//     for (i = 0; i < N_TESTS; i++) {
//         sum += time[i];
//     }
//     average = sum / N_TESTS;

//     sum = 0.0;
//     // Calc standard deviation
//     for (i = 0; i < N_TESTS; i++) {
//         double sub = fabs(time[i] - average);
//         sum += pow(sub, 2);
//     }
//     sd = sqrt(sum / (N_TESTS - 1));

//     // Save data on file:
//     FILE *file = fopen("../tests/out-greedy.txt", "a+");
//     // FILE *file = fopen("../tests/out-dynamic.txt", "a+");
//     fprintf (file, "Sudoki NXN %d - Média: %f - Desvio Padrão: %lf\n", N, N, average, sd);
//     fclose(file);
// }

void printAllInformations (int N, int I, int J, int **data) {
    int i, j;
    printf("\n%d %d %d\n", N, I, J);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }
}

void pringGraph(Sudoku *s) {
    int size = s->N * s->N, i;
    Item *begin;
    Item *end;
    Item *it;

    for (i = 0; i < size; i++) {
        begin = s->adjacencyList[i].inicio->prox;
        end = s->adjacencyList[i].fim;

        for (it = begin; ; it = it->prox) {
            printf("%d ", it->item);
            if (it == end) {
                break;
            }
        }
        printf("\n");
    }
}

void makeEmptyGraph (Sudoku *s, int N, int I, int J, int **data) {
    int i, j;
    s->N = N;
    s->I = I;
    s->J = J;

    s->possibilities = (Lista*) malloc((N * N) * sizeof(Lista));
    s->adjacencyList = (Lista*) malloc((N * N) * sizeof(Lista));
    for (i = 0; i < N * N; i++) {
        makeEmptyList(&s->possibilities[i]);
        makeEmptyList(&s->adjacencyList[i]);
    }
    s->data = (int*) calloc(N * N, sizeof(int));

    insertEdges(s, N);
    setGrids(s, N, I, J);

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            insertValues(s, i, j, data[i][j], N);
        }
    }
}

void insertValues (Sudoku *s, int row, int column, int value, int N) {
    int position = row * N + column;

    Item *begin = s->adjacencyList[position].inicio;
    Item *end = s->adjacencyList[position].fim;
    
    s->data[position] = value;
    
    if(value != 0) {
        Item *it;
        for (it = begin; it != end; it = it->prox) {
            // Only add non-repetitive numbers
            if (!findItem(&s->possibilities[it->item], value)) {
                addItemEnd(&s->possibilities[it->item], value);
            }
        }
    }
}

void insertEdges (Sudoku *s, int N) {
    int i, size = N * N;
    for (i = 0; i < size; i++) {
        int actualLine = (i / N), column;
        for (column = (i % N) + 1; column < N; column++) {
            addItemEnd(&s->adjacencyList[i], actualLine * N + column);
            addItemEnd(&s->adjacencyList[actualLine * N + column], i);
        }

        int actualColumn = (i % N), line;
        for (line = (i / N) + 1; line < N; line++) {
            addItemEnd(&s->adjacencyList[i], line * N + actualColumn);
            addItemEnd(&s->adjacencyList[line * N + actualColumn], i);
        }
    }
}

void setGrids(Sudoku *s, int N, int I, int J) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int source = i * N + j;
            for(int line = i + 1; (line < N) && (line % J != 0); line++) {
                for(int column = 0; column < I; column++) {
                    column += (j / I) * I;
                    int destiny = line * N + column;
                    if(j % I != column % I) {
                        addItemEnd(&s->adjacencyList[source], destiny);
                        addItemEnd(&s->adjacencyList[destiny], source);
                    }
                    column -= (j / I) * I;
                }
            }
        }
    }
}

void freeGraph (Sudoku *s) {
    int i, size = s->N * s->N;
    for (i = 0; i < size ; i++) {
        freeList(&s->possibilities[i]);
        freeList(&s->adjacencyList[i]);
    }

    free(s->data);
    freeList(s->possibilities);
    freeList(s->adjacencyList);
}

void initProgram (FILE *file) {
    // Read data:
    // N -> Size NXN
    // I -> Columns
    // J -> Lines

    int N, I, J, **data, i;

    int charCtrl = 0, columnCtrl = 0, lineCtrl = 0;
    char linhaAlfanumerica[SIZE], *info;

    // Extract informations:

    while(fscanf(file, "%s", linhaAlfanumerica) != EOF) {
        info = strtok(linhaAlfanumerica, " ");
        while (info != NULL) {
            if (charCtrl == 0) { // Size NXN
                N = atoi(info);
                // if (!N) { return; }
                data = (int**)calloc(N, sizeof(int*)); 
                for (i = 0; i < N; i++) 
                    data[i] = (int*)calloc(N, sizeof(int)); 
            }
            else if (charCtrl == 1) { // Columns
                I = atoi(info);
            }
            else if (charCtrl == 2) { // Lines
                J = atoi(info);
            }
            else if (charCtrl > 2) { // sudoku data
                int item = atoi(info);
                data[lineCtrl][columnCtrl] = item;
                if (columnCtrl == N - 1) {
                    lineCtrl++;
                    columnCtrl = 0;
                } else {
                    columnCtrl++;
                }
            }
            charCtrl++;
            info = strtok(NULL, " ");
        }
    }

    printAllInformations(N, I, J, data);

    // Fill structures
    Sudoku s;
    makeEmptyGraph(&s, N, I, J, data);
    printf("------------------------------------\n");
    pringGraph(&s);

    // Algorithm to calc time execution: 
    // double executionTime[N_TESTS];
    // int clk = 0;
    // for (clk = 0; clk < N_TESTS; clk++) {
    //     clock_t tempoInicial;
    //     clock_t tempoFinal;
    //     tempoInicial = clock();

        // TODO: calc answer
       
    //     tempoFinal = clock();
    //     executionTime[clk] = (tempoFinal- tempoInicial) * 1000.0 / CLOCKS_PER_SEC;
    // }


    // TODO: print answer

    // calcAndSaveTests(executionTime, M);

    // Free memory
    for (i = 0; i < N; i++)
        free(data[i]);
    free(data);
    freeGraph(&s);
}