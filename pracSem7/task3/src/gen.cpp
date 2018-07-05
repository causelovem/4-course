#include <iostream>
#include <ctime> // в ней функция time
#include <cstdlib>
#include <fstream>

#include <complex>
 
using namespace std;

typedef complex<double> complexed;
 
int main(int argc, char *argv[])
{
    ofstream matrix(argv[1], ios::binary);
    if (argc < 4)
    {
        srand(time(NULL)); // Инициализируем генератор случайных чисел. 
        int n = 0; 
        n = atoi(argv[2]);
        complexed **a = new complexed* [n]; // Создаем массив указателей
        for (int i = 0; i < n; i++)
        {
            a[i] = new complexed [n]; // Создаем элементы
        }
        
        for (int i = 0; i < n; i++)
        {
            for (int j = i; j < n ; j++)
            {
                double re = (double)(rand()) / RAND_MAX * 20 - 10;
                double im = (double)(rand()) / RAND_MAX * 20 - 10;
                re = i == j && i == 0 ? 1 : 0;
                im = 0;

                complexed c(re, im);
                a[i][j] = c;

                a[j][i] = conj(c);
            }
            a[i][i] = complexed(a[i][i].real(), 0);
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n ; j++)
            {
                matrix.write((char *)&a[i][j], 2 * sizeof(double));
            }
        } 

        // for (int i = 0; i < n; i++)
        // {
        //     for (int j = 0; j < n ; j++)
        //     {
        //         cout << a[i][j] << endl;
        //     }
        //     cout << endl;
        // }

        for (int i = 0; i < n; i++)
        {
            delete [] a[i];
        }
        delete [] a;
    } 
    else
    {
        // int count = 9;
        // complexed b[count] = {complexed(7,0),complexed(17,13),complexed(8,1),
        //                       complexed(17,-13),complexed(88,0),complexed(1,1),
        //                       complexed(8,-1),complexed(1,-1),complexed(-14,0)};

        // int count = 9;
        // complexed b[count] = {complexed(1,0),complexed(0,0),complexed(0,0),
        //                       complexed(0,-0),complexed(1,0),complexed(0,-0),
        //                       complexed(0,-0),complexed(0,0),complexed(1,0)};

        // int count = 16;
        // complexed b[count] = {complexed(2,0),complexed(0,0),complexed(0,0),complexed(0,0),
        //                       complexed(0,0),complexed(2,0),complexed(0,0),complexed(0,0),
        //                       complexed(0,0),complexed(0,0),complexed(2,0), complexed(0,0),
        //                       complexed(0,0),complexed(0,0),complexed(0,0), complexed(2,0)};


        // int count = 16;
        // complexed b[count] = {complexed(7,0),complexed(17,13),complexed(8,1),complexed(1,0),
        //                       complexed(17,-13),complexed(88,0),complexed(1,-1),complexed(1,0),
        //                       complexed(8,-1),complexed(1,1),complexed(-14,0), complexed(1,0),
        //                       complexed(1,0),complexed(1,0),complexed(1,0), complexed(1,0)};

        int count = 16;
        complexed b[count] = {complexed(0,0),complexed(0,0),complexed(0,0),complexed(0,0),
                              complexed(0,0),complexed(0,0),complexed(0,0),complexed(0,0),
                              complexed(0,0),complexed(0,0),complexed(-1.570766,0), complexed(1.570766,0),
                              complexed(0,0),complexed(0,0),complexed(1.570766,0), complexed(-1.570766,0)};

        // int count = 16;
        // complexed b[count] = {complexed(0,0),complexed(0,0),complexed(0,0),complexed(0,0),
        //                       complexed(0,0),complexed(0,0),complexed(0,0),complexed(0,0),
        //                       complexed(0,0),complexed(0,0),complexed(0,0),complexed(0,0),
        //                       complexed(0,0),complexed(0,0),complexed(0,0),complexed(1,0)};

        for (int i = 0; i < count; i++)
        {
            matrix.write((char *)&b[i], 2 * sizeof(double));
            // cout << b[i] << endl;
        }
    }

    matrix.close();
    return 0;
}