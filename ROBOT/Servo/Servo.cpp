#include "Servo.h"
#include <stdio.h>
 
Servo::Servo(int pin_, int idx, const Mat<float>& se3Init, float zerooffset_, float min_, float max_) : pin(pin_), min(min_), max(max_), zerooffset(zerooffset_)
{
	frame = new Frame(idx,se3Init);
	
	if( softPwmCreate( pin, zerooffset+se3Init.get(idx,1), max) == 0)
	{
		printf("WPI : softPWM : %f : pin %d : OKAY.\n", zerooffset+se3Init.get(idx,1), pin);
	}
	else
	{
		cerr << "WPI : softPWM : ERROR : pin : " << pin << "." << endl;
	}
	
}
Servo::~Servo()
{
	delete frame;
}
	
	//---------------------
void Servo::set(float val)
{
	if( zerooffset+val>=min && zerooffset+val <= max)
	{
		frame->setParam(val);
		softPwmWrite( pin, zerooffset+val);
	}
}
float Servo::get()
{
	return frame->getParam();
}

Mat<float> Servo::getSE3()
{
	return frame->getSE3();
}
	//---------------------
