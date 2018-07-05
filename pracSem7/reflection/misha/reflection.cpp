#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

#include <mpi.h>

using namespace std;


int main(int argc, char *argv[])
{
    /*<matrixDim>*/
    // if (argc != 2)
    if ((argc != 2) && (argc != 3))
    {
        cerr << ">Unexpected quantity of arguments, check your comand string." << endl;
        return -1;
    }

    MPI_Init(&argc, &argv);

    int nProc = 0, myRank = 0;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int matrixDim = atoi(argv[1]);

    if (matrixDim < nProc)
    {
        if (myRank == 0)
            cout << ">Can not parallel program, because your proc number is more than matrix dim." << endl;

        MPI_Finalize();
        return -1;
    }


    /*РАСЧЁТ РАЗМЕРОВ*/

    int blockSize = 0;
    double **matrix = NULL;

    for (int i = myRank; i < matrixDim; i += nProc)
        blockSize++;

    matrix = new double* [blockSize];
    for (int i = 0; i < blockSize; i++)
        matrix[i] = new double [matrixDim];

    double *bVec = new double [matrixDim];

    /*ГЕНЕРАЦИЯ МАТРИЦЫ*/

    if ((nProc == 1) && (!strcmp(argv[2], "f")))
    {
        ifstream matrixFile;
        matrixFile.open("matrixFile");

        for (int i = 0; i < matrixDim; i++)
        {
            for (int j = 0; j < matrixDim; j++)
                matrixFile >> matrix[j][i];

            matrixFile >> bVec[i];
        }

        matrixFile.close();
    }
    else
        for (int i = myRank; i < matrixDim; i += nProc)
        {
            int num = i / nProc;
            for (int j = 0; j < matrixDim; j++)
            {
                matrix[num][j] = 1.0 / ((i + 1.0) + (j + 1.0) + 1.0);
                // matrix[num][j] = i + j + 1;
            }

            matrix[num][i] += matrixDim;
            matrix[num][i] += 0.5;
        }


    // if (myRank == 0)
    // {
    //     matrix[0][0] = 3.0;
    //     matrix[0][1] = 4.0;
    // }
    // else
    // {
    //     matrix[0][0] = 5.0;
    //     matrix[0][1] = 6.0;
    // }

    // if (nProc == 1)
    // {
    //     matrix[0][0] = 3.0;
    //     matrix[0][1] = 4.0;
    //     matrix[1][0] = 5.0;
    //     matrix[1][1] = 6.0;
    // }


    /*ГЕНЕРАЦИЯ ВЕКТОРА В*/

    if ((nProc != 1) || (strcmp(argv[2], "f")))
    {
        for (int i = 0; i < matrixDim; i++)
            bVec[i] = 0;

        for (int i = 0; i < matrixDim; i++)
        {
            for (int j = 0; j < matrixDim; j++)
                bVec[i] += 1.0 / ((i + 1.0) + (j + 1.0) + 1.0);
            bVec[i] += 0.5;
        }
    }

    double start = 0, end = 0;
    double *time = new double [3];


    /*МЕТОД ОТРАЖЕНИЙ - ПРИВЕДЕНИЕ МАТРИЦЫ К ВЕРХНЕТРЕУГОЛЬНОМУ ВИДУ*/

    start = MPI_Wtime();
    for (int i = 0; i < matrixDim - 1; i++)
    {
        double *sendVec = new double [matrixDim - i];
        int num = i / nProc;

        if (i % nProc == myRank)
        {
            // i / nProc = какой столбей на процессе
            // i = на сколько надо сделать вектор меньше

            double preNorm = 0.0;
            for (int j = i + 1; j < matrixDim; j++)
                preNorm += matrix[num][j] * matrix[num][j];

            double colNorm = sqrt(preNorm + matrix[num][i] * matrix[num][i]);
            double tmp = matrix[num][i] - colNorm;
            double colSideNorm = sqrt(preNorm + tmp * tmp);

            sendVec[0] = tmp / colSideNorm;

            for (int j = 1; j < matrixDim - i; j++)
                sendVec[j] = matrix[num][i + j] / colSideNorm;

            // double tmpNorm = 0.0;
            // for (int j = 0; j < matrixDim - i; j++)
            //     tmpNorm += sendVec[j] * sendVec[j];
            // cout << sqrt(tmpNorm) << endl;
        }

        MPI_Bcast(sendVec, (matrixDim - i), MPI_DOUBLE, i % nProc, MPI_COMM_WORLD);

        if (i % nProc > myRank)
            num++;

        for (int j = num; j < blockSize; j++)
        {
            double tmpA = 0.0;
            for (int k = i; k < matrixDim; k++)
                tmpA += sendVec[k - i] * matrix[j][k];

            tmpA *= 2.0;

            for (int k = i; k < matrixDim; k++)
                matrix[j][k] -= sendVec[k - i] * tmpA;
        }

        double tmpB = 0.0;
        for (int k = i; k < matrixDim; k++)
            tmpB += sendVec[k - i] * bVec[k];

        tmpB *= 2.0;

        for (int k = i; k < matrixDim; k++)
            bVec[k] -= sendVec[k - i] * tmpB;

        delete [] sendVec;
    }
    end = MPI_Wtime() - start;
    MPI_Reduce(&end, &time[0], 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);


    /*ВЫЧИСЛЕНИЕ РЕШЕНИЙ МЕТОДОМ ГАУССА*/
 
    double *tmpBVec = new double [matrixDim];
    memcpy(tmpBVec, bVec, matrixDim * sizeof(double));

    double *solution = new double [matrixDim];
    
    start = MPI_Wtime();
    for (int i = matrixDim - 1; i > -1; i--)
    {
        double *sendVec = new double [i + 1];
        int num = i / nProc;

        if (i % nProc == myRank)
        {
            for (int j = 0; j < i; j++)
                sendVec[j] = matrix[num][j];
            sendVec[i] = tmpBVec[i] / matrix[num][i];
        }

        MPI_Bcast(sendVec, i + 1, MPI_DOUBLE, i % nProc, MPI_COMM_WORLD);

        for (int j = 0; j < i; j++)
            tmpBVec[j] -= sendVec[j] * sendVec[i];

        solution[i] = sendVec[i];

        delete [] sendVec;
    }
    end = MPI_Wtime() - start;
    MPI_Reduce(&end, &time[1], 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);


    /*ВЫЧИСЛЕНИЕ НЕВЯЗКИ - ||Ax - b||*/

    double norm = 0.0;
    double *sendVec = new double [matrixDim];

    start = MPI_Wtime();
    for (int i = 0; i < matrixDim; i++)
    {
        double tmp = 0.0;
        for (int j = 0; j < blockSize; j++)
            tmp += matrix[j][i] * solution[myRank + j * nProc];
        sendVec[i] = tmp;
    }

    if (myRank != 0)
    {
        MPI_Send(sendVec, matrixDim, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        double *discrepancy = new double [matrixDim];
        memcpy(discrepancy, sendVec, matrixDim * sizeof(double));

        for (int i = 1; i < nProc; i++)
        {
            MPI_Recv(sendVec, matrixDim, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);   
            for (int j = 0; j < matrixDim; j++)
                discrepancy[j] += sendVec[j];
        }


        /*ВЫВОД ОТВЕТА НА КОНСОЛЬ - ВЕКТОР Ax*/

        // cout << endl << "Ax" << endl;
        // for (int i = 0; i < matrixDim; i++)
        //     cout << discrepancy[i] << " ";
        // cout << endl;

        for (int i = 0; i < matrixDim; i++)
        {
            discrepancy[i] -= bVec[i];
            norm += discrepancy[i] * discrepancy[i];
        }

        norm = sqrt(norm);

        delete [] discrepancy;
    }
    end = MPI_Wtime() - start;
    MPI_Reduce(&end, &time[2], 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    delete [] sendVec;


    /*ВЫВОД ОТВЕТА НА КОНСОЛЬ - МАТРИЦА И ВЕКТОР В*/

    // if (myRank != 0)
    // {
    //     for (int i = 0; i < blockSize; i++)
    //         MPI_Send(matrix[i], matrixDim, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    // }
    // else
    // {
    //     cout << endl;
    //     for (int i = 0; i < blockSize; i++)
    //     {
    //         // cout << myRank << " " << i << endl;
    //         for (int j = 0; j < matrixDim; j++)
    //             cout << matrix[i][j] << " ";
    //         cout << endl;
    //     }

    //     for (int i = 1; i < nProc; i++)
    //     {
    //         int tmpBlockSize = 0;
    //         for (int j = i; j < matrixDim; j += nProc)
    //             tmpBlockSize++;

    //         for (int k = 0; k < tmpBlockSize; k++)
    //         {
    //             double *outVec = new double [matrixDim];
    //             MPI_Recv(outVec, matrixDim, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

    //             // cout << i << " " << k << endl;
    //             for (int j = 0; j < matrixDim; j++)
    //                 cout << outVec[j] << " ";
    //             cout << endl;

    //             delete [] outVec;
    //         }
    //     }

    //     cout << endl;
    //     cout << "B" << endl;
    //     for (int j = 0; j < matrixDim; j++)
    //         cout << bVec[j] << " ";
    //     cout << endl;
    // }


    /*ВЫВОД ОТВЕТА НА КОНСОЛЬ - РЕШЕНИЕ, НЕВЯЗКА И ВРЕМЯ*/

    if (myRank == 0)
    {
        cout << endl;
        for(int i = 0; i < matrixDim; i++)
            cout << "X" << i << " = " << solution[i] << endl;
        cout << endl;

        cout << "||Ax - b|| = " << norm << endl;

        cout << endl;
        cout << "Number of proc = " << nProc << endl;
        cout << "Matrix Dim = " << matrixDim << endl;     
        cout << "Reflection time = " << time[0] << endl;
        cout << "Gauss time = " << time[1] << endl;
        cout << "Discrepancy time = " << time[2] << endl;   
    }


    for (int i = 0; i < blockSize; i++)
        delete [] matrix[i];
    delete [] matrix;

    delete [] bVec;
    delete [] tmpBVec;
    delete [] solution;

    delete [] time;

    MPI_Finalize();
    return 0;
}
