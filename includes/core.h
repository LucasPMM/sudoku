#include "listas.h"

typedef struct {
    int *data;
    int N;
    int I;
    int J;
    Lista *notPossible;
    Lista *adjacencyList;
} Sudoku;

// Utils functions:

// #1) Read all informations and calc solutions
void initProgram (FILE *file);
// #2) Calc average and standard deviation 
void calcAndSaveTests(double *time, int N);
void printAllInformations (int N, int I, int J, int **data);
void makeEmptyGraph (Sudoku *s, int N, int I, int J, int **data);
void insertEdges (Sudoku *s, int N);
void setGrids(Sudoku *s, int N, int I, int J);
void insertValues (Sudoku *s, int row, int column, int value, int N);
void freeGraph (Sudoku *s);

// TODO: organizar e documentar todas as funções