#ifndef FUNCTIONSTASK2_CPP
#define FUNCTIONSTASK2_CPP
#include "functions_task2.h"

// int initGrid(int &argc, char *argv[], int &nProc, int &myRank, int &gridDim,
//     int &myRankBlacs, int &nProcBlacs, int &ictxtBlacs,
//     int &npRowBlacs, int &npColBlacs, int &myRowBlacs, int &myColBlacs)
// {
//     MPI_Init(&argc, &argv);

//     MPI_Comm_size(MPI_COMM_WORLD, &nProc);
//     MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

//     double tmp = sqrt(nProc);
//     gridDim = tmp;
//     if (tmp - gridDim > 0.0000001)
//     {
//         if (myRank == 0)
//             cout << ">Can not parallel program, because your proc number is not a 2-nd power of some N." << endl;

//         MPI_Finalize();
//         return -1;
//     }

//     npRowBlacs = gridDim;
//     npColBlacs = gridDim;

//     /* Grid Init */
//     Cblacs_pinfo(&myRankBlacs, &nProcBlacs);
//     Cblacs_get(-1, 0, &ictxtBlacs);
//     Cblacs_gridinit(&ictxtBlacs, (char*)"Row", npRowBlacs, npColBlacs);
//     Cblacs_gridinfo(ictxtBlacs, &npRowBlacs, &npColBlacs, &myRowBlacs, &myColBlacs);
//     /*************/
    
//     return 0;
// }

int makeCombination(vector< vector<int> > &res, int N)
{
    res.resize(N + 1);
    for(int i = 0; i <= N; i++)
        for(int j = 0; j <= i; j++)
        {
            if(j == 0)
                res[i].push_back(1);
            else if(j == 1)
                res[i].push_back(i);
            else if (j == i)
                res[i].push_back(1);
            else
                res[i].push_back(res[i - 1][j - 1] + res[i - 1][j]);
        }

    return 0;
}

int createBlockCoordsMas(const Parameters &p, const vector< vector<int> > &C, vector<int> &v, vector<int> &v1)
{
    int i = 0;
    int s = 0;
    for(int l = 0; l <= p.k; l++)
    {
        v1.push_back(l);
        v1.push_back(s);
        for(int m = (p.eMin - l > 0 ? p.eMin - l : 0); m <= p.eMax - l; m++)
        {
            if (m <= 2 * p.N)
            {
                v.push_back(i);
                v.push_back(C[2 * p.N][m]);
                s += C[2 * p.N][m];
                i += v[v.size() - 1];
                v.push_back(m);
            }
        }
    }
    return 0;
}

int getGlobalIndex(const Parameters &p, int &iG, int &jG, int i, int j)
{
    iG = p.rowsN + i;
    jG = p.colsN + j;

    return 0;
}

int getCoordsInDiagBlock(const Parameters &p, int iG, int jG, int &i, int &j, int &num)
{
    i = -1;
    j = -1;
    num = -1;

    if ((iG >= p.rowsN) && (jG >= p.colsN) && (iG <= p.rows + p.rowsN) && (jG <= p.cols + p.colsN))
        for (int k = p.size - 3; k > -1; k -= 3)
            if ((p.vec[k] <= iG) && (p.vec[k] <= jG) && (p.vec[k] + p.vec[k + 1] - 1 >= iG) && (p.vec[k] + p.vec[k + 1] - 1 >= jG))
            {
                num = p.vec[k + 2];

                i = iG - p.vec[k];
                j = jG - p.vec[k];
                break;
            }

    if (num == -1)
        return -1;

    return 0;
}

int getCoordsInDiagBlockL(const Parameters &p, int iG, int jG, int &i, int &j, int &num)
{
    i = -1;
    j = -1;
    num = -1;

    for (int k = p.size - 3; k > -1; k -= 3)
        if ((p.vec[k] <= iG) && (p.vec[k] <= jG) && (p.vec[k] + p.vec[k + 1] - 1 >= iG) && (p.vec[k] + p.vec[k + 1] - 1 >= jG))
        {
            num = p.vec[k + 2];

            i = iG - p.vec[k];
            j = jG - p.vec[k];
            break;
        }

    if (num == -1)
        return -1;

    return 0;
}

int vecToCoord(const Parameters &p, std::vector<bool> &st)
{
    int j = 0;

    return j;
}

int buildLMatrix(const Parameters &p, complexed *l0, complexed *l1, int size, vector< vector<int> > &C)
{
    int iG = 0, jG = 0;

    for (int i = 0; i < p.rows; i++)
    {
        getGlobalIndex(p, iG, jG, i, 0);

    }


    return 0;
}

int buildState(const Parameters &p, const vector< vector<int> > &C, int nBlock, int iL, vector<bool> &res)
{
    int k = nBlock;
    int n = p.N * 2;
    while(n > 0)
    {
        if(n == k)
        {
            res.push_back(true);
            k--;
            n--;
            continue;
        }
        if(k == 0)
        {
            res.push_back(false);
            n--;
            continue;
        }

        if (iL >= C[n-1][k])
        {
            res.push_back(true);
            iL -= C[n-1][k];
            k--;
        }
        else
            res.push_back(false);
        n--;
    }

    return 0;
}

double calcTransfer(const Parameters &p, vector<bool> &st1, vector<bool> &st2)
{
    bool* tmp;
    int tmp_size = 2 * p.N;
    int i1 = -1, i2 = -1, cnt = 0;
    double res = 0;
    tmp = new bool [tmp_size];

    for (int i = 0; i < tmp_size; i++)
        tmp[i] = st1[i] ^ st2[i];

    for (int i = 0; i < tmp_size; i++)
        if(tmp[i] == 1)
        {
            if (i1 == -1)
                i1 = i;
            else
                i2 = i;
            cnt++;
        }

    delete [] tmp;

    if (cnt == 0)
    {
        for (int i = 0; i < tmp_size; i++)
        {
            if ((i % 2 == 0) && (st1[i] == 1))
                res += p.wa;
            else
            if ((i % 2 == 1) && (st1[i] == 1))
                res += p.wb;
        }
    }
    else
    if (cnt == 2)
    {
        if ((st1[i1] + st1[i2] == 1) && (st2[i1] + st2[i2] == 1))
        {
            if ((i1 % 2 == 1) && (i2 - 2 == i1))
            {
                // фотоны в соседних плоскостях
                res = p.b;
            }
            else
            if ((i1 % 2 == 0) && (i2 - 1 == i1))
            {
                // атом и фотон в одной полости
                res = p.a;
            }
        }
    }

    return res;
}

int printMatrix(Parameters &p, double* blockH, int size)
{
    MPI_Status status;

    int nProc = 0, myRank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    std::vector<std::vector<double> > matrixH(p.matrixDim, std::vector<double>(p.matrixDim));

    if (myRank == 0)
    {
        for (int i = 0; i < p.rows; i++)
        {
            for (int j = 0; j < p.cols; j++)
            {
                int iG = 0, jG = 0;

                getGlobalIndex(p, iG, jG, i, j);

                matrixH[iG][jG] = blockH[j + i * p.cols];
            }
        }

        for (int i = 1; i < nProc; i++)
        {
            Parameters pTmp;
            MPI_Recv(&pTmp.rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&pTmp.cols, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

            if (pTmp.rows * pTmp.cols != 0)
            {
                int sizeTmp = pTmp.rows * pTmp.cols;

                MPI_Recv(&pTmp.rowsN, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&pTmp.colsN, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                
                double* vecTmp = new double[sizeTmp];
                MPI_Recv(vecTmp, sizeTmp, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

                for (int i = 0; i < pTmp.rows; i++)
                {
                    for (int j = 0; j < pTmp.cols; j++)
                    {
                        int iG = 0, jG = 0;

                        getGlobalIndex(pTmp, iG, jG, i, j);

                        matrixH[iG][jG] = vecTmp[j + i * pTmp.cols];
                    }
                }
                delete [] vecTmp;
            }
        }

        for (int i = 0; i < p.matrixDim; i++)
        {
            for (int j = 0; j < p.matrixDim; j++)
                cout << matrixH[i][j] << " ";
            cout << endl;
        }
    }
    else
    {
        MPI_Send(&p.rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&p.cols, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        if (size != 0)
        {
            MPI_Send(&p.rowsN, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&p.colsN, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(blockH, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    return 0;
}

#endif