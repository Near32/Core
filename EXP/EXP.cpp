#include "EXP.h"

#include "../FUNC/FUNC.h"
#include "../VAR/VAR.h"

EXP::EXP(const int& int_) : otype(EOId), type(ETEXP), parent(NULL)
{
	this->arg.clear();
	this->addArg( new FUNC(FTcst) );
	//create a Id( Fcst = 0) = ZERO function ... EXP --> Fcst=0;
}

EXP::EXP(const EXPOp& otype_) : otype(otype_), type(ETEXP), parent(NULL)
{
	this->arg.clear();
}

EXP::EXP(const EXP& e)
{
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
			//temp = new EXP;
			//*temp = *a;
			break;
			
			case ETFUNC :
			temp = (EXP*)new FUNC( *((FUNC*)a) );
			//temp = (EXP*)new FUNC;
			//*((FUNC*)temp) = *((FUNC*)a);
			break;
			
			case ETVAR :
			temp = (EXP*)new VAR( *((VAR*)a) );
			//temp = (EXP*)new VAR;
			//*((VAR*)temp) = *((VAR*)a);
			break;
		}
		
		temp->setParent(this);
		//we attach this arg node to *this.
		
		this->arg.insert( this->arg.end(), temp); 
		idx++;
	}
}

EXP::~EXP()
{
	if(this->type != ETVAR)
	{
		for(int i=this->arg.size();i--;)	delete this->arg[i];	
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
		
		for(int i=1;i<this->arg.size();i++)
		{
			try
			{
				switch(this->otype)
				{
					case EOId :
					//parenthesis = false;
					r=arg[0]->toString();
					break;
					
					case EOSum :
					parenthesis = false;
					r += std::string( " + ") + arg[i]->toString();
					break;
					
					case EOProduct :
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
			return std::string( "(")+r+std::string(")");
		}
		else
		{
			return r;
		}
	}
	else
	{
		throw;
	}
}

void EXP::addArg( EXP* arg)
{
	arg->setParent(this);
	this->arg.insert( this->arg.end(), arg);
}

void EXP::setParent( EXP* p)
{
	this->parent = p;
}

EXP EXP::operator=(const EXP& exp)
{
	this->type = exp.getType();
	this->otype = exp.getOType();
	this->parent = exp.getParent();
	this->arg.clear();
	int idx = 0;
	EXP* a;
	
	while( (a = exp.getArg(idx)) != NULL)
	{
		switch(a->getType())
		{
			case ETEXP :
			this->arg.insert( this->arg.end(), new EXP( *a) );
			break;
			
			case ETFUNC :
			this->arg.insert( this->arg.end(), (EXP*)new FUNC( *((FUNC*)a) ) );  
			break;
			
			case ETVAR :
			this->arg.insert( this->arg.end(), (EXP*)new VAR( *((VAR*)a) ) );
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
	
	return r;
}


void attach(EXP& parent, EXP& node)
{
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



