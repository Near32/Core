#ifndef FUNC_H
#define FUN_H

#include "../EXP/EXP.h"

enum FUNCType{
	FTzero,
	FTid,
	FTminus,
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
	
	public :
	
	FUNC(const FUNCType& ftype_ = FTid);
	FUNC(const FUNC* f);
	~FUNC();
	
	virtual float evaluate()	override;
	FUNC operator=(const FUNC& f);
	
	FUNCType getFType()	const	{	return this->ftype;	}
	
	void setArg(EXP* exp);

};

EXP sin(const EXP& e);
EXP asin(const EXP& e);
EXP cos(const EXP& e);
EXP acos(const EXP& e);

#endif

