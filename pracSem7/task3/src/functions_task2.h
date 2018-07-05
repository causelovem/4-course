#ifndef FUNCTIONSTASK2_H
#define FUNCTIONSTASK2_H


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

struct Parameters
{
    int N;
    double a, b;
    double wa, wb;
    int k, eMin, eMax;
    int rows, cols; // строк, столбцов в блоке
    int rowsN, colsN; // смещение по строкам, столбцам в глобальной матрице
    int matrixDim;

    int* vec;
    int size;
};

// Инициализация
// int initGrid(int &argc, char *argv[], int &nProc, int &myRank, int &grid_dim,
//     int &myRankBlacs, int &nProcBlacs, int &ictxtBlacs,
//     int &npRowBlacs, int &npColBlacs, int &myRowBlacs, int &myColBlacs);

// Вычисляет координаты диагональных блоков матрицы с их размерами в формате (<i><кол-во строк><номер блока>)
int createBlockCoordsMas(const Parameters &p, const vector< vector<int> > &C, vector<int> &v, vector<int> &v1);

// Вычисляет глобальные координаты в матрице по локальным координатам блока находящегося в процессе
int getGlobalIndex(const Parameters &p, int &iG, int &jG, int i, int j);

// По глобальной координате вычисляет номер диагонального блока в котором находится элемент, иначе возвращает -1, и координату относительно этого блока
int getCoordsInDiagBlock(const Parameters &p, int iG, int jG, int &i, int &j, int &num);

// То же самое, но для построения матриц L0, L1
int getCoordsInDiagBlockL(const Parameters &p, int iG, int jG, int &i, int &j, int &num);

// Построить по номеру диагонального блоку и номеру строки блока вектор состояния
int buildState(const Parameters &p, const vector< vector<int> > &C, int nBlock, int iL, vector<bool> &res);

// Вычисляет переход по двум состояниям
double calcTransfer(const Parameters &p, vector<bool> &st1, vector<bool> &st2);

// Получить координату по вектору
int vecToCoord(const Parameters &p, std::vector<bool> &st);

int buildLMatrix(const Parameters &p, complexed *l0, complexed *l1, int size, vector< vector<int> > &C);

// Печать матрицы в stdout
int printMatrix(Parameters &p, double* blockH, int size);

int makeCombination(vector< vector<int> > &res, int N);

#endif