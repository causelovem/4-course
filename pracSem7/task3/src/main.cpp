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
#include <string>

using namespace std;
#include "scalapack.h"
#include "functions.h"
#include "functions_task2.h"

typedef complex<double> complexed;

int main(int argc, char *argv[])
{   
    /*<N> <a> <b> <wa> <wb> <k> <Emin> <Emax> <lIn> <lOut> <n> <dT> <R-file>*/
    /*<parametersFile> <[sizeFlag]>*/
    if ((argc > 3) || (argc < 2))
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

    ifstream parametersFile(argv[1]);
    int N, k, eMin, eMax, n;
    double a, b, wa, wb, dT, lIn, lOut;
    string file;

    parametersFile >> N;
    parametersFile >> a;
    parametersFile >> b;
    parametersFile >> wa;
    parametersFile >> wb;
    parametersFile >> k;
    parametersFile >> eMin;
    parametersFile >> eMax;
    parametersFile >> lIn;
    parametersFile >> lOut;
    parametersFile >> n;
    parametersFile >> dT;
    parametersFile >> file;

    char *fileName = new char [file.size()];
    memcpy(fileName, file.c_str(), file.size() * sizeof(char));

    Parameters params;

    params.N = N;

    params.a = a;
    params.b = b;
    params.wa = wa;
    params.wb = wb;

    params.k = k;
    params.eMin = eMin; 
    params.eMax = eMax;

    vector<int> v;
    vector<int> v1;
    vector< vector<int> > C;
    makeCombination(C, 2 * params.N);
    createBlockCoordsMas(params, C, v, v1);

    params.size = v.size();
    params.vec = new int [params.size];
    for (int i = 0; i < params.size; i++)
        params.vec[i] = v[i];

    params.matrixDim = params.vec[params.size - 3] + params.vec[params.size - 2];

    if (argc == 3)
    {
        if (myRank == 0)
            cout << params.matrixDim << endl;

        delete [] params.vec;
        delete [] fileName;
        
        Cblacs_gridexit(ictxtBlacs);
        Cblacs_exit(0);
        return 0;
    }

    int blockSize = int(ceil(double(params.matrixDim) / double(gridDim))), mod = params.matrixDim % gridDim;

    int root = 0;
    int size = 0;
    params.cols = numroc_(&params.matrixDim, &blockSize, &myRowBlacs, &root, &gridDim);
    params.rows = numroc_(&params.matrixDim, &blockSize, &myColBlacs, &root, &gridDim);
    size = params.rows * params.cols;

    params.colsN = npreroc_(&params.matrixDim, &blockSize, &myRowBlacs, &root, &gridDim);
    params.rowsN = npreroc_(&params.matrixDim, &blockSize, &myColBlacs, &root, &gridDim);
    
    double* blockH = new double [size];
    complexed *l0 = new complexed [size], *l1 = new complexed [size];

    for (int i = 0; i < size; i++)
    {
        l0[i] = 0;
        l1[i] = 0;
    }

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

    for (int i = 0; i < params.rows; i++)
    {
        for (int j = 0; j < params.cols; j++)
        {
            int iG = 0, jG = 0, nBlock1 = 0, nBlock2 = 0, tmp = -1;
            int i1 = 0, j1 = 0;
            getGlobalIndex(params, iG, jG, i, j);

            for (int l = 0; l < params.matrixDim; l++)
            {
                int jTmp = 0;
                tmp = getCoordsInDiagBlockL(params, iG, l, i1, jTmp, nBlock1);
                if (tmp != -1)
                    break;
            }
            tmp = -1;

            for (int l = 0; l < params.matrixDim; l++)
            {
                int iTmp = 0;
                tmp = getCoordsInDiagBlockL(params, l, jG, iTmp, j1, nBlock2);
                if (tmp != -1)
                    break;
            }

            int k1 = 0, k2 = 0;
            for (int k = v1.size() - 1; k > -1; k -= 2)
                if (iG >= v1[k])
                {
                    k1 = v1[k - 1];
                    break;
                }

            for (int k = v1.size() - 1; k > -1; k -= 2)
                if (jG >= v1[k])
                {
                    k2 = v1[k - 1];
                    break;
                }

            std::vector<bool> st1, st2;

            buildState(params, C, nBlock1, i1, st1);
            buildState(params, C, nBlock2, j1, st2);

            // for (int i = 0; i < st1.size(); i++)
            //     cout << st1[i] << ' ';
            // cout << ' ' << k1 << endl;
            // for (int i = 0; i < st2.size(); i++)
            //     cout << st2[i] << ' ';
            // cout << ' ' << k2 << endl;
            // cout << endl;

            int cnt = 0, sizeTmp = st1.size(), iTmp = 0;

            for (int k = 0; k < sizeTmp; k++)
                if ((st1[k] ^ st2[k]) == 1)
                {
                    iTmp = k;
                    cnt++;
                }

            if (cnt == 1)
            {
                if ((iTmp == 1) && (st1[iTmp] == 0) && (st2[iTmp] == 1) && (k1 == k2))
                {
                    l0[j + i * params.cols] = 1;
                    l1[j + i * params.cols] = 0;

                    // for (int i = 0; i < st1.size(); i++)
                    //     cout << st1[i] << ' ';
                    // cout << ' ' << k1 << endl;
                    // for (int i = 0; i < st2.size(); i++)
                    //     cout << st2[i] << ' ';
                    // cout << ' ' << k2 << endl;
                    // cout << endl;
                    continue;
                }
                else
                if ((iTmp == sizeTmp - 2) && (st1[iTmp] == 1) && (st2[iTmp] == 0) && (k2 - k1 == 1))
                {
                    l0[j + i * params.cols] = 0;
                    l1[j + i * params.cols] = 1;

                    // for (int i = 0; i < st1.size(); i++)
                    //     cout << st1[i] << ' ';
                    // cout << ' ' << k1 << endl;
                    // for (int i = 0; i < st2.size(); i++)
                    //     cout << st2[i] << ' ';
                    // cout << ' ' << k2 << endl;
                    // cout << endl;
                    continue;
                }
            }

            l0[j + i * params.cols] = 0;
            l1[j + i * params.cols] = 0;
        }
    }

    printMatrix(params, blockH, size);

    double *l00 = new double [size], *l10 = new double [size];

    for (int i = 0; i < size; i++)
    {
        l00[i] = l0[i].real();
        l10[i] = l1[i].real();
    }

    if (myRank == 0)
        cout << endl;
    printMatrix(params, l00, size);

    if (myRank == 0)
        cout << endl;
    printMatrix(params, l10, size);


    MPI_Datatype matrixBlock;

    int offset = (params.colsN + params.rowsN * params.matrixDim) * 2 * sizeof(double);

    if (size != 0)
    {
        initBlock(params.rows, params.cols, params.matrixDim, matrixBlock);
    }
    else
    {
        initBlock(blockSize, blockSize, params.matrixDim, matrixBlock);
    }


    MPI_File fileR;

    complexed *blockR = NULL, *blockHC = NULL;

    readMatrixParallel(&fileR, fileName, myRowBlacs, myColBlacs, gridDim, offset, blockSize, mod, params.matrixDim, blockR, size, params.rows, params.cols, matrixBlock);

    blockHC = new complexed [size];
    for (int i = 0; i < size; i++)
        blockHC[i] = complexed(blockH[i], 0);
    
    
    UnitaryEvolution(myRank, myRowBlacs, myColBlacs, params.matrixDim, params.rows, params.cols, blockSize, ictxtBlacs, blockR, size, blockHC, size, params.rowsN, dT, n, l0, size, l1, size, lIn, lOut);

    // for (int i = 0; i < size; i++)
    // {
    //     l00[i] = l0[i].real();
    //     l10[i] = l1[i].real();
    // }

    // if (myRank == 0)
    //     cout << endl;
    // printMatrix(params, l00, size);

    // if (myRank == 0)
    //     cout << endl;
    // printMatrix(params, l10, size);

    delete [] blockH;
    delete [] blockHC;
    delete [] params.vec;
    delete [] blockR;
    delete [] fileName;
    delete [] l0;
    delete [] l1;
    delete [] l00;
    delete [] l10;

    MPI_File_close(&fileR);
    
    Cblacs_gridexit(ictxtBlacs);
    Cblacs_exit(0);
    return 0;
}
