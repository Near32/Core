#ifndef SERVOEXP_H
#define SERVOEXP_H

#include "../Servo/Servo.h"
#include "../../EXP/EXP.h"
#include "../../FUNC/FUNC.h"
#include "../../VAR/VAR.h"
#include "../../Operations/Operations.h"

class ServoEXP : public Servo
{
	private :
	FrameEXP* frameEXP;
	
	public :
	
	ServoEXP(const VAR& variable_, int pin_, int idx, const Mat<float>& se3Init, float zerooffset_ = 0.0f, float min = -120.0f, float max = 120.0f);
	~ServoEXP();
};

#endif
