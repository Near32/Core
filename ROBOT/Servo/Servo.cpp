#include "Servo.h"
#include <stdio.h>
 
#define	pwmRange 200

float toRad( float deg)
{
	return deg*PI/180.0f;
}

float toDeg( float rad)
{
	return rad*180.0f/PI;
}


Servo::Servo(int pin_, int idx, const Mat<float>& se3Init, float zerooffset_, float min_, float max_) : inverted(false), pin(pin_), min(min_), max(max_), zerooffset(zerooffset_)
{
	frame = new Frame(idx,se3Init);
	
	//200*100 = 20 000 microseconds = 20 ms pwm length, made up of 200 steps.
	if( softPwmCreate( pin, (zerooffset+se3Init.get(idx,1))*pwmRange/max, pwmRange) == 0)
	{
		printf("WPI : softPWM : %f : pin %d : OKAY.\n", (zerooffset+se3Init.get(idx,1))*pwmRange/max, pin);
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
	float value = (inverted? zerooffset-val : zerooffset+val)*pwmRange/max;
	
	if(  value >= 0.0f && value <= max)
	{
		frame->setParam( toRad(val) );
		softPwmWrite( pin, value);
		printf("WPI : softPWM : %f : pin %d : SET.\n", value, this->pin);
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
