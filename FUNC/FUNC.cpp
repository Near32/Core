#include "FUNC.h"

#include "../VAR/VAR.h"

FUNC::FUNC(const FUNCType& ftype_, const float param_) : EXP(), ftype(ftype_), param(param_)
{
	this->type = ETFUNC;
}

FUNC::FUNC(const FUNC* f)
{
	this->type = ETFUNC;
	this->ftype = f->getFType();
	
	this->parent = f->getParent();
	this->param = f->getParam();
	
	//f has only one arg node :
	EXP* a = f->getArg(0);
	
	EXP* temp;
		
	switch(a->getType())
	{
	
		case ETEXP :
		temp = new EXP();
		*temp = *a;
		break;
		
		case ETFUNC :
		temp = (EXP*)new FUNC();
		*((FUNC*)temp) = *((FUNC*)a);
		break;
		
		case ETVAR :
		temp = (EXP*)new VAR();
		*((VAR*)temp) = *((VAR*)a);
		break;
	}
	
	temp->setParent(this);
	//we attach this arg node to *this.
	
	this->arg.clear();		
	this->arg.insert( this->arg.end(), temp); 
}


FUNC::~FUNC()
{

}

float FUNC::evaluate()
{
	switch(this->ftype)
	{
		case FTzero :
		
		return 0.0f;
		
		break;
		
		case FTcst :
		
		return this->param;
		
		break;
		
		case FTone :
		
		return 1.0f;
		
		break;
		
		case FTmone :
		
		return -1.0f;
		
		break;
		
		case FTid :
		
		if(this->arg.size() == 1)
		{
			return this->arg[0]->evaluate();
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTminus :
		
		if(this->arg.size() == 1)
		{
			return (-1.0f)*(this->arg[0]->evaluate());
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTsquare :
		
		if(this->arg.size() == 1)
		{
			float r = this->arg[0]->evaluate();
			return r*r;
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTinverse :
		
		if(this->arg.size() == 1)
		{
			float r = this->arg[0]->evaluate();
			if (r!=0.0f)
			{
				return 1.0f/r;
			}
			else
			{
				r=1e-20f;
				std::cerr << "DIVISION PAR ZERO .... " << std::endl;
				return 1.0f/r;
			} 
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTsin:
		
		if(this->arg.size() == 1)
		{
			return sin(this->arg[0]->evaluate());
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTasin:
		
		if(this->arg.size() == 1)
		{
			return asin(this->arg[0]->evaluate());
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTcos:
		
		if(this->arg.size() == 1)
		{
			return cos(this->arg[0]->evaluate());
		}
		else
		{
			throw;
		}
		
		break;
		
		case FTacos:
		
		if(this->arg.size() == 1)
		{
			return acos(this->arg[0]->evaluate());
		}
		else
		{
			throw;
		}
		
		break;
	}
}

std::string FUNC::toString()
{
	switch(this->ftype)
	{
		case FTzero :
		
		return std::string( " 0 ");
		
		break;
		
		case FTcst :
		{
		std::ostringstream ss;
		ss << this->param;
		return std::string( " ") + ss.str() + std::string( " " );
		}
		break;
		
		case FTone :
		{
		return std::string( " 1 ");
		}
		break;
		
		case FTmone :
		{
		return std::string( " (-1) " );
		}
		break;
		
		case FTid :
		{
		if(this->arg.size() == 1)
		{
			return this->arg[0]->toString();
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTminus :
		{
		if(this->arg.size() == 1)
		{
			return std::string( " (-1)*") + std::string( this->arg[0]->toString() );
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTsquare :
		{
		if(this->arg.size() == 1)
		{
			std::string r = this->arg[0]->toString();
			return r+std::string( "*")+r;
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTinverse :
		{
		if(this->arg.size() == 1)
		{
			std::string r = this->arg[0]->toString();
			return std::string( "1/")+r;
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTsin:
		{
		if(this->arg.size() == 1)
		{
			return std::string( " sin( ")+this->arg[0]->toString()+std::string( ") ");
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTasin:
		{
		if(this->arg.size() == 1)
		{
			return std::string( " asin( ")+this->arg[0]->toString()+std::string( ") ");
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTcos:
		{
		if(this->arg.size() == 1)
		{
			return std::string( " cos( ")+this->arg[0]->toString()+std::string( ") ");
		}
		else
		{
			throw;
		}
		}
		break;
		
		case FTacos:
		{
		if(this->arg.size() == 1)
		{
			return std::string( " acos( ")+this->arg[0]->toString()+std::string( ") ");
		}
		else
		{
			throw;
		}
		}
		break;
	}
}

void FUNC::setParam(float p)
{
	this->param = p;
}

FUNC FUNC::operator=(const FUNC& f)
{
	//FUNCType :
	this->ftype = f.getFType();
	
	
	this->parent = f.getParent();
	this->arg.clear();
	
	EXP* a = f.getArg(0);
	//f has only one arg node :
	
	switch(a->getType())
	{
		case ETEXP :
		this->arg.insert( this->arg.end(), new EXP(*a) );
		break;
		
		case ETFUNC :
		this->arg.insert( this->arg.end(), (EXP*)new FUNC( *((FUNC*)a) ) );  
		break;
		
		case ETVAR :
		this->arg.insert( this->arg.end(), (EXP*)new VAR( *((VAR*)a) ) );
		break; 
		
	}
	
	return *this;
}


void FUNC::setArg(EXP* exp)
{
	// can only have one argument :
	for(int i=this->arg.size();i--;)	delete this->arg[i];
	this->arg.clear();
	
	//--------------------
	this->addArg(exp);
}


EXP sin(const EXP& e)
{
	EXP r(EOId);
	FUNC sin(FTsin);
	
	//sin.setArg(&e);
	switch(e.getType())
	{
		case ETEXP :
		sin.setArg( new EXP(e) );
		break;
		
		case ETFUNC :
		sin.setArg( (EXP*)new FUNC( *((FUNC*)&e) ) );
		break;
		
		case ETVAR :
		sin.setArg( (EXP*)new VAR( *((VAR*)&e) ) );
		break;
	}
	
	r.addArg( (EXP*)new FUNC( sin) );
	return r;
}

EXP asin(const EXP& e)
{
	EXP r(EOId);
	FUNC asin(FTasin);
	
	//sin.setArg(&e);
	switch(e.getType())
	{
		case ETEXP :
		asin.setArg( new EXP(e) );
		break;
		
		case ETFUNC :
		asin.setArg( (EXP*)new FUNC( *((FUNC*)&e) ) );
		break;
		
		case ETVAR :
		asin.setArg( (EXP*)new VAR( *((VAR*)&e) ) );
		break;
	}
	
	r.addArg( (EXP*)new FUNC( asin) );
	return r;
}

EXP cos(const EXP& e)
{
	EXP r(EOId);
	FUNC cos(FTcos);
	
	//sin.setArg(&e);
	switch(e.getType())
	{
		case ETEXP :
		cos.setArg( new EXP(e) );
		break;
		
		case ETFUNC :
		cos.setArg( (EXP*)new FUNC( *((FUNC*)&e) ) );
		break;
		
		case ETVAR :
		cos.setArg( (EXP*)new VAR( *((VAR*)&e) ) );
		break;
	}
	
	r.addArg( (EXP*)new FUNC( cos) );
	return r;
}

EXP acos(const EXP& e)
{
	EXP r(EOId);
	FUNC acos(FTacos);
	
	//sin.setArg(&e);
	switch(e.getType())
	{
		case ETEXP :
		acos.setArg( new EXP(e) );
		break;
		
		case ETFUNC :
		acos.setArg( (EXP*)new FUNC( *((FUNC*)&e) ) );
		break;
		
		case ETVAR :
		acos.setArg( (EXP*)new VAR( *((VAR*)&e) ) );
		break;
	}
	
	r.addArg( (EXP*)new FUNC( acos) );
	return r;
}


EXP regularize(EXP& e)
{
	if( e.getType() == ETFUNC && ((FUNC&)e).getFType() == FTzero)
	{
		return e;
	}
	
	std::vector<EXP*> file;
	std::vector<int> fileIDX;
	file.insert( file.end(), &e);
	fileIDX.insert( fileIDX.end(), -1);
	EXP* p = NULL;
	
	while( file.size() )
	{
		p=file[0];
		
		//------------------------------
		
		switch(p->getType())
		{
			case ETEXP :
			{
				if(p->getOType() == EOId)
				{
					
				}
			}
			break;
		}
		
		//------------------------------
		
		EXP* temp = p->getArg(0);
		int idxArg = 1;
		while( temp != NULL)
		{
			file.insert( file.end(), temp);
			fileIDX.insert( fileIDX.end(), idxArg-1);
			
			temp = p->getArg(idxArg);
			idxArg++;
		}
		//parcour en profondeur.
		
		//------------------------------		
		//------------------------------
		
		file.erase(file.begin());
		fileIDX.erase(fileIDX.begin());
	}
	
	return e;
}
