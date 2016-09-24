#include "ServoEXP.h"

ServoEXP::ServoEXP(const EXP<float>& variable_, int pin_, int idx, const Mat<float>& se3Init, float zerooffset_, float min, float max) : Servo( pin_, idx, se3Init, zerooffset_, min, max)
{
		frameEXP = new FrameEXP(variable_, idx, se3Init);
		
}

ServoEXP::~ServoEXP()
{
	if(frameEXP != NULL)	delete frameEXP;
}


void ServoEXP::setVARVALUE()
{
	this->set( toDeg(this->frameEXP->getVAR().evaluate()) );
	this->frame->setParam( this->frameEXP->getVAR().evaluate() );
}