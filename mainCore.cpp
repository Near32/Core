#include "EXP/EXP.h"
#include "FUNC/FUNC.h"
#include "VAR/VAR.h"

int main(int argc, char* argv[])
{
	VAR x(1.0f);
	VAR y(1.0f);
	
	EXP e1 = x+y;
	
	std::cout << "VALUE e1 = x + y = " << e1.evaluate() << std::endl;
	
	float tempx = 0.0f;
	float tempy = 0.0f;
	
	std::cout << "NEW X = ?" << std::endl;
	std::cin >> tempx;
	x.setValue(tempx);
	std::cout << "NEW Y = ?" << std::endl;
	std::cin >> tempy;
	y.setValue(tempy);
	
	std::cout << "NEW VALUE e1 = x + y = " << e1.evaluate() << std::endl;
	
	EXP e2(e1);
	e1 = e1 * e1;
	
	std::cout << "NEW VALUE e1 = e1^2 = (x + y)*(x + y) = " << e1.evaluate() << " and e2 = " << e2.evaluate() << std::endl;
	
	e1 = e1 / e2;
	
	std::cout << "NEW VALUE e1 = e1 / e2 = (x + y) = " << e1.evaluate() << std::endl;
	
	e1 = sin( e1);
	std::cout << "NEW VALUE e1 = sin( e1 ) = sin(x + y) = " << e1.evaluate() << std::endl;
	e1 = asin( e1);
	std::cout << "NEW VALUE e1 = asin( e1 ) = (x + y) = " << e1.evaluate() << std::endl;
	e1 = cos( e1);
	std::cout << "NEW VALUE e1 = cos( e1 ) = cos(x + y) = " << e1.evaluate() << std::endl;
	e1 = acos( e1);
	std::cout << "NEW VALUE e1 = acos( e1 ) = (x + y) = " << e1.evaluate() << std::endl;
	
	return 0;
}
