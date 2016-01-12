#ifndef EXP_H
#define EXP_H

#include <iostream>
#include <vector>
#include <exception>
#include <math.h>
#include <string>
#include <sstream>

enum EXPType {
	ETEXP,
	ETFUNC,
	ETVAR
};

enum EXPOp{
	EOId,
	EOSum,
	EOProduct
};

class EXP
{
	private :
	EXPOp otype;
	
	protected :
	
	EXPType type;
	
	//liste doublement chainee.
	std::vector<EXP*> arg;
	EXP* parent;
	
	public :
	
	EXP(const int& int_);
	EXP(const EXPOp& otype_ = EOId);
	EXP(const EXP& e);
	~EXP();
	
	virtual float evaluate();
	virtual std::string toString();
	
	void addArg( EXP* arg);
	void setParent( EXP* p);
	
	EXP operator=(const EXP& exp);
	EXP operator*( const EXP& exp);
	EXP operator+( const EXP& exp);
	EXP operator+=(const EXP& exp);
	EXP operator-( const EXP& exp);
	EXP operator/( const EXP& exp);
	
	/*
	EXP operator*( EXP exp);
	EXP operator+( EXP exp);
	EXP operator-( EXP exp);
	EXP operator/( EXP exp);
	*/
	
	//--------------------------
	EXP* getParent()	const	{	return this->parent;	}
	EXP* getArg(int idx)	const	{	return (idx<arg.size()? arg[idx] : NULL);	}	
	EXPType getType()	const	{	return type;	}
	EXPOp getOType()	const	{	return otype;	}
	
	int getNBRArg()	const	{	return arg.size();	}
	
};


void attach(EXP& parent, EXP& node);

#endif


