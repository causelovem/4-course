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
#include <math.h>

using namespace std;
#include "scalapack.h"
#include "functions.h"

typedef complex<double> complexed;

int main(int argc, char *argv[])
{
    /* <N> <a> <b> <wa> <wb> <k> <Emin> <Emax> */
    /* <a> - at <-> ph
       <b> - ph1 <-> ph2
       <wa> - at1 <-> at1
       <wb> - ph1 <-> ph1 */
    if (argc != 9)
    {
        cerr << ">Unexpected quantity of arguments, check your comand string." << endl;
        return -1;
    }
    
    int nProc, myRank;
    int myRankBlacs, nProcBlacs;
    int ictxtBlacs, npRowBlacs, npColBlacs, myRowBlacs, myColBlacs;
    int gridDim;

    initGrid(argc, argv, nProc, myRank, gridDim,
    myRankBlacs, nProcBlacs, ictxtBlacs,
    npRowBlacs, npColBlacs, myRowBlacs, myColBlacs);

    Parameters params;

    params.N = atoi(argv[1]);

    params.a = atof(argv[2]);
    params.b = atof(argv[3]);
    params.wa = atof(argv[4]);
    params.wb = atof(argv[5]);

    params.k = atoi(argv[6]);
    params.eMin = atoi(argv[7]); 
    params.eMax = atoi(argv[8]);

    vector<int> v;
    vector< vector<int> > C;
    makeCombination(C, 2 * params.N);
    createBlockCoordsMas(params, C, v);

    params.size = v.size();
    params.vec = new int [params.size];
    for (int i = 0; i < params.size; i++)
        params.vec[i] = v[i];

    params.matrixDim = params.vec[params.size - 3] + params.vec[params.size - 2];

    int blockSize = int(ceil(double(params.matrixDim) / double(gridDim)));

    int root = 0;
    int size = 0;
    params.cols = numroc_(&params.matrixDim, &blockSize, &myRowBlacs, &root, &gridDim);
    params.rows = numroc_(&params.matrixDim, &blockSize, &myColBlacs, &root, &gridDim);
    size = params.rows * params.cols;

    params.colsN = npreroc_(&params.matrixDim, &blockSize, &myRowBlacs, &root, &gridDim);
    params.rowsN = npreroc_(&params.matrixDim, &blockSize, &myColBlacs, &root, &gridDim);
    // params.rows = numroc_(&params.matrixDim, &blockSize, &myRowBlacs, &root, &gridDim);
    // params.cols = numroc_(&params.matrixDim, &blockSize, &myColBlacs, &root, &gridDim);
    // size = params.rows * params.cols;

    // params.rowsN = npreroc_(&params.matrixDim, &blockSize, &myRowBlacs, &root, &gridDim);
    // params.colsN = npreroc_(&params.matrixDim, &blockSize, &myColBlacs, &root, &gridDim);
    
    double* blockH = new double [size];

    for (int i = 0; i < params.rows; i++)
    {
        for (int j = 0; j < params.cols; j++)
        {
            int iG = 0, jG = 0, nBlock = 0, tmp = -1;
            int i1 = 0, j1 = 0;
            getGlobalIndex(params, iG, jG, i, j);

            tmp = getCoordsInDiagBlock(params, iG, jG, i1, j1, nBlock);

            if (tmp == -1)
            {
                blockH[j + i * params.cols] = 0;
                // continue;
            }
            else
            {

                std::vector<bool> st1, st2;

                buildState(params, C, nBlock, i1, st1);
                buildState(params, C, nBlock, j1, st2);

                blockH[j + i * params.cols] = calcTransfer(params, st1, st2);
            }
        }
    }

    printMatrix(params, blockH, size);

    delete [] blockH;
    delete [] params.vec;
    Cblacs_gridexit(ictxtBlacs);
    Cblacs_exit(0);
    return 0;
}