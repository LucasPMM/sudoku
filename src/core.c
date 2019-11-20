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
    int max_probable_vertex, max_probable;
    int possible_solution, sudoku_complete;
    int count = 0;
    while(1) {
        // printf("---INICIO---\n");
        count ++;
        max_probable_vertex = 0;
        max_probable = 0;
        possible_solution = 0;
        sudoku_complete = 1;

        for (i = 0; i < size; i++) {
            if(s->data[i] == 0) {
                sudoku_complete = 0;
                break;
            }
        }

        if(sudoku_complete) {
            printf("solução\n");
            printSudoku(s);
            return;
        }

        for (i = 0; i < size; i++) {
            if (s->notPossible[i].size > max_probable && s->data[i] == 0) {
                max_probable_vertex = i;
                max_probable = s->notPossible[i].size;
            }
        }
        for (i = 1; i <= s->N; i++) {
            int colour_possible = 1;
            Item *item = findItem(&s->notPossible[max_probable_vertex], i);
            
            
            
            // printf("tentando encontrar o item %d no set na pos %d\n", i, max_probable_vertex);
            // printf("-------------------------\n");
            // int k;
            // Item *begin;
            // Item *end;
            // Item *it;
            // for (k = 0; k < size; k++) {
            //     begin = s->notPossible[k].inicio->prox;
            //     end = s->notPossible[k].fim;

            //     for (it = begin; ; it = it->prox) {
            //         printf("%d ", it->item);
            //         if (it == end) { break; }
            //     }
            //     printf("\n");
            // }
            // printf("-------------------------\n");



            if(item != NULL) {
                colour_possible = 0;
                // printf("entrei 1\n");
            }
            
            if (colour_possible) {
                // printf("entrei 2\n");
                possible_solution = 1;
                s->data[max_probable_vertex] = i;
        
                Item *it;

                for (it = s->adjacencyList[max_probable_vertex].inicio->prox ; ; it = it->prox) {
                    Item *possible = findItem(&s->notPossible[it->item], i);
                    if (possible == NULL) {
                        addItemOrdered(&s->notPossible[it->item], i);
                        s->notPossible[it->item].size++;
                    }
                    if (it == s->adjacencyList[max_probable_vertex].fim) { break; }
                }
                break;
            }
        }

        if (possible_solution == 0) {
            printf("sem solução\n");
            printSudoku(s);
            return;
        }
    }

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

    printAllInformations(N, I, J, data);

    // Fill structures
    Sudoku s;
    makeEmptyGraph(&s, N, I, J, data);
    printf("---------------------------------------------\n");
    pringGraph(&s);
    printf("---------------------------------------------\n");

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


    // calcAndSaveTests(executionTime, M);

    // Free memory
    for (i = 0; i < N; i++)
        free(data[i]);
    free(data);
    freeGraph(&s);
}