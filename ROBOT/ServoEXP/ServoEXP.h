#ifndef SERVOEXP_H
#define SERVOEXP_H

#include "../Servo/Servo.h"
#include "../../EXP/EXP.h"
#include "../../FUNC/FUNC.h"
#include "../../VAR/VAR.h"
#include "../../Operations/Operations.h"
#include "../FrameEXP/FrameEXP.h"

class ServoEXP : public Servo
{
	private :
	FrameEXP* frameEXP;
	
	public :
	
	ServoEXP(const VAR& variable_, int pin_, int idx, const Mat<float>& se3Init, float zerooffset_ = 120.0f, float min = 0.0f, float max = 240.0f);
	~ServoEXP();
	
	
	int getVARid()	const	{	return (frameEXP!=NULL? frameEXP->getVARid() : 0);	}
	Mat<EXP> getT()	const	{	return (frameEXP!=NULL? frameEXP->getT() : Mat<EXP>(1,1) );	}
	
	void setVARVALUE();
};

#endif
