#include <iostream>
#include <ctime> // в ней функция time
#include <cstdlib>
#include <fstream>
#include <string>
#include <complex>
using namespace std;

typedef complex<double> complexed;

int main(int argc, char *argv[])
{
	ofstream file(argv[1], ios::binary);

	string s;
	cin >> s;
	while(s != "END")
	{
		double re = atoi(s.c_str());
		cin >> s;
		double im = atoi(s.c_str());
		complexed c(re, im);
		file.write((char *)&c, 2 * sizeof(double));
		cin >> s;
	}

	file.close();
	return 0;
}