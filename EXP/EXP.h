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
	void deleteArg( int idx);
	void deleteArg( EXP* pe);
	void replaceArg( EXP* pe2delete, EXP* pe2insert);
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
	EXP* getParent()	const;
	EXP* getArg(int idx)	const;
	EXPType getType()	const;
	EXPOp getOType()	const;
	
	unsigned int getNBRArg()	const;
	
};


void attach(EXP& parent, EXP& node);

#endif


