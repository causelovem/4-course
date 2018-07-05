#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <ctime>
#include <mpi.h>
#include <vector>
#include <algorithm>

#include <complex>
#include <unistd.h>

using namespace std;
#include "scalapack.h"

typedef complex<double> complexed;

// Инициализация решетки Scalapack
int initGrid(int &argc, char *argv[], int &nProc, int &myRank, int &grid_dim,
    int &myRankBlacs, int &nProcBlacs, int &ictxtBlacs,
    int &npRowBlacs, int &npColBlacs, int &myRowBlacs, int &myColBlacs);

// Деиницилизация решетки Scalapack
int gridExit();

// Инициализация блоков для чтения
int initBlock(int &block_size_col, int &block_size_num, int &dist, MPI_Datatype &type);

// Параллельное считывание матрицы из бинарного файла
int readMatrixParallel(MPI_File *file, char* file_name, int myRowBlacs, int myColBlacs,
    int grid_dim, int offset, int block_size, int mod, int &matrix_dim, complexed *&matrix,
    int &size, int &rows, int &cols, MPI_Datatype matrix_block);

// Параллельное запись матрицы в бинарный файл
int writeMatrixParallel();

// Вывод диагонали матрицы в stdout (только 0 процесс выведет диагональ)
int printMatrix(complexed* w, int lenW, int myRank, int myRowBlacs, int myColBlacs, int rows, int cols);

// Умножение матрицы на матрицу
int multiplyMatrix();

// Взятие экспоненты от матрицы
int expMatrix();

// Умножение матрицы на число
int multiplyMatrix(complexed a, complexed* m, int size);

// Сложение матриц и запись в (а)
int addMatrix(complexed *a, complexed* m, int size);

// Эрмитово сопряжение матрицы
int conjugateTranspose(); 

// Транспонирование матрицы
int transpose();

// ---Вычисление собственных значения и векторов эрмитовой матрицы A
// matrixDim - кол-во стоблцов и строк в матрице A
// A - распределнная эрмитова матрица
// ia, ja - The row and column indices in the global matrix A indicating the first row and the first column of the submatrix A, respectively.
// desca - дескриптор матрицы A
// w - массив куда будут записаны собственные значения(перед вызовом должна быть выделена память для него размера matrixDim)
// Z - распределенная матрица куда будут записаны собственные вектора (под него должна быть выделенна память)
// descz - дескриптор матрицы Z
int getEigenvaluesAndEigenvectors(int matrixDim, complexed *A, int lenA, int *desca, double* w, complexed* Z, int* descz);

// Построение матрицы плотности по вектору
int buildDensityMatrix();

// Унитарная эволюция
int UnitaryEvolution(int myRank, int myRowBlacs, int myColBlacs, int matrixDim, int rows, int cols, int block_size, int ictxtBlacs,
    complexed* R0, int lenR0, complexed* H, int lenH, int colsN, double dt, int n,
    complexed *L0, int lenL0, complexed *L1, int lenL1, double lIn, double lOut);
//int UnitaryEvolution(int myRank, int myRowBlacs, int myColBlacs, int matrixDim, int rows, int cols, int block_size, int ictxtBlacs, int itemBlacs, complexed* R0, int lenR0, int* decsR0, complexed* H, int lenH, int* descH, double dt, int n);

#endif