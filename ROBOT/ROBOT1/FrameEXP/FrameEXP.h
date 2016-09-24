#ifndef FRAMEEXP_H
#define FRAMEEXP_H

#include "../Frame/Frame.h"
#include "../../../Core1/EXP.h"
#include "../../../Core1/Operations/Operations.h"


class FrameEXP : public Frame
{
	private :
	EXP<float>* variable;
	Mat<EXP<float> >* T;
	
	void generateT();
	
	public :
	
	FrameEXP();
	FrameEXP(const EXP<float>& variable_, int idxParameter_, const Mat<float>& se3Init);
	~FrameEXP();
	
	//-----------------------------
	int getVARid()	const	{	return (variable!=NULL? variable->VARID : 0);	}
	Mat<EXP<float> > getT()	const	{	return (T!=NULL? *T : Mat<EXP<float> >(1,1) );	}
	
	void setVAR(const EXP<float>& v);
	EXP<float>& getVAR() const 	{	return *variable;	}
};

#endif
