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

    // Fill structures and find solutions
    // TODO: fill structure

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


    // calcAndSaveTests(executionTime, M);

    // Free memory
    for (i = 0; i < N; i++)
        free(data[i]);
    free(data);
}