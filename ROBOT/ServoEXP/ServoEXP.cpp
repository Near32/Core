#include "ServoEXP.h"

ServoEXP::ServoEXP(const VAR& variable_, int pin_, int idx, const Mat<float>& se3Init, float zerooffset_, float min, float max) : Servo(, int pin_, int idx, const Mat<float>& se3Init, float zerooffset_, float min, float max)
{
		frameEXP = new FrameEXP(variable_, idx, se3Init);
		
}

ServoEXP::~ServoEXP()
{
	if(frameEXP != NULL)	delete frameEXP;
}
