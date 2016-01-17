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
	this->type = ETFUNC;
	this->ftype = f.getFType();
	
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

EXP regularize(EXP* e, bool& goOn)
{
	if( e->getNBRArg() )
	{
		if( e->getParent() == NULL)
		{
			//we are at the root.
			//let us work on the arguments in the right order :
			unsigned int nbrarg = e->getNBRArg();
			
			for(int i=0;i < nbrarg;i++)
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
			}
			
			//-------------------------------------
			//now, what can we assume on our arguments... ?
			//what to do on the root ?
			
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
							std::cout << "EOID : " << e->toString() << std::endl;
							std::cout << "PARENT :" << eParent->toString() << std::endl;
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
							std::cout << "PARENT after replace :" << eParent->toString() << std::endl;
							//replacement --> do not advance in the list, at parent level... :
							goOn = false;
							
						}
						break;
						
						case EOSum :
						{
							//obviously, there is at least two arguments.
							//let us work on the arguments in the right order :
							unsigned int nbrarg = e->getNBRArg();
							
							for(int i=0;i < nbrarg;i++)
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
							unsigned int nbrarg = e->getNBRArg();
							//std::cout << "nbr Arg : " << nbrarg << std::endl;
							
							for(int i=0;i < nbrarg;i++)
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
							}
			
							//-------------------------------------
							//now, what can we assume on our arguments... ?
							//what to do on this product node ?
							
							//there can be no argument left anymore : ??
							if( e->getNBRArg() == 0)
							{
								//the we have to delete this node and NULLed the pointer :
								eParent->deleteArg( (EXP*)e);								
								//deletion --> do not advance in the list, at parent level... :
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
									eParent->replaceArg( e, (EXP*)new FUNC(FTzero) );
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
		switch(e->getType())
		{
			case ETFUNC :
			{
				switch( ((FUNC*)e)->getFType())
				{
					case FTzero :
					{
						if( eParent->getType() == ETEXP && eParent->getOType() == EOSum )
						{
							//then we can delete this argument :
							eParent->deleteArg(e);
							//deletion->do not advance in the parent node list :
							goOn = false;
						}
					}
					break;
					
					case FTone :
					{
					
					}
					break;
					
					case FTmone :
					{
					
					}
					break;
					
					case FTcst :
					{
						if( ((FUNC*)e)->getParam() == 0.0f)
						{
							//then it is like a FTzero funciton... :
							
							if( eParent->getType() == ETEXP && eParent->getOType() == EOSum )
							{
								//then we can delete this argument :
								eParent->deleteArg(e);
								//deletion->do not advance in the parent node list :
								goOn = false;
							}
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
	return EXP();
}


bool seek4Zeros(EXP* e)
{
	bool r = false;
	
	//we assume that the zeros have been raised within the tree and so on,
	// so than we only have to check for the given arguments :
	for(int i=e->getNBRArg();i--;)
	{
		EXP* targ = e->getArg(i);
		if( targ->getType() == ETFUNC)
		{
			if( ((FUNC*)targ)->getFType() == FTzero || ( ((FUNC*)targ)->getFType() == FTcst && ((FUNC*)targ)->getParam() == 0.0f ) )
			{
				r = true;
				break;
			}
		}
	}
	
	std::cout << "SEEK AND DESTROY ZEROS = " << r << std::endl;
	
	return r;
}



