#include "EXP/EXP.h"
#include "FUNC/FUNC.h"
#include "VAR/VAR.h"
#include "Operations/Operations.h"


int main(int argc, char* argv[])
{
	Mat<EXP> m(2,1);
	
	VAR x1;
	VAR x2;
	
	m.set(  cos(x1), 1,1);
	m.set( sin(x2), 2,1);
	
	toString(m);
	
	m = m*transpose(m);
	
	toString(m);
	
	float tempx1;
	float tempx2;
	
	std::cin >> tempx1;
	std::cin >> tempx2;
	
	x1.setValue(tempx1);
	x2.setValue(tempx2);
	
	evaluate(m);
	toString(m);
	
	return 0;
}
