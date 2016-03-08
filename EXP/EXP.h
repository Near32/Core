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

class FUNC;
class VAR;

class EXP
{
	private :
	EXPOp otype;
	
	protected :
	
	bool hasChanged;
	bool containsVAR;
	EXPType type;
	
	//liste doublement chainee.
	std::vector<EXP*> arg;
	EXP* parent;
	
	public :
	
	EXP(const int& int_);
	EXP(const EXPOp& otype_ = EOId);
	EXP(const EXP& e);
	EXP(const FUNC& f);
	EXP(const VAR& v);
	/*EXP( FUNC f);
	EXP( VAR v);*/
	~EXP();
	
	virtual float evaluate();
	virtual std::string toString();
	virtual bool doesContainVAR();
	
	void addArg( EXP* arg);
	void deleteArg( int idx);
	void deleteArg( EXP* pe);
	void replaceArg( EXP* pe2delete, EXP* pe2insert);
	void setParent( EXP* p);
	
	EXP operator=(const EXP& exp);
	EXP operator*( const EXP& exp);
	EXP operator*=( const EXP& exp);
	EXP operator*=( const float& value);
	EXP operator*( const float& value);
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
	bool getContainsVAR()	const;
	
	unsigned int getNBRArg()	const;
	
};


void attach(EXP& parent, EXP& node);
void attach(EXP* parent, EXP* node);
void reattach(EXP* parent, EXP* node);


#endif


