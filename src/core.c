#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../includes/core.h"

#define SIZE 500
#define N_TESTS 20

void calcAndSaveTests (double *time) {
    int i;
    double sum = 0.0, average, sd;

    // Calc average
    for (i = 0; i < N_TESTS; i++) {
        sum += time[i];
    }
    average = sum / N_TESTS;

    sum = 0.0;
    // Calc standard deviation
    for (i = 0; i < N_TESTS; i++) {
        double sub = fabs(time[i] - average);
        sum += pow(sub, 2);
    }
    sd = sqrt(sum / (N_TESTS - 1));

    // Save data on file:
    FILE *file = fopen("../tests/out.txt", "a+");
    fprintf (file, " Média: %f - Desvio Padrão: %lf\n", average, sd);
    fclose(file);
}

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

void printSudoku(Sudoku *s) {
    int i, j;
    for (i = 0; i < s->N; i++) {
        for (j = 0; j < s->N; j++) {
            printf("%d ", s->data[i * s->N + j]);
        }
        printf("\n");
    }
}

void pringGraph(Sudoku *s) {
    int size = s->N * s->N, i;
    Item *begin;
    Item *end;
    Item *it;

    // Print adjacency List
    for (i = 0; i < size; i++) {
        begin = s->adjacencyList[i].inicio->prox;
        end = s->adjacencyList[i].fim;

        for (it = begin; ; it = it->prox) {
            printf("%d ", it->item);
            if (it == end) { break; }
        }
        printf("\n");
    }
    printf("----------------------------------\n");

    // Print possibilities List
    for (i = 0; i < size; i++) {
        begin = s->notPossible[i].inicio->prox;
        end = s->notPossible[i].fim;

        for (it = begin; ; it = it->prox) {
            printf("%d ", it->item);
            if (it == end) { break; }
        }
        printf("\n");
    }
    printf("----------------------------------\n");

    // Print data
    for (i = 0; i < size; i++) {
        printf("%d ", s->data[i]);
    }
    printf("\n");
}

void makeEmptyGraph (Sudoku *s, int N, int I, int J, int **data) {
    int i, j;
    s->N = N;
    s->I = I;
    s->J = J;

    s->notPossible = (Lista*) malloc((N * N) * sizeof(Lista));
    s->adjacencyList = (Lista*) malloc((N * N) * sizeof(Lista));
    for (i = 0; i < N * N; i++) {
        makeEmptyList(&s->notPossible[i]);
        makeEmptyList(&s->adjacencyList[i]);
    }
    s->data = (int*) calloc(N * N, sizeof(int));
    s->numberOfZeros = 0;

    insertEdges(s, N);
    setGrids(s, N, I, J);

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            insertValues(s, i, j, data[i][j], N);
            if (!data[i][j]) { s->numberOfZeros++; }
        }
    }
}

void insertValues (Sudoku *s, int row, int column, int value, int N) {
    int position = row * N + column;

    Item *begin = s->adjacencyList[position].inicio->prox;
    Item *end = s->adjacencyList[position].fim;
    
    s->data[position] = value;
    
    if(value != 0) {
        Item *it;
        for (it = begin;  ; it = it->prox) {
            // Only add non-repetitive numbers
            Item *item = findItem(&s->notPossible[it->item], value);
            if (item == NULL) {
                addItemOrdered(&s->notPossible[it->item], value);
                s->notPossible[it->item].size++;
            }
            if (it == end) { break; }
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

void calcSudoku (Sudoku *s) {
    int size = s->N * s->N, i;
    int maxPossibilityIndex = 0, maxPossibilitySize = 0;

    while(s->numberOfZeros) {
        for (i = 0; i < size; i++) {
            if (s->notPossible[i].size > maxPossibilitySize && s->data[i] == 0) {
                maxPossibilityIndex = i;
                maxPossibilitySize = s->notPossible[i].size;
            }
        }

        for (i = 1; i <= s->N; i++) {
            Item *item = findItem(&s->notPossible[maxPossibilityIndex], i);
            if (item == NULL) {
                s->data[maxPossibilityIndex] = i;
        
                Item *it;
                for (it = s->adjacencyList[maxPossibilityIndex].inicio->prox ; ; it = it->prox) {
                    Item *possible = findItem(&s->notPossible[it->item], i);
                    if (possible == NULL) {
                        addItemOrdered(&s->notPossible[it->item], i);
                        s->notPossible[it->item].size++;
                    }
                    if (it == s->adjacencyList[maxPossibilityIndex].fim) { break; }
                }
                break;
            }
        }

        if (i == s->N + 1) {
            printf("sem solução\n");
            printSudoku(s);
            return;
        }
        
        s->numberOfZeros--;
        maxPossibilityIndex = 0;
        maxPossibilitySize = 0;
    }

    printf("solução\n");
    printSudoku(s);
}

void freeGraph (Sudoku *s) {
    int i, size = s->N * s->N;
    for (i = 0; i < size ; i++) {
        freeList(&s->notPossible[i]);
        freeList(&s->adjacencyList[i]);
    }

    free(s->data);
    freeList(s->notPossible);
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

    // Fill structures
    Sudoku s;
    makeEmptyGraph(&s, N, I, J, data);

    // Algorithm to calc time execution: 
    // double executionTime[N_TESTS];
    // int clk = 0;
    // for (clk = 0; clk < N_TESTS; clk++) {
    //     clock_t tempoInicial;
    //     clock_t tempoFinal;
    //     tempoInicial = clock();

        calcSudoku(&s);

    //     tempoFinal = clock();
    //     executionTime[clk] = (tempoFinal- tempoInicial) * 1000.0 / CLOCKS_PER_SEC;
    // }

    // calcAndSaveTests(executionTime);

    // Free memory
    for (i = 0; i < N; i++)
        free(data[i]);
    free(data);
    freeGraph(&s);  
}