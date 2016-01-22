#include "VAR.h"

#include "../FUNC/FUNC.h"

int idVARCOUNT = 0;

VAR::VAR(float val)	: EXP(), id(idVARCOUNT), value(val), valueREF(value) 
{
	this->containsVAR = true;
	this->type = ETVAR;
	idVARCOUNT++;
}

VAR::VAR(const VAR& v) : EXP(), id(v.id), valueREF( v.valueREF)
{	
	this->containsVAR = true;
	this->parent = v.getParent();

	//v has no arg node :
	this->arg.clear();
	
	this->type = ETVAR;
}

VAR::~VAR()
{

}

float VAR::evaluate()
{
	this->value = this->valueREF;
	return this->value;
}

std::string VAR::toString()
{
	this->value = this->valueREF;
	std::string r( "X");
	std::ostringstream ssi;
	ssi << this->id;
	r += ssi.str() + std::string( "=");
	
	std::ostringstream ss;
	ss << this->value;
	
	return r+ss.str();
}

VAR VAR::operator=(const VAR& v)
{
	this->containsVAR = true;
	//copies of the id :
	this->id = v.id;
	//make a copy of the VAR v and v.valueREF.
	this->parent = v.getParent();
	this->valueREF = v.valueREF;
	//copies made.
	//regularization :
	this->value = this->valueREF;
	
	//var has no arg node :
	this->arg.clear();
}

void VAR::setValue(const float val)
{
	this->value = val;
	this->valueREF = this->value;
	// if this function is used, then valueREF is a reference to value.
	// that way, this VAR is  depending on its own and it means that there
	// are copies/references of itself in circulation.
	// copies are made thanks to the operator= or the copy constructor.
}

bool VAR::doesContainVAR()
{
	return true;
}




