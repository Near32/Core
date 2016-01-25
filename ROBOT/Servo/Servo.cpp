#include "Servo.h"
#include <stdio.h>
 
int	pwmRange = 200;

float toRad( float deg)
{
	return deg*PI/180.0f;
}

float toDeg( float rad)
{
	return rad*180.0f/PI;
}

float wpi2Servo( float wpi)
{
	return 5.0f+wpi*20.0f/pwmRange;
}

Servo::Servo(int pin_, int idx, const Mat<float>& se3Init, float zerooffset_, float min_, float max_) : inverted(false), pin(pin_), min(min_), max(max_), zerooffset(zerooffset_)
{
	frame = new Frame(idx,se3Init);
	
	//200*100 = 20 000 microseconds = 20 ms pwm length, made up of 200 steps.
	if( softPwmCreate( pin, wpi2Servo( (zerooffset+se3Init.get(idx,1))*pwmRange/max ), pwmRange) == 0)
	{
		printf("WPI : softPWM : %f : pin %d : OKAY.\n", (zerooffset+se3Init.get(idx,1))*100.0f/max, pin);
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
	
	if(  value >= 0.0f && value <= pwmRange)
	{
		frame->setParam( toRad(val*180.0f/pwmRange) );
		softPwmWrite( pin, wpi2Servo(value) );
		printf("WPI : softPWM : %f : pin %d : SET.\n", wpi2Servo(value)/*100.0f/pwmRange*/, this->pin);
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
