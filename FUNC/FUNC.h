#ifndef FUNC_H
#define FUNC_H

#include "../EXP/EXP.h"

enum FUNCType{
	FTzero,
	FTcst,
	FTone,
	FTmone,
	FTid,
	FTminus,
	FTsquare,
	FTinverse,
	FTsin,
	FTcos,
	FTasin,
	FTacos
};

class FUNC : public EXP
{
	/* can only have one argument */
	
	protected :
	FUNCType ftype;
	float param;
	
	public :
	
	FUNC(const FUNCType& ftype_ = FTid, const float param_ = 0.0f);
	FUNC(const FUNC* f);
	FUNC(const FUNC& f);
	~FUNC();
	
	virtual float evaluate()	override;
	virtual std::string toString()	override;
	virtual bool doesContainVAR()	override;
	
	FUNC operator=(const FUNC& f);
	
	FUNCType getFType()	const	{	return this->ftype;	}
	float getParam()	const	{	return this->param;	}
	
	void setArg(EXP* exp);
	void setParam(float p);
	

};

EXP sin(const EXP& e);
EXP asin(const EXP& e);
EXP cos(const EXP& e);
EXP acos(const EXP& e);

EXP& regularize(EXP* e, bool& goOn);
EXP regw(EXP& exp);

bool seek4Zeros(EXP* e);
void raiseUpPRODUCT( EXP* prod);
void raiseUpSUM( EXP* sum);
void deleteIdentity( EXP* exp);
void replaceCST( EXP* exp);

#endif

