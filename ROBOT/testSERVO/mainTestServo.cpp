#include "../ServoEXP/ServoEXP.h"

int main(int argc, char* argv[])
{
	VAR x;
	int pin = 17;
	int idx = 3;
	
	Mat<float> se3init(0.0f,6,1);
	float zerooffset = 120.0f;
	float max = 240.0f;
	float min = 0.0f;
	
	float valDegree = 0.0f;
	
	if(argc > 1)
	{
		pin = atoi(argv[1]);
	}
	std::cout << " PIN :: " << pin << std::endl;
	
	
	if(argc > 2)
	{
		valDegree = atof(argv[2]);
	}
	
	std::cout << " INIT VALUE :: " << valDegree << " degree. " << std::endl;
	std::cout << " CONTINUE ?? [0,1]" << std::endl;
	int continuer = 0;
	std::cin >> continuer;
	
	if(continuer)
	{
		x.setValue( toRad(valDegree) );
	
	
		std::cout << "Raspberry Pi : initialization of wiringPi : ..." << std::endl;
		if( wiringPiSetup() == -1)
			exit(-1);
		std::cout << "Raspberry Pi : initialization of wiringPi : OKAY." << std::endl;
	
	
		ServoEXP servo( x, pin, idx, se3init, zerooffset, min,max);
		
		while(1)
		{
			int tempval = 0;
			std::cin >> tempval;
			float val = x.getValue();
		
			if(tempval != 0)
			{
				val+=toRad((float)tempval);
				x.setValue( val );
			}
		
			std::cout << "val = " << val << std::endl;
		
			servo.setVARVALUE();
		}
	}
		
	return 0;
}
