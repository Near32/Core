#include "ROBOONE2016/HAROEXP.h"

int main( int argc, char* argv[])
{
	std::cout << "Raspberry Pi : initialization of wiringPi : ..." << std::endl;
	if( wiringPiSetup() == -1)
		exit(-1);
	std::cout << "Raspberry Pi : initialization of wiringPi : OKAY." << std::endl;
	
	HAROEXP instanceHARO;
	
	instanceHARO.loop();
	
	return 0;	
}
