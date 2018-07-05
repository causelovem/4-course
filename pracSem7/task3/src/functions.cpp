#ifndef FUNCTIONS_CPP
#define FUNCTIONS_CPP
#include "functions.h"

// #define HPLANKS 6.62606957e-27
#define HPLANKS 1

int initGrid(int &argc, char *argv[], int &nProc, int &myRank, int &grid_dim,
    int &myRankBlacs, int &nProcBlacs, int &ictxtBlacs,
    int &npRowBlacs, int &npColBlacs, int &myRowBlacs, int &myColBlacs)
{
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    double tmp = sqrt(nProc);
    grid_dim = tmp;
    if (tmp - grid_dim > 0.0000001)
    {
        if (myRank == 0)
            cout << ">Can not parallel program, because your proc number is not a 2-nd power of some N." << endl;

        MPI_Finalize();
        return -1;
    }

    npRowBlacs = grid_dim;
    npColBlacs = grid_dim;

    /* Grid Init */
    Cblacs_pinfo(&myRankBlacs, &nProcBlacs);
    Cblacs_get(-1, 0, &ictxtBlacs);
    Cblacs_gridinit(&ictxtBlacs, (char*)"Row", npRowBlacs, npColBlacs);
    Cblacs_gridinfo(ictxtBlacs, &npRowBlacs, &npColBlacs, &myRowBlacs, &myColBlacs);
    /*************/
    
    return 0;
}

int gridExit()
{
    return 0;
}

int initBlock(int &block_size_col, int &block_size_num, int &dist, MPI_Datatype &type)
{
    MPI_Type_vector(block_size_col, block_size_num, dist, MPI_DOUBLE_COMPLEX, &type);
    MPI_Type_commit(&type);

    return 0;
}

int readMatrixParallel(MPI_File *file, char* file_name, int myRowBlacs, int myColBlacs,
    int grid_dim, int offset, int block_size, int mod, int &matrix_dim, complexed *&matrix,
    int &size, int &rows, int &cols, MPI_Datatype matrix_block)
{
    MPI_Status status;
    MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, file);

    MPI_File_set_view(*file, offset, MPI_DOUBLE_COMPLEX, matrix_block, (char *)"native", MPI_INFO_NULL);

    matrix = new complexed [size];
    MPI_File_read(*file, matrix, size, MPI_DOUBLE_COMPLEX, &status);

    for (int i = 0; i < size; i++)
        matrix[i] = conj(matrix[i]);

    return 0;
}

int writeMatrixParallel()
{
    return 0;
}

int printMatrix(complexed* w, int lenW, int myRank, int myRowBlacs, int myColBlacs, int rows, int cols)
{
    if(myRowBlacs != myColBlacs)
        return 0;

    MPI_Status status;

    int nProc = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    double tmp = sqrt(nProc);
    int grid_dim = tmp;

    int size = rows * cols;

    double *mas = new double[rows];
    int i = 0;
    int k = 0;
    while(i < size)
    {
        mas[k] = w[i].real();
        k++;
        i += cols + 1;
    }

    if(myRank == 0)
    {
        for (int j = 0; j < rows; j++)
            cout << mas[j] << " ";
        // Получить от процессоров данные
        for (int i = grid_dim + 1; i < nProc; i += grid_dim + 1)
        {
            MPI_Recv(&rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            
            delete [] mas;
            mas = new double [rows];
            MPI_Recv(mas, rows, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);

            for (int j = 0; j < rows; j++)
                cout << mas[j] << " ";
        }
        cout << endl;
    }
    else
    {
        MPI_Send(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(mas, rows, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    delete [] mas;

    return 0;
}

int multiplyMatrix()
{
    return 0;
}

int expMatrix()
{
    return 0;
}

int multiplyMatrix(complexed a, complexed* m, int size)
{
    for(int i = 0; i < size; i++)
        m[i] *= a;
    
    return 0;
}

int addMatrix(complexed *a, complexed* m, int size)
{
    for(int i = 0; i < size; i++)
        a[i] += m[i];
    
    return 0;
}

int getEigenvaluesAndEigenvectors(int matrixDim, complexed *A, int lenA, int *desca, double* w, complexed* Z, int* descz)
{
    complexed* copyA = new complexed [lenA];
    memcpy(copyA, A, lenA * 2 * sizeof(double));

    complexed* work = new complexed [1];
    double* rwork = new double [1];

    int lwork = -1;
    int lrwork = -1;
    int info = 0;
    int one = 1;

    char v = 'V';
    char u = 'U';
    
    pzheev_(&v, &u, &matrixDim, copyA, 
        &one, &one, desca, w, Z, &one, &one, descz, 
        work, &lwork, rwork, &lrwork, &info);

    lwork = work[0].real();
    delete[] work;
    work = new complexed [lwork];

    lrwork = rwork[0];
    delete[] rwork;
    rwork = new double [lrwork * 2];

    pzheev_(&v, &u, &matrixDim, copyA, 
        &one, &one, desca, w, Z, &one, &one, descz, 
        work, &lwork, rwork, &lrwork, &info);

    delete [] copyA;
    delete [] work;
    delete [] rwork;
    return 0;
}

int UnitaryEvolution(int myRank, int myRowBlacs, int myColBlacs, int matrixDim, int rows, int cols, int block_size, int ictxtBlacs,
    complexed* R0, int lenR0, complexed* H, int lenH, int colsN, double dt, int n,
    complexed *L0, int lenL0, complexed *L1, int lenL1, double lIn, double lOut)
{
    char N = 'N';
    char C = 'C';
    char T = 'T';
    complexed oneC = 1.0, zeroC = 0.0;
    int one = 1;

    //получить спектральный вид и собств вектора
    int itemBlacs = max(1, cols), infoBlacs, zeroBlacs = 0;
    int size = rows * cols;

    int *descBlacsR = new int[9];
    descinit_(descBlacsR, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsH = new int[9];
    descinit_(descBlacsH, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);

    int *descBlacsL0 = new int[9];
    descinit_(descBlacsL0, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsL1 = new int[9];
    descinit_(descBlacsL1, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);

    int *descBlacsL0Tmp = new int[9];
    descinit_(descBlacsL0Tmp, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsL1Tmp = new int[9];
    descinit_(descBlacsL1Tmp, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);

    int *descBlacsZ = new int[9];
    descinit_(descBlacsZ, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsZleft = new int[9];
    descinit_(descBlacsZleft, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsZmul = new int[9];
    descinit_(descBlacsZmul, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);

    int *descBlacsW = new int[9];
    descinit_(descBlacsW, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);

    int *descBlacsResTmpL = new int[9];
    descinit_(descBlacsResTmpL, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsResL = new int[9];
    descinit_(descBlacsResL, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsResTmp = new int[9];
    descinit_(descBlacsResTmp, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);
    int *descBlacsRes = new int[9];
    descinit_(descBlacsRes, &matrixDim, &matrixDim, &block_size, &block_size, &zeroBlacs, &zeroBlacs, &ictxtBlacs, &itemBlacs, &infoBlacs);

    double* w = new double[matrixDim];
    complexed* W = new complexed[size];
    complexed* Z = new complexed[size];
    complexed* Zleft = new complexed[size];
    complexed* Zmul = new complexed[size];
    complexed* res_tmpL = new complexed[size];
    complexed* resL = new complexed[size];
    complexed* res_tmp = new complexed[size];
    complexed* res = new complexed[size];

    getEigenvaluesAndEigenvectors(matrixDim, H, lenH, descBlacsH, w, Z, descBlacsZ);

    complexed* w_tmp = new complexed[matrixDim];
    
    for(int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < matrixDim; j++)
                w_tmp[j] = complexed(w[j], 0);

            // умножить спектральный вид на (i+1) * dt * -(~i)/h
            multiplyMatrix(complexed(0, -dt * (i + 1) / HPLANKS), w_tmp, matrixDim);

            // возвести в экспоненту спектральный вид
            for(int j = 0; j < matrixDim; j++)
                w_tmp[j] = exp(w_tmp[j]);

            // перемножить матрицы преобразование и спектральный вид
            for (int j = 0; j < size; j++)
                W[j] = 0;

            if (myRowBlacs == myColBlacs)
                for (int j = 0; j < cols; j++)
                    W[j * cols + j] = w_tmp[j + colsN];



        }
        if (i != 0)
            memcpy(R0, res, size * sizeof(complexed));
        for (int t = 0; t < size; t++)
            res[t] = complexed(0, 0);
        // ШАГ 1
        // Z * W = Zleft
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, Z, &one, &one, descBlacsZ, W, &one, &one, descBlacsW, &zeroC, Zleft, &one, &one, descBlacsZleft);
        // Zleft * Z* = Zmul
        pzgemm_(&N, &C, &matrixDim, &matrixDim, &matrixDim, &oneC, Zleft, &one, &one, descBlacsZleft, Z, &one, &one, descBlacsZ, &zeroC, Zmul, &one, &one, descBlacsZmul);

        // Zmul * R0 = tmp_res
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, Zmul, &one, &one, descBlacsZmul, R0, &one, &one, descBlacsR, &zeroC, res_tmp, &one, &one, descBlacsResTmp);

        // res_tmp * Zmul* = res
        pzgemm_(&N, &C, &matrixDim, &matrixDim, &matrixDim, &oneC, res_tmp, &one, &one, descBlacsResTmp, Zmul, &one, &one, descBlacsZmul, &zeroC, res, &one, &one, descBlacsRes);


        // ШАГ 2
        // L0 * R0 = Zleft
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, L0, &one, &one, descBlacsL0, R0, &one, &one, descBlacsR, &zeroC, Zleft, &one, &one, descBlacsZleft);


        // Zleft * L0* = Zmul **
        pzgemm_(&N, &C, &matrixDim, &matrixDim, &matrixDim, &oneC, Zleft, &one, &one, descBlacsZleft, L0, &one, &one, descBlacsL0, &zeroC, Zmul, &one, &one, descBlacsZmul);

        // L0* * L0 = Zleft
        pzgemm_(&C, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, L0, &one, &one, descBlacsL0, L0, &one, &one, descBlacsL0, &zeroC, Zleft, &one, &one, descBlacsZleft);
        
        // Zleft * R0 = tmp_resL *
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, Zleft, &one, &one, descBlacsZleft, R0, &one, &one, descBlacsR, &zeroC, res_tmpL, &one, &one, descBlacsResTmpL);
        
        // R0 * Zleft = tmp_res *
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, R0, &one, &one, descBlacsR, Zleft, &one, &one, descBlacsZleft, &zeroC, res_tmp, &one, &one, descBlacsResTmp);


        // tmp_resL + tmp_res = tmp_resL *
        addMatrix(res_tmpL, res_tmp, size);

        // tmp_resL * -1\2 = tmp_resL **
        multiplyMatrix(complexed(-0.5, 0), res_tmpL, size);

        // Zmul + tmp_resL = Zmul
        addMatrix(Zmul, res_tmpL, size);

        // Zmul * Lin = Zmul
        multiplyMatrix(complexed(lIn, 0), Zmul, size);

        // Zmul * 1\h = Zmul
        multiplyMatrix(complexed(1/HPLANKS, 0), Zmul, size);
        // resL + Zmul = resL
        addMatrix(resL, Zmul, size);

        multiplyMatrix(complexed(dt, 0), resL, size);

        // res + resL = res
        addMatrix(res, resL, size);


        // ШАГ 3
        // L1 * R0 = Zleft
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, L1, &one, &one, descBlacsL1, R0, &one, &one, descBlacsR, &zeroC, Zleft, &one, &one, descBlacsZleft);

        // Zleft * L1* = Zmul **
        pzgemm_(&N, &C, &matrixDim, &matrixDim, &matrixDim, &oneC, Zleft, &one, &one, descBlacsZleft, L1, &one, &one, descBlacsL1, &zeroC, Zmul, &one, &one, descBlacsZmul);

        // L1* * L1 = Zleft
        pzgemm_(&C, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, L1, &one, &one, descBlacsL1, L1, &one, &one, descBlacsL1, &zeroC, Zleft, &one, &one, descBlacsZleft);

        // Zleft * R0 = tmp_resL *
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, Zleft, &one, &one, descBlacsZleft, R0, &one, &one, descBlacsR, &zeroC, res_tmpL, &one, &one, descBlacsResTmpL);

        // R0 * Zleft = tmp_res *
        pzgemm_(&N, &N, &matrixDim, &matrixDim, &matrixDim, &oneC, R0, &one, &one, descBlacsR, Zleft, &one, &one, descBlacsZleft, &zeroC, res_tmp, &one, &one, descBlacsResTmp);

        // tmp_resL + tmp_res = tmp_resL *
        addMatrix(res_tmpL, res_tmp, size);

        // tmp_resL * -1\2 = tmp_resL **
        multiplyMatrix(complexed(-0.5, 0), res_tmpL, size);

        // Zmul + tmp_resL = Zmul
        addMatrix(Zmul, res_tmpL, size);

        // Zmul * Lout = Zmul
        multiplyMatrix(complexed(lOut, 0), Zmul, size);

        // Zmul * 1\h = Zmul
        multiplyMatrix(complexed(1/HPLANKS, 0), Zmul, size);
        // resL + Zmul = resL
        addMatrix(resL, Zmul, size);

        multiplyMatrix(complexed(dt, 0), resL, size);

        // res + resL = res
        addMatrix(res, resL, size);


        // вывести диагональ
        if (myRank == 0)
            cout << i + 1 << endl;
        printMatrix(res, size, myRank, myRowBlacs, myColBlacs, rows, cols);
    }

    delete [] w_tmp;
    delete [] w;
    delete [] W;
    delete [] Z;
    delete [] Zleft;
    delete [] Zmul;
    delete [] res_tmpL;
    delete [] resL;
    delete [] res_tmp;
    delete [] res;

    delete [] descBlacsR;
    delete [] descBlacsH;
    delete [] descBlacsL0;
    delete [] descBlacsL1;
    delete [] descBlacsL0Tmp;
    delete [] descBlacsL1Tmp;
    delete [] descBlacsZ;
    delete [] descBlacsZleft;
    delete [] descBlacsZmul;
    delete [] descBlacsW;
    delete [] descBlacsResTmpL;
    delete [] descBlacsResL;
    delete [] descBlacsRes;
    delete [] descBlacsResTmp;
    return 0;
}
#endif