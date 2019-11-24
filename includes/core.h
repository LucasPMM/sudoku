#include "listas.h"

typedef struct {
    int numberOfZeros;      // Number of zeros on sudoku
    int *data;              // Sudoku values
    int N;                  // Sudoku size
    int I;                  // Number of columns
    int J;                  // Number of lines
    Lista *notPossible;     // Number that the vertex cannot be
    Lista *adjacencyList;   // Mounted adjacency list
} Sudoku;

// Utils functions:

// #1) Read all informations and calc solutions
void initProgram (FILE *file);
// #2) Calc average and standard deviation
void calcAndSaveTests (double *time, int line, int col);
// #3) Print readed information 
void printAllInformations (int N, int I, int J, int **data);
// #4) Print graph information 
void pringGraph(Sudoku *s);


// Sudoku functions:

// #1) Print sudoku 
void printSudoku(Sudoku *s);
// #2) Calc sudoku solution 
void calcSudoku (Sudoku *s);
// #3) Starts to build graph 
void makeEmptyGraph (Sudoku *s, int N, int I, int J, int **data);
// #4) Insert edges between vertex 
void insertEdges (Sudoku *s, int N);
// #5) Insert edges between vertex inside block 
void setGrids(Sudoku *s, int N, int I, int J);
// #6) Insert readed values on graph 
void insertValues (Sudoku *s, int row, int column, int value, int N);
// #7) Free graph memory 
void freeGraph (Sudoku *s);
