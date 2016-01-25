#ifndef SERVO_H
#define SERVO_H

#include "../Frame/Frame.h"
#include <wiringPi.h>
#include <softPwm.h>


float toRad( float deg);
float toDeg( float rad);



class Servo 
{
	protected :
	Frame* frame;
	float min;
	float max;
	float zerooffset;
	//additionnal offset that is to be added to the given value in order to go
	//from the zero of the servo to the zero of the frame.
	// SERVO --> FRAME offset.
	
	int pin;
	
	public :
	
	bool inversed;
	
	Servo(int pin_, int idx, const Mat<float>& se3Init, float zerooffset_ = 120.0f, float min = 0.0f, float max = 240.0f);
	~Servo();
	
	//---------------------
	void set(float val);
	float get();	
	
	Mat<float> getSE3();
	//---------------------
	
};

#endif
