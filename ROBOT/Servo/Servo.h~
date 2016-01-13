#ifndef SERVO_H
#define SERVO_H

#include "../Frame/Frame.h"
#include <wiringPi.h>
#include <softPwm.h>

class Servo 
{
	private :
	Frame* frame;
	float min;
	float max;
	float zerooffset;
	//additionnal offset that is to be added to the given value in order to go
	//from the zero of the servo to the zero of the frame.
	// SERVO --> FRAME offset.
	
	int pin;
	
	public :
	
	Servo(int pin_, int idx, const Mat<float>& se3Init, float zerooffset_ = 0.0f, float min = -120.0f, float max = 120.0f);
	~Servo();
	
	//---------------------
	void set(float val);
	float get();	
	
	Mat<float> getSE3();
	//---------------------
	
};

#endif
