#ifndef FRAMEEXP_H
#define FRAMEEXP_H

#include "../Frame/Frame.h"
#include "../../EXP/EXP.h"
#include "../../FUNC/FUNC.h"
#include "../../VAR/VAR.h"
#include "../../Operations/Operations.h"


class FrameEXP : public Frame
{
	private :
	VAR* variable;
	Mat<EXP>* T;
	
	void generateT();
	
	public :
	
	FrameEXP();
	FrameEXP(const VAR& variable_, int idxParameter_, const Mat<float>& se3Init);
	~FrameEXP();
	
	//-----------------------------
	int getVARid()	const	{	return (variable!=NULL? variable->id : 0);	}
	Mat<EXP> getT()	const	{	return (T!=NULL? *T : Mat<EXP>(1,1) );	}
	
	void setVAR(const VAR& v);
};

#endif
