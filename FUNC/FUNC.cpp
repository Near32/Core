#include "FUNC.h"

#include "../VAR/VAR.h"

//#define debuglvl1
//#define debuglvl2
//#define debuglvl3
//#define debuglvl4

FUNC::FUNC(const FUNCType& ftype_, const float param_) : EXP(), ftype(ftype_), param(param_)
{
	this->type = ETFUNC;
}

FUNC::FUNC(const FUNC* f)
{
	this->hasChanged = true;
	this->containsVAR = f->getContainsVAR();
	
	this->type = ETFUNC;
	this->ftype = f->getFType();
	
	if(this->ftype == FTzero || this->ftype == FTone || this->ftype == FTmone || this->ftype == FTcst )
	{
		this->containsVAR = false;
	}
	
	this->parent = f->getParent();
	this->param = f->getParam();
	
	//f has only one arg node :
	EXP* a = f->getArg(0);
	this->arg.clear();		
	
	if(a != NULL)
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
	}
	
}

FUNC::FUNC(const FUNC& f)
{
	this->hasChanged = true;
	this->containsVAR = f.getContainsVAR();
	
	this->type = ETFUNC;
	this->ftype = f.getFType();
	
	if(this->ftype == FTzero || this->ftype == FTone || this->ftype == FTmone || this->ftype == FTcst )
	{
		this->containsVAR = false;
	}
	
	this->parent = f.getParent();
	this->param = f.getParam();
	
	//f has only one arg node :
	EXP* a = f.getArg(0);
	this->arg.clear();		
	
	if(a != NULL)
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
	}
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
		
		return std::string( " cst0 ");
		
		break;
		
		case FTcst :
		{
		std::ostringstream ss;
		ss << this->param;
		return std::string( " cst=") + ss.str() + std::string( " " );
		}
		break;
		
		case FTone :
		{
		return std::string( " cst1 ");
		}
		break;
		
		case FTmone :
		{
		return std::string( " cst-1 " );
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
	this->hasChanged = true;
	this->containsVAR = f.getContainsVAR();
	
	//FUNCType :
	this->ftype = f.getFType();
	
	
	this->parent = f.getParent();
	this->arg.clear();
	
	EXP* a = f.getArg(0);
	//f has only one arg node :
	
	if(a != NULL)
	{
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
	
	this->hasChanged = true;
}

bool FUNC::doesContainVAR()
{
	if(this->hasChanged)
	{
		int nbrarg = this->arg.size();
		
		if( nbrarg >= 1)
		{
			this->containsVAR = this->arg[0]->doesContainVAR();
		}
		else
		{
			//We assume that there can be no mistake and it does only mean that this FUNC is one of FTzero etc...
			this->containsVAR = false;
		}
		
		this->hasChanged = true;
	}
	
	return this->containsVAR;
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


/*
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
*/

EXP& regularize(EXP* e, bool& goOn)
{
	int nbrargbegin = e->getNBRArg();
	
	if( nbrargbegin > 0 )
	{
		if( e->getParent() == NULL)
		{
			//we are at the root.
			if( e->getType() == ETEXP && e->getOType() == EOProduct )
			{
				raiseUpPRODUCT(e);
				raiseUpSUM(e);
			}
			
			replaceCST(e);
			deleteIdentity(e);
			//let us work on the arguments in the right order :
			int nbrarg = e->getNBRArg();
			
			for(int i=0;i < nbrarg;i++)
			{
				EXP* temp = e->getArg(i);
				
				bool regularizationNeed = false;
				//verify if it can be replaced by a constant:
				if( !(temp->doesContainVAR()) )
				{
					//let us replace it :
					FUNC fcst(FTcst);
					fcst.setParam( temp->evaluate() );
#ifdef debuglvl2
std::cout << "ROOT : FTCST replacement : arg = " << i+1 << "/" << nbrarg << " : " << e->toString() << std::endl;
#endif						
					e->replaceArg( temp, (EXP*)new FUNC(fcst) ); 
				}
				regularizationNeed = true;	//we want to delete the multiplicative constants...
				//end of verification and.
				
				if(regularizationNeed)
				{
					bool tGoOn = true;
					regularize( temp, tGoOn);
			
					if( !tGoOn )
					{
						//it means that a replacement/deletion have taken place.
						// we do not need to advance on the list of argument :
#ifdef debuglvl1
std::cout << "REENTERING." << std::endl;
#endif
						i--;
					}
				}
				else
				{
					//replacement -> nothing to do here.
				}
				
				nbrarg = e->getNBRArg();
			}
			
			//-------------------------------------
			//now, what can we assume on our arguments... ?
			//what to do on the root ?
			replaceCST(e);
			
			if(nbrarg == 0)
			{
				//assert the will of having EOId whenever we begin an expression...
				*e = EXP(EOId);
				FUNC fzero(FTzero);
				attach( *e, (EXP&)fzero);
			}
			
			//done nothing --> we can advance in the list, at parent level... :
			goOn = true;
			
			//let us return the now corrected EXP :
			return *e;
			//-------------------------------------
		}
		else
		{
			//we are not on the root and we have arguments :
			EXP* eParent = e->getParent();
			
			switch( e->getType() )
			{
				case ETEXP :
				{
					switch( e->getOType() )
					{
						case EOId :
						{
							//obviously, there is only one argument and we have to make it raise within the tree,
							// deleting this node and adding its only argument at the end of its parent list so that it will be treated after all...
							//attach( *eParent, (EXP&)( *(e->getArg(0)) ) );
							//eParent->deleteArg( (EXP*)e);
							EXP* targ = e->getArg(0);
							
							//verify if it can be replaced by a constant:
							if( !(targ->doesContainVAR()) )
							{
								//let us replace it :
								FUNC fcst(FTcst);
								fcst.setParam( targ->evaluate() );
#ifdef debuglvl4
std::cout << "EOID : FTCST replacement : " << " : " << e->toString() << std::endl;
#endif						
								e->replaceArg( targ, (EXP*)new FUNC(fcst) ); 
							}
							
							//it does contain a variable, we have to regularize it : let us go on :
				
#ifdef debuglvl1
std::cout << "EOID : " << e->toString() << std::endl;
std::cout << "PARENT :" << eParent->toString() << std::endl;
#endif
							
							switch(targ->getType())
							{
								
								case ETEXP :
								{
									eParent->replaceArg( e, (EXP*)new EXP(*targ) );
								}
								break;
								
								case ETFUNC :
								{
									eParent->replaceArg( e, (EXP*)new FUNC( (FUNC&)(*targ) ) );
								}
								break;
								
								case ETVAR :
								{
									eParent->replaceArg( e, (EXP*)new VAR( (VAR&)(*targ) ) );
								}
								break;
								
							}
#ifdef debuglvl1
std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
#endif
							//replacement --> do not advance in the list, at parent level... :
							goOn = false;
							
						}
						break;
						
						case EOSum :
						{
							//obviously, there is at least two arguments.
							//let us work on the arguments in the right order :
							int nbrarg = e->getNBRArg();
							
							if( e->getNBRArg() == 1)
							{
								//there can be only one argument, so we have to raise it up : 
								//cause we check it first it might be deleted due to its parents properties
								//whereas it should have been raised up.
								EXP* targ = e->getArg(0);
								
								switch(targ->getType())
								{
								
									case ETEXP :
									{
										eParent->replaceArg( e, (EXP*)new EXP(*targ) );
									}
									break;
								
									case ETFUNC :
									{
										eParent->replaceArg( e, (EXP*)new FUNC( (FUNC&)(*targ) ) );
									}
									break;
								
									case ETVAR :
									{
										eParent->replaceArg( e, (EXP*)new VAR( (VAR&)(*targ) ) );
									}
									break;
								
								}
								//std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
								//replacement --> do not advance in the list, at parent level... :
								goOn = false;
								
								//we have to return something...
								return *(e->getParent());
							}
							else
							{
								//if we have more than one argument, let us raise up all of those inner product before we go on:
								
#ifdef debuglvl3
std::cout << "SUM RAISE UP : before : " << e->toString() << std::endl;
#endif								
								raiseUpSUM( e );
#ifdef debuglvl3
std::cout << "SUM RAISE UP : after : " << e->toString() << std::endl;
#endif								
								replaceCST(e);
								deleteIdentity(e);
																
							}
							
							for(int i=0;i < nbrarg;i++)
							{
								EXP* temp = e->getArg(i);
								
								bool regularizationNeed = false;
								//verify if it can be replaced by a constant:
								if( !(temp->doesContainVAR()) )
								{
									//let us replace it :
									FUNC fcst(FTcst);
									fcst.setParam( temp->evaluate() );
#ifdef debuglvl3
std::cout << "SUM : FTCST replacement : arg = " << i+1 << "/" << nbrarg << " : " << e->toString() << std::endl;
#endif						
									e->replaceArg( temp, (EXP*)new FUNC(fcst) ); 
								}
								else
								{
									//it does contain a variable, we have to regularize it :
									regularizationNeed = true;
					
								}
								//end of verification and.
				
								if(regularizationNeed)
								{
									bool tGoOn = true;
									regularize( temp, tGoOn);
								
									if( !tGoOn )
									{
								
#ifdef debuglvl2
std::cout << "REENTERING SUM : arg = " << i+1 << "/" << nbrarg << " sum : " << e->toString() << std::endl;
#endif								
										//it means that a replacement/deletion have taken place.
										// we do not need to advance on the list of argument :
										i--;
									}
								
#ifdef debuglvl2
std::cout << "RESULT SUM : arg = " << i+1 << "/" << nbrarg << " sum : " << e->toString() << std::endl;
#endif							
								}
								else
								{
									//replacement -> nothing to do here.
								}
								
								
											
								
								nbrarg = e->getNBRArg();
							}
			
							//-------------------------------------
							//now, what can we assume on our arguments... ?
							//what to do on this summing node ?
							
							//there can be no argument left anymore :
							if( e->getNBRArg() == 0)
							{
								//then we have to delete this node and NULLed the pointer :
								eParent->deleteArg( (EXP*)e);
								//deletion --> do not advance in the list, at parent level... :
								goOn = false;
							}
							else if( e->getNBRArg() == 1)
							{
								//there can be only one argument, so we have to raise it up : 
								EXP* targ = e->getArg(0);
								
								switch(targ->getType())
								{
								
									case ETEXP :
									{
										eParent->replaceArg( e, (EXP*)new EXP(*targ) );
									}
									break;
								
									case ETFUNC :
									{
										eParent->replaceArg( e, (EXP*)new FUNC( (FUNC&)(*targ) ) );
									}
									break;
								
									case ETVAR :
									{
										eParent->replaceArg( e, (EXP*)new VAR( (VAR&)(*targ) ) );
									}
									break;
								
								}
								//std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
								//replacement --> do not advance in the list, at parent level... :
								goOn = false;
							}
							else
							{
								//if there are remaining arguments, those are obviously none zero arguments
								// for they have been handled in the right order.
								// therefore, this node needs to be reattached to its parent.
								// This reattaching has to take place here, and not at its parent level.
								// Otherwise, it would be treated again and again.
								
								//don't have to delete it then...
								//nothing done --> we can advance in the list, at parent level... :
								goOn = true;
							}
							
							//-------------------------------------
						}
						break;
						
						case EOProduct :
						{
							//obviously, there is at least two arguments.
							//let us work on the arguments in the right order :
							int nbrarg = e->getNBRArg();
							//std::cout << "nbr Arg : " << nbrarg << std::endl;
							
							if( nbrarg == 1)
							{
								//there can be only one argument, so we have to raise it up : 
								//cause we check it first it might be deleted due to its parents properties
								//whereas it should have been raised up.
								EXP* targ = e->getArg(0);
								
								switch(targ->getType())
								{
								
									case ETEXP :
									{
										eParent->replaceArg( e, (EXP*)new EXP(*targ) );
									}
									break;
								
									case ETFUNC :
									{
										eParent->replaceArg( e, (EXP*)new FUNC( (FUNC&)(*targ) ) );
									}
									break;
								
									case ETVAR :
									{
										eParent->replaceArg( e, (EXP*)new VAR( (VAR&)(*targ) ) );
									}
									break;
								
								}
								//std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
								//replacement --> do not advance in the list, at parent level... :
								goOn = false;
								
								//we have to return something...
								return *(e->getParent());
							}
							else
							{
								//if we have more than one argument, let us raise up all of those inner product before we go on:
								
#ifdef debuglvl3
std::cout << "PRODUCT RAISE UP : before : " << e->toString() << std::endl;
#endif								
								raiseUpPRODUCT( e );
#ifdef debuglvl3
std::cout << "PRODUCT RAISE UP : after : " << e->toString() << std::endl;
#endif										
								replaceCST(e);
								deleteIdentity(e);						
							}
							
							for(int i=0;i < nbrarg;i++)
							{
								EXP* temp = e->getArg(i);
								
								bool regularizationNeed = false;
								//verify if it can be replaced by a constant:
								if( !(temp->doesContainVAR()) )
								{
									//let us replace it :
									FUNC fcst(FTcst);
									fcst.setParam( temp->evaluate() );
#ifdef debuglvl3
std::cout << "PRODUCT : FTCST replacement : arg = " << i+1 << "/" << nbrarg << " : " << e->toString() << std::endl;
#endif						
									e->replaceArg( temp, (EXP*)new FUNC(fcst) ); 
								}
								else
								{
									//it does contain a variable, we have to regularize it :
									regularizationNeed = true;
					
								}
								//end of verification and.
				
								if(regularizationNeed)
								{
									bool tGoOn = true;
									regularize( temp, tGoOn);
								
									if( !tGoOn )
									{
								
#ifdef debuglvl2
std::cout << "REENTERING PRODUCT : arg = " << i+1 << "/" << nbrarg << " sum : " << e->toString() << std::endl;
#endif								
										//it means that a replacement/deletion have taken place.
										// we do not need to advance on the list of argument :
										i--;
									}
								
#ifdef debuglvl2
std::cout << "RESULT PRODUCT : arg = " << i+1 << "/" << nbrarg << " sum : " << e->toString() << std::endl;
#endif							
								}
								else
								{
									//replacement -> nothing to do here.
								}
								
								
											
								
								nbrarg = e->getNBRArg();
							}
			
							//-------------------------------------
							//now, what can we assume on our arguments... ?
							//what to do on this product node ?
							
							//there can be no argument left anymore : ??
							if( e->getNBRArg() == 0)
							{
								//the we have to delete this node and NULLed the pointer :
								//eParent->deleteArg( (EXP*)e);								
								e->getParent()->deleteArg( (EXP*)e);								
								//deletion --> do not advance in the list, at parent level... :
								goOn = false;
							}
							else if( e->getNBRArg() == 1)
							{
								//there can be only one argument, so we have to raise it up : 
								EXP* targ = e->getArg(0);
								
								switch(targ->getType())
								{
								
									case ETEXP :
									{
										eParent->replaceArg( e, (EXP*)new EXP(*targ) );
									}
									break;
								
									case ETFUNC :
									{
										eParent->replaceArg( e, (EXP*)new FUNC( (FUNC&)(*targ) ) );
									}
									break;
								
									case ETVAR :
									{
										eParent->replaceArg( e, (EXP*)new VAR( (VAR&)(*targ) ) );
									}
									break;
								
								}
								//std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
								//replacement --> do not advance in the list, at parent level... :
								goOn = false;
							}
							else
							{
								//if there are remaining arguments, those may obviously comprised zero arguments
								// therefore, this node needs to be reattached to its parent only if there are no zero argument nodes attached.
								
								bool zeros = seek4Zeros( e);
								
								if( !zeros)
								{
									//there is no zero argument :
									// This reattaching has to take place here, and not at its parent level.
									// Otherwise, it would be treated again and again.
									//nothing done --> we can advance in the list, at parent level... :
									goOn = true;
								}
								else
								{
									//there is at least one zero argument :
									// Therefore this node can be replaced by a zero.
									//std::cout << "PARENT before replace :" << eParent->toString() << std::endl;
									eParent->replaceArg( e, (EXP*)new FUNC(FTzero) );
									//std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
									//replacement --> we can advance in the list, at parent level... :
									goOn = true;
								}
							}
							
							//-------------------------------------
						}
						break;
					}
				}
				break;
				
				case ETFUNC :
				{
					//if we are here, it means that this FUNC has at least one argument.
					// let us deal with it/those :
					unsigned int nbrarg = e->getNBRArg();
					
					for(int i=0;i<nbrarg;i++)
					{
						EXP* temp = e->getArg(i);
						bool tGoOn = true;
						regularize( temp, tGoOn);
						
						if( !tGoOn )
						{
							//it means that a replacement/deletion have taken place.
							// we do not need to advance on the list of argument :
							i--;
						}
						
						nbrarg = e->getNBRArg();
					}
	
					//-------------------------------------
					//now, what can we assume on our arguments... ?
					//what to do on this FUNC node ?
					//there is at least one argument that remained.
					
					//no modification --> we can advance in the list, at parent level... :
					goOn = true;
					//-------------------------------------
				}
				break;
				
				case ETVAR :
				{
					//------------------------------------------------------------------------------------
					//Since it is a VAR node, there are no arguments so, this case will never appears....
					//------------------------------------------------------------------------------------
					
					
					//Nothing to do here.
					
					//But here's what will be done on the rigt case :
					//-------------------------------------
					//no modification --> we can advance in the list, at parent level... :
					goOn = true;
					//-------------------------------------
				}
				break;
			}

		}		
	}
	else
	{
		//we have no arguments... :
		// what to do ?
		//the type can only be VAR-->nothing to do,  or FUNC-->case of 1 or 0 to handle in parent = sum or product...
		EXP* eParent = e->getParent();
//		std::cout << " NOARG : type of e = " << e->getType() << " /ETFUNC = " << ETFUNC << std::endl;
		
		switch(e->getType())
		{
			case ETFUNC :
			{
				switch( ((FUNC*)e)->getFType())
				{
					case FTzero :
					{
						if( eParent->getOType() == EOSum )
						{
							//then we can delete this argument :
#ifdef debuglvl1
std::cout << "BEFORE DELETION :" << eParent->toString() << std::endl;
#endif
							//eParent->deleteArg(e);
							e->getParent()->deleteArg(e);
#ifdef debuglvl1
std::cout << "PARENT after deletion :" << eParent->toString() << std::endl;
#endif							
							
							//deletion->do not advance in the parent node list :
							goOn = false;
						}
					}
					break;
					
					case FTone :
					{
						if( eParent->getOType() == EOProduct && eParent->getNBRArg() != 1 )
						{
						
#ifdef debuglvl4
std::cout << "CAS FT 1 : with a parent product... deletion." << std::endl;
#endif						
							//then we can delete this argument :
							//std::cout << "BEFORE DELETION :" << eParent->toString() << std::endl;
							//eParent->deleteArg(e);
							e->getParent()->deleteArg(e);
							//std::cout << "PARENT after deletion :" << eParent->toString() << std::endl;
							
							
							//deletion->do not advance in the parent node list :
							goOn = false;
						}
						else
						{
						
#ifdef debuglvl4
std::cout << "CAS FT 1 : DID NOT ENTER." << std::endl;
#endif											
						}
					}
					break;
					
					case FTmone :
					{
#ifdef debuglvl1
std::cout << "CAS FT -1 : nothing to do : parent :" << eParent->toString() << std::endl;
#endif					
						/* WATCH OUT : there is nothing to do here !!!!! */
						
						
						/*if( eParent->getOType() == EOProduct && eParent->getNBRArg() != 1 )
						{
						
#ifdef debuglvl1
std::cout << "CAS FT -1 : entering for DELETION." << std::endl;
#endif											
							//then we can delete this argument :
							//std::cout << "BEFORE DELETION :" << eParent->toString() << std::endl;
							eParent->deleteArg(e);
							//std::cout << "PARENT after deletion :" << eParent->toString() << std::endl;
#ifdef debuglvl1								
std::cout << "CAS FT -1 : PARENT after deletion :" << eParent->toString() << std::endl;
#endif							
							
							//deletion->do not advance in the parent node list :
							goOn = false;
						}*/
					}
					break;
					
					case FTcst :
					{
#ifdef debuglvl4
std::cout << "CAS FTCST !!!!!! " << std::endl;
#endif

						//almost zero :
						float prec = 1e-7f;
						if( ((FUNC*)e)->getParam() <= 0.0f+prec && ((FUNC*)e)->getParam() >= 0.0f-prec)
						{
#ifdef debuglvl1
std::cout << "CAS FTCST 0 : BEFORE DELETION :" << eParent->toString() << std::endl;
#endif
							//then it is like a FTzero funciton... :
							
							//if( eParent->getType() == ETEXP && eParent->getOType() == EOSum )
							if(eParent->getOType() == EOSum )
							{
								//then we can delete this argument :
								//eParent->deleteArg(e);
								e->getParent()->deleteArg(e);
#ifdef debuglvl1								
std::cout << "CAS FTCST 0: PARENT after deletion :" << eParent->toString() << std::endl;
#endif
								//deletion->do not advance in the parent node list :
								goOn = false;
							}
						}
						
						if( ((FUNC*)e)->getParam() == 1.0f)
						{
#ifdef debuglvl4
std::cout << "CAS FTCST 1 : BEFORE DELETION :" << eParent->toString() << std::endl;
#endif
							//then it is like a FTone funcion... :
							
							if( eParent->getOType() == EOProduct && eParent->getNBRArg() != 1 )
							{
								//then we can delete this argument :
								//eParent->deleteArg(e);
								e->getParent()->deleteArg(e);
#ifdef debuglvl4								
std::cout << "CAS FTCST 1 : PARENT after deletion :" << eParent->toString() << std::endl;
#endif
								//deletion->do not advance in the parent node list :
								goOn = false;
							}
						}
						else
						{
						
#ifdef debuglvl4
std::cout << "CAS FTCST 1 : DID NOT ENTER." << std::endl;
#endif											
						}
						
					}
					break;
				}
			}
			break;
			
			default :
			//do nothing...
			break;
		}
		
		//no modification --> we can advance in the list, at parent level... :
		goOn = true;
		//-------------------------------------
	
	}
	
	//we have to return something...
	return *(e->getParent());
}

EXP regw( EXP& exp)
{
	bool dummy = false;
	return regularize( &exp, dummy);
}

bool seek4Zeros(EXP* e)
{
	bool r = false;
	float prec = 1e-10f;
	
	//we assume that the zeros have been raised within the tree and so on,
	// so than we only have to check for the given arguments :
	
	/*
	for(int i=e->getNBRArg();i--;)
	{
		EXP* targ = e->getArg(i);
		if( targ->getType() == ETFUNC)
		{
			if( ((FUNC*)targ)->getFType() == FTzero || ( ((FUNC*)targ)->getFType() == FTcst && ((FUNC*)targ)->getParam() <= 0.0f+prec && ((FUNC*)targ)->getParam() >= 0.0f-prec ) )
			{
				r = true;
				break;
			}
		}
	}
	*/
	
	
	for(int i=e->getNBRArg();i--;)
	{
		EXP* targ = e->getArg(i);
		float value = targ->evaluate();
		if( !(targ->doesContainVAR()) && value <= 0.0f+prec && value >= 0.0f-prec )
		{
			r = true;
			break;
		}
		else if( targ->getType() == ETFUNC)
		{
			if( ((FUNC*)targ)->getFType() == FTzero || ( ((FUNC*)targ)->getFType() == FTcst && ((FUNC*)targ)->getParam() <= 0.0f+prec && ((FUNC*)targ)->getParam() >= 0.0f-prec ) )
			{
				r = true;
				break;
			}
		}
	}
	
	//std::cout << "SEEK AND DESTROY ZEROS = " << r << std::endl;
	
	return r;
}

//delete the association of product arguments... by raising all up to one product node
void raiseUpPRODUCT( EXP* prod)
{	

#define working
#ifdef working
	//we know that prod is an EXP class with otype=EOProduct, and more than 1 argument :
	EXP* targ = NULL;
	int nbrarg = prod->getNBRArg();
	
	for(int i=0;i<nbrarg;i++)
	{
		targ = prod->getArg(i);
		
		if(targ != NULL)
		{
			//theoritically always the case :
			if( targ->getOType() == EOProduct)
			{
				raiseUpPRODUCT(targ);
				
				int nbrargtarg = targ->getNBRArg();
				
				for(int j=nbrargtarg;j--;)
				{
					attach( *prod, *(targ->getArg(j)) );
					//std::cout << "Current product reattaching j = " << j << " : " << 	prod->toString() << std::endl;
				}
				
				//std::cout << "Current product final : i = " << i << " : " << prod->toString() << std::endl;
				prod->deleteArg(i);
				//std::cout << "Current product final after deletion :  i = " << i << "/" << nbrarg-1 << " : " << prod->toString() << std::endl;
				
				i--;
			}
		}
		
		nbrarg = prod->getNBRArg();
		
		//std::cout << "Current product final:  i = " << i << "/" << nbrarg-1 << " : " << prod->toString() << std::endl;
	}
	
	
	
	
#else	
	//parcour en largeur d'abord :
	std::vector<EXP*> list;
	list.insert( list.end(), prod);
	
	while( list.size() != 0)
	{
		EXP* p = list[0];
		EXP* pParent = p->getParent();
		
		//---------------------------
		if(pParent != NULL)
		{
			//otherwise we would be at the root and cannot afford to change it...
			
			if( p->getType() == ETEXP && p->getOType() == EOProduct && pParent->getType() == ETEXP && pParent->getOType() == EOProduct )
			{
				//there are many arguments :
				int nbrarg = p->getNBRArg();
				EXP* targ = NULL;
				list.erase(list.begin()+0);
				
				for(int i=nbrarg;i--;)
				{
					targ = p->getArg(i);
					
					if( targ != NULL)
					{
						//theoritycally always the case :
						attach( *pParent, *targ);
						list.insert( list.begin(), pParent->getArg( pParent->getNBRArg()-1 ) ); 
					}
					else
					{
						throw;
					}
				}
				
				pParent->deleteArg( p);
				continue;
				
			}
		}
		
		//------------------------
		
		int nbrarg = p->getNBRArg();
		while(nbrarg--)
		{
			list.insert( list.end(), p->getArg(nbrarg) ); 
		}
		
		
		//------------------------
		list.erase(list.begin()+0);
		
	}
#endif	
	
	
	
}


//delete the association of sum arguments... by raising all up to one sum node
void raiseUpSUM( EXP* sum)
{
	//we know that sum is an EXP class with otype=EOSum, and more than 1 argument :
	EXP* targ = NULL;
	int nbrarg = sum->getNBRArg();
	
	for(int i=0;i<nbrarg;i++)
	{
		targ = sum->getArg(i);
		
		if(targ != NULL)
		{
			//theoritically always the case :
			if( targ->getOType() == EOSum)
			{
				raiseUpSUM(targ);
				
				int nbrargtarg = targ->getNBRArg();
				
				for(int j=nbrargtarg;j--;)
				{
					attach( *sum, *(targ->getArg(j)) );
				}
				
				//std::cout << "Current sum final : i = " << i << " : " << sum->toString() << std::endl;
				sum->deleteArg(i);
				//std::cout << "Current sum final after deletion :  i = " << i << "/" << nbrarg-1 << " : " << sum->toString() << std::endl;
				
				i--;
			}
		}
		
		nbrarg = sum->getNBRArg();
		
		//std::cout << "Current sum final:  i = " << i << "/" << nbrarg-1 << " : " << sum->toString() << std::endl;
	}
}


void deleteIdentity( EXP* exp)
{
	//parcour en largeur d'abord :
	std::vector<EXP*> list;
	list.insert( list.end(), exp);
	
	while( list.size() != 0)
	{
		EXP* p = list[0];
		EXP* pParent = p->getParent();
		
		//---------------------------
		if(pParent != NULL)
		{
			//otherwise we would be at the root and cannot afford to change it...
			
			if( p->getType() == ETEXP && p->getOType() == EOId)
			{
				//there is only one argument :
				EXP* targ = p->getArg(0);
				EXP* nextp = NULL;
				
				switch(targ->getType())
				{
		
					case ETEXP :
					{
						nextp = (EXP*)new EXP(*targ);
						pParent->replaceArg( p, nextp );
					}
					break;
		
					case ETFUNC :
					{
						nextp = (EXP*)new FUNC( (FUNC&)(*targ) );
						pParent->replaceArg( p, nextp );
					}
					break;
		
					case ETVAR :
					{
						nextp = (EXP*)new VAR( (VAR&)(*targ) );
						pParent->replaceArg( p, nextp );
					}
					break;
		
				}
				
				p = nextp;
			}
		}
		
		//------------------------
		
		int nbrarg = p->getNBRArg();
		
		while(nbrarg--)
		{
			list.insert( list.end(), p->getArg(nbrarg) ); 
		}
		
		
		//------------------------
		list.erase(list.begin()+0);
		
	}
}

void replaceCST( EXP* exp)
{
	//parcour en largeur d'abord :
	std::vector<EXP*> list;
	list.insert( list.end(), exp);
	
	while( list.size() != 0)
	{
		EXP* p = list[0];
		EXP* pParent = p->getParent();
		
		//---------------------------
		if(pParent != NULL)
		{
			//otherwise we would be at the root and cannot afford to change it...
			
			if( !( p->doesContainVAR() ) )
			{
				//let us replace that node by a FTcst :
				FUNC fcst(FTcst);
				fcst.setParam( p->evaluate() );
				
				pParent->replaceArg( p, (EXP*)new FUNC(fcst) );				
				
				//we do not need to continue and add the arguments since there are none :
				list.erase(list.begin()+0);
				continue;
			}
		}
		
		//------------------------
		
		int nbrarg = p->getNBRArg();
		
		while(nbrarg--)
		{
			list.insert( list.end(), p->getArg(nbrarg) ); 
		}
		
		
		//------------------------
		list.erase(list.begin()+0);
		
	}
}



