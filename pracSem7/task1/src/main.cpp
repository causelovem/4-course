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
    /*<R-file> <H-file> <N> <dT> <n>*/
    if (argc != 6)
    {
        cerr << ">Unexpected quantity of arguments, check your comand string." << endl;
        return -1;
    }

    int nProc, myRank;
    int matrix_dim = atoi(argv[3]), n = atoi(argv[5]);
    double dT = atof(argv[4]);
    int myRankBlacs, nProcBlacs;
    int ictxtBlacs, npRowBlacs, npColBlacs, myRowBlacs, myColBlacs;
    int grid_dim;

    initGrid(argc, argv, nProc, myRank, grid_dim,
    myRankBlacs, nProcBlacs, ictxtBlacs,
    npRowBlacs, npColBlacs, myRowBlacs, myColBlacs);

    int block_size = int(ceil(double(matrix_dim) / double(grid_dim))), mod = matrix_dim % grid_dim;

    MPI_Datatype matrix_block;

    int root = 0;
    int size = 0, rows = 0, cols = 0;
    rows = numroc_(&matrix_dim, &block_size, &myRowBlacs, &root, &grid_dim);
    cols = numroc_(&matrix_dim, &block_size, &myColBlacs, &root, &grid_dim);
    size = rows * cols;

    int rowsN = npreroc_(&matrix_dim, &block_size, &myRowBlacs, &root, &grid_dim);
    int colsN = npreroc_(&matrix_dim, &block_size, &myColBlacs, &root, &grid_dim);

    int offset = (rowsN + colsN * matrix_dim) * 2 * sizeof(double);
    // int offset = (colsN + rowsN * matrix_dim) * 2 * sizeof(double);

    // cout << myRank << " " << block_size << " " << rows << " " << cols << " " << rowsN << " " << colsN << endl;
    // initBlock(rows, cols, matrix_dim, matrix_block);
    if (size != 0)
    {
        initBlock(cols, rows, matrix_dim, matrix_block);
    }
    else
    {
        initBlock(block_size, block_size, matrix_dim, matrix_block);
    }

    MPI_File fileR;
    MPI_File fileH;

    complexed *blockR = NULL, *blockH = NULL;
    
    readMatrixParallel(&fileR, argv[1], myRowBlacs, myColBlacs, grid_dim, offset, block_size, mod, matrix_dim, blockR, size, cols, rows, matrix_block);
    readMatrixParallel(&fileH, argv[2], myRowBlacs, myColBlacs, grid_dim, offset, block_size, mod, matrix_dim, blockH, size, cols, rows, matrix_block);
    
    UnitaryEvolution(myRank, myRowBlacs, myColBlacs, matrix_dim, cols, rows, block_size, ictxtBlacs, blockR, size, blockH, size, colsN, dT, n);

    delete [] blockR;
    delete [] blockH;
    blockH = blockR = NULL;

    MPI_File_close(&fileR);
    MPI_File_close(&fileH);
    
    Cblacs_gridexit(ictxtBlacs);
    Cblacs_exit(0);
    return 0;
}
