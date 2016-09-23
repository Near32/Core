#ifndef SERVOEXP_H
#define SERVOEXP_H

#include "../Servo/Servo.h"
#include "../FrameEXP/FrameEXP.h"

class ServoEXP : public Servo
{
	private :
	FrameEXP* frameEXP;
	
	public :
	
	ServoEXP(const EXP<float>& variable_, int pin_, int idx, const Mat<float>& se3Init, float zerooffset_ = 120.0f, float min = 0.0f, float max = 240.0f);
	~ServoEXP();
	
	
	int getVARid()	const	{	return (frameEXP!=NULL? frameEXP->getVARid() : 0);	}
	Mat<EXP<float> > getT()	const	{	return (frameEXP!=NULL? frameEXP->getT() : Mat<EXP<float> >(1,1) );	}
	
	void setVARVALUE();
};

#endif
