#include <iostream>
#include "Mat/Mat.h"

using namespace std;

int main(int argc, char* argv[])
{
	Mat<float> A(10.0f,100,100);
	Mat<float> B(100,100,(char)1);
	
	clock_t timer = clock();
	for(int i=10;i--;)
	{
		A*=B;
	}
	
	cout << (float)(clock()-timer)/CLOCKS_PER_SEC << " seconds." << endl;
	
	
};
