#include "EXP.h"

#include "../FUNC/FUNC.h"
#include "../VAR/VAR.h"

EXP::EXP(const int& int_) : otype(EOId), hasChanged(false), containsVAR(false), type(ETEXP), parent(NULL)
{
	this->arg.clear();
	EXP* temp = new FUNC(FTcst);
	this->arg.insert( this->arg.begin(), temp );
	//create a Id( Fcst = 0) = ZERO function ... EXP --> Fcst=0;
}

EXP::EXP(const EXPOp& otype_) : otype(otype_), type(ETEXP), parent(NULL)
{
	this->arg.clear();
}

EXP::EXP(const EXP& e)
{
	this->hasChanged = true;
	this->containsVAR = e.getContainsVAR();
	
	this->type = e.getType();
	this->otype = e.getOType();
	this->parent = e.getParent();
	
	int idx = 0;
	EXP* a = NULL;
	//we do not know how many arg node e has...
	while( (a=e.getArg(idx)) != NULL)
	{
		EXP* temp;
		
		switch(a->getType())
		{
			case ETEXP :
			temp = new EXP( *a );
			break;
			
			case ETFUNC :
			temp = (EXP*)new FUNC( *((FUNC*)a) );
			break;
			
			case ETVAR :
			temp = (EXP*)new VAR( *((VAR*)a) );
			break;
		}
		
		temp->setParent(this);
		//we attach this arg node to *this.
		
		this->arg.insert( this->arg.end(), temp); 
		idx++;
	}
}

EXP::EXP(const FUNC& f)
{
	this->hasChanged = true;
	this->containsVAR = f.getContainsVAR();
	
	this->type = ETEXP;
	this->otype = EOId;
	this->parent = f.getParent();
	
	EXP* temp = (EXP*)new FUNC(f); 
	temp->setParent(this);
	//we attach this arg node to *this.
		
	this->arg.insert( this->arg.end(), temp); 
}

EXP::EXP(const VAR& v)
{
	this->hasChanged = false;
	this->containsVAR = true;
	
	this->type = ETEXP;
	this->otype = EOId;
	this->parent = v.getParent();
	
	EXP* temp = (EXP*)new VAR(v); 
	temp->setParent(this);
	//we attach this arg node to *this.
		
	this->arg.insert( this->arg.end(), temp); 
}

EXP::~EXP()
{
	if(this->type != ETVAR)
	{
		unsigned int nbrArg = this->arg.size();
		for(int i=0;i<nbrArg;i++)	
		{
			delete this->arg[i];	
		}
	}
}

	
float EXP::evaluate()
{
	if(arg.size() >= 1)
	{
		float r=0.0f;
		if(this->otype==EOProduct)	r = 1.0f;
		
		for(int i=this->arg.size();i--;)
		{
			try
			{
				switch(this->otype)
				{
					case EOId :
					r=arg[0]->evaluate();
					break;
					
					case EOSum :
					r+=arg[i]->evaluate();
					break;
					
					case EOProduct :
					r*=arg[i]->evaluate();
					break;
				}
				
			}
			catch(std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		
		return r;
	}
	else
	{
		throw;
	}
}

std::string EXP::toString()
{
	if(arg.size() >= 1)
	{
		std::string r=arg[0]->toString();
		bool parenthesis = true;
		bool bracket = false;
		
		for(int i=1;i<this->arg.size();i++)
		{
			try
			{
				switch(this->otype)
				{
					case EOId :
					parenthesis = false;
					r=arg[0]->toString();
					break;
					
					case EOSum :
					bracket = false;
					parenthesis = true;
					r += std::string( " + ") + arg[i]->toString();
					break;
					
					case EOProduct :
					bracket = true;
					parenthesis = false;
					r += std::string( " * ") + arg[i]->toString();
					break;
				}
				
			}
			catch(std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		
		if(parenthesis)	
		{
			r = std::string( "(")+r+std::string(")");
		}
		
		if(bracket)
		{
			r = std::string( "[")+r+std::string("]");
		}
		
		return r;
	}
	else
	{
		return std::string( " [NO ARG] " );
	}
}

void EXP::addArg( EXP* arg)
{
	//arg is a pointer to a EXP that will now be entirely handled by this.
	arg->setParent(this);
	this->arg.insert( this->arg.end(), arg);
	
	this->hasChanged = true;
}

void EXP::deleteArg( int idx)
{
	if( arg.size() > idx && idx >= 0)
	{
		delete arg[idx];
		arg.erase(arg.begin()+idx);
		
		this->hasChanged = true;
	}
	else
	{
		throw;
	}
}

void EXP::deleteArg( EXP* pe)
{
	bool deleted = false;
	
	for(int i=this->arg.size();i--;)
	{
		if( pe == this->arg[i] )
		{
			delete this->arg[i];
			this->arg.erase(this->arg.begin()+i);
			
			this->hasChanged = true;
			deleted = true;
			
			break;
		}
	}
	
	if(deleted == false)
	{
		std::cerr << "DELETION : unable to localize the given node...." << std::endl;
	}
}

void EXP::replaceArg( EXP* pe2delete, EXP* pe2insert)
{
	//pe2delete is a pointer that is already being handled by this, assumingly...
	//pe2insert is a pointer that will be entirely handled by this, in the futur.
	//no copy are being done...
	for(int i=this->arg.size();i--;)
	{
		if( pe2delete == this->arg[i] )
		{
			delete this->arg[i];
			pe2insert->setParent(this);
			this->arg[i] = pe2insert;
			
			this->hasChanged = true;
			
			break;
		}
	}
}

void EXP::setParent( EXP* p)
{
	this->parent = p;
}

EXP EXP::operator=(const EXP& exp)
{
	this->hasChanged = true;
	this->containsVAR = exp.getContainsVAR();
	
	this->type = exp.getType();
	this->otype = exp.getOType();
	this->parent = exp.getParent();
	this->arg.clear();
	int idx = 0;
	EXP* a = NULL;
	
	while( (a = exp.getArg(idx)) != NULL)
	{
		switch(a->getType())
		{
			case ETEXP :
			{
				EXP* temp = new EXP( *a);
				temp->setParent(this);
				this->arg.insert( this->arg.end(), temp );
			}
			break;
			
			case ETFUNC :
			{
				FUNC* temp = new FUNC( *((FUNC*)a) );
				temp->setParent(this);
				this->arg.insert( this->arg.end(), (EXP*)temp );
			}  
			break;
			
			case ETVAR :
			{
				VAR* temp = new VAR( *((VAR*)a) );
				temp->setParent(this);
				this->arg.insert( this->arg.end(), (EXP*)temp );
			}
			break; 
			
		}
		
		idx++;
	}
	
	return *this;
}


EXP EXP::operator*(const EXP& exp)
{
	if( (this->type == ETFUNC && ((FUNC*)this)->getFType() == FTzero ) || (this->type == ETEXP && this->otype == EOId && this->arg[0]->getType() == ETFUNC && ((FUNC*)(this->arg[0]))->getFType() == FTzero ) || ( exp.getType() == ETFUNC && ((FUNC&)(exp)).getFType() == FTzero) || (exp.getType() == ETEXP && exp.getOType() == EOId && exp.getArg(0)->getType() == ETFUNC && ((FUNC*)(exp.getArg(0)))->getFType() == FTzero ) )
	{
		EXP r(EOId);
		FUNC fzero(FTzero);
		attach(r, (EXP&)fzero);
		return r;
	}
		
	EXP r(EOProduct);
	
	//r.addArg(this);
	switch(this->type)
	{
		case ETEXP :
		r.addArg( new EXP(*this) );
		break;
		
		case ETFUNC :
		r.addArg( (EXP*)new FUNC( *((FUNC*)this) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)this) ) );
		break;
	}
	
	//r.addArg(&exp);
	switch(exp.getType())
	{
		case ETEXP :
		r.addArg( new EXP(exp) );
		break;
		
		case ETFUNC :
		r.addArg( (EXP*)new FUNC( *((FUNC*)&exp) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)&exp) ) );
		break;
	}
	
	
	//return regw(r);
	return r;
}

EXP EXP::operator+(const EXP& exp)
{
	EXP r(EOSum);
	int zeroFOUND = 0;
	
	//r.addArg(this);
	switch(this->type)
	{
		case ETEXP :
		if( this->otype == EOId )
		{
			EXP* temp = this->arg[0];
			if( temp->getType() == ETFUNC && ((FUNC*)(temp))->getFType() == FTzero )
			{
				zeroFOUND++;
				break;
			}
		}
		
		r.addArg( new EXP(*this) );		
		break;
		
		case ETFUNC :
		
		if( ((FUNC*)(this))->getFType() == FTzero )
		{
			zeroFOUND++;
			break;
		}
		
		r.addArg( (EXP*)new FUNC( *((FUNC*)this) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)this) ) );
		break;
	}
	
	//r.addArg(&exp);
	switch(exp.getType())
	{
		case ETEXP :
		if( exp.getOType() == EOId )
		{
			EXP* temp = exp.getArg(0);
			if( temp->getType() == ETFUNC && ((FUNC*)(temp))->getFType() == FTzero )
			{
				zeroFOUND++;
				break;
			}
		}
		
		//if we go on, it means that we didn't find a zero in exp :
		if(zeroFOUND == 1)
		{
			r=EXP(EOId);
		}
		
		r.addArg( new EXP(exp) );
		break;
		
		case ETFUNC :
		
		if( ((FUNC&)(exp)).getFType() == FTzero )
		{
			zeroFOUND++;
			break;
		}
		
		//if we go on, it means that we didn't find a zero in exp :
		if(zeroFOUND == 1)
		{
			r=EXP(EOId);
		}
		
		r.addArg( (EXP*)new FUNC( *((FUNC*)&exp) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)&exp) ) );
		break;
	}
	
	if(zeroFOUND == 2)
	{
		//entering here means that none of this or exp was interesting in this sum :
		r=EXP(EOId);
		FUNC fzero(FTzero);
		attach(r, (EXP&)fzero);
	}
	
	//return regw(r);
	return r;
}

EXP EXP::operator+=(const EXP& exp)
{
	EXP r(EOSum);
	int zeroFOUND = 0;
	
	//r.addArg(this);
	switch(this->type)
	{
		case ETEXP :
		if( this->otype == EOId )
		{
			EXP* temp = this->arg[0];
			if( temp->getType() == ETFUNC && ((FUNC*)(temp))->getFType() == FTzero )
			{
				zeroFOUND++;
				break;
			}
		}
		
		r.addArg( new EXP(*this) );		
		break;
		
		case ETFUNC :
		
		if( ((FUNC*)(this))->getFType() == FTzero )
		{
			zeroFOUND++;
			break;
		}
		
		r.addArg( (EXP*)new FUNC( *((FUNC*)this) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)this) ) );
		break;
	}
	
	//r.addArg(&exp);
	switch(exp.getType())
	{
		case ETEXP :
		if( exp.getOType() == EOId )
		{
			EXP* temp = exp.getArg(0);
			if( temp->getType() == ETFUNC && ((FUNC*)(temp))->getFType() == FTzero )
			{
				zeroFOUND++;
				break;
			}
		}
		
		//if we go on, it means that we didn't find a zero in exp :
		if(zeroFOUND == 1)
		{
			r=EXP(EOId);
		}
		
		r.addArg( new EXP(exp) );
		break;
		
		case ETFUNC :
		
		if( ((FUNC&)(exp)).getFType() == FTzero )
		{
			zeroFOUND++;
			break;
		}
		
		//if we go on, it means that we didn't find a zero in exp :
		if(zeroFOUND == 1)
		{
			r=EXP(EOId);
		}
		
		r.addArg( (EXP*)new FUNC( *((FUNC*)&exp) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)&exp) ) );
		break;
	}
	
	if(zeroFOUND == 2)
	{
		//entering here means that none of this or exp was interesting in this sum :
		r=EXP(EOId);
		FUNC fzero(FTzero);
		attach(r, (EXP&)fzero);
	}
	
	//*((EXP*)this) = regw(r);
	*((EXP*)this) = r;
	
	return *this;
}


EXP EXP::operator-( const EXP& exp)
{
	EXP r(EOSum);
	
	//r.addArg(this);
	switch(this->type)
	{
		case ETEXP :
		r.addArg( new EXP(*this) );
		break;
		
		case ETFUNC :
		r.addArg( (EXP*)new FUNC( *((FUNC*)this) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)this) ) );
		break;
	}
	
	FUNC temp(FTminus);
	
	//temp->setArg(&exp);
	switch(exp.getType())
	{
		case ETEXP :
		temp.setArg( new EXP(exp) );
		break;
		
		case ETFUNC :
		temp.setArg( (EXP*)new FUNC( *((FUNC*)&exp) ) );
		break;
		
		case ETVAR :
		temp.setArg( (EXP*)new VAR( *((VAR*)&exp) ) );
		break;
	}
	
	r.addArg( (EXP*)new FUNC(temp) );
	
	//return regw(r);
	return r;
}


EXP EXP::operator/( const EXP& exp)
{
	if( this->type == ETFUNC && ((FUNC*)this)->getFType() == FTzero )
	{
		EXP r(EOId);
		FUNC fzero(FTzero);
		attach(r, (EXP&)fzero);
		return r;
	}
	
	EXP r(EOProduct);
	
	//r.addArg(this);
	switch(this->type)
	{
		case ETEXP :
		r.addArg( new EXP(*this) );
		break;
		
		case ETFUNC :
		r.addArg( (EXP*)new FUNC( *((FUNC*)this) ) );
		break;
		
		case ETVAR :
		r.addArg( (EXP*)new VAR( *((VAR*)this) ) );
		break;
	}
	
	FUNC temp(FTinverse);
	
	//temp->setArg(&exp);
	switch(exp.getType())
	{
		case ETEXP :
		temp.setArg( new EXP(exp) );
		break;
		
		case ETFUNC :
		temp.setArg( (EXP*)new FUNC( *((FUNC*)&exp) ) );
		break;
		
		case ETVAR :
		temp.setArg( (EXP*)new VAR( *((VAR*)&exp) ) );
		break;
	}
	
	r.addArg( (EXP*)new FUNC(temp) );
	
	//return regw(r);
	return r;
}


EXP* EXP::getParent()	const
{
	return this->parent;
}

EXP* EXP::getArg(int idx)	const	
{
	unsigned int nbrArg = this->arg.size();
	EXP* r = NULL;
	
	if( idx < nbrArg && idx >= 0)
	{
		r = this->arg[idx];
	}
	return r;	
}
	
EXPType EXP::getType()	const
{
	return this->type;
}

EXPOp EXP::getOType()	const	
{
	return this->otype;	
}


unsigned int EXP::getNBRArg()	const
{
	return this->arg.size();	
}

bool EXP::getContainsVAR()	const
{
	return this->containsVAR;
}


bool EXP::doesContainVAR()
{
	if(this->hasChanged)
	{
		int nbrarg = this->arg.size();
		
		if( nbrarg != 0)
		{
			for(int i=nbrarg;i--;)
			{
				if( arg[i]->doesContainVAR() )
				{
					this->containsVAR = true;
					break;
				}
			}
		}
		else
		{
			//TODO:assume no problem for now on...
			this->containsVAR = false;
		}
		
		this->hasChanged = false;
	}
	
	return this->containsVAR;
}

void attach(EXP& parent, EXP& node)
{
	//copy are being done and then entirely handled by the parent, there is no problem.
	switch(node.getType())
	{
		case ETEXP :
		parent.addArg( new EXP(node) );
		break;
		
		case ETFUNC :
		parent.addArg( (EXP*)new FUNC( *((FUNC*)&node) ) );
		break;
		
		case ETVAR :
		parent.addArg( (EXP*)new VAR( *((VAR*)&node) ) );
		break;
	}
}



