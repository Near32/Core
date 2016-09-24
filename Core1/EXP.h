#ifndef EXP_H
#define EXP_H

#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <math.h>
#include <string>
#include <sstream>


//#define debug_lvl1		

template<typename T>
class EXP;

template<typename T>
EXP<T> regularize(EXP<T>& e);

enum EXPType {
	ETEXP,
	ETFUNC,
	ETVAR
};

enum EXPOp{
	EONone,
	EOId,
	EOSum,
	EOProduct
};


enum FUNCType{
	FTNone,
	
	FTcst,	
	FTzero,
	FTone,
	FTmone,
	
	FTid,
	FTminus,
	FTsquare,
	FTinverse,
	
	FTsin,
	FTcos,
	FTasin,
	FTacos
};



extern int VARID_COUNT;


extern std::map<int,float> ID2REF;


template<typename T>
class EXP
{
	private :
	EXPOp otype;
	EXPType type;
	FUNCType ftype;
	T value;
	
	EXP* parent;
	std::vector<EXP*> arg;
	
	
	
	public :
	
	//VAR ?
	T& valueREF;
	int VARID;
	
	//FT cst ?
	T param;
	
	
	//----------------------------
	//----------------------------
	//----------------------------
	
	//CREATE A VAR AND INITIALIZE IT. DEFAULT INITIALIZER :
	EXP(const T& value_=0.0f) : otype(EONone), type(ETVAR), ftype(FTNone), parent(NULL), value((T)value_), valueREF(value), VARID(VARID_COUNT++), param((T)0)
	{
		this->arg.clear();
		ID2REF[VARID] = value;
	}
	
	void create(const T& value_=0.0f)
	{
		otype = EONone;
		type = ETVAR;
		ftype = FTNone;
		parent = NULL;
		value = (T)value_;
		valueREF = value;
		
		VARID = VARID_COUNT++;
		ID2REF[VARID] = value;
		param = (T)0;
		 
		this->arg.clear();
	}
	
	//CREATE AN OPERATION :
	EXP(const EXPOp& otype_) : otype(otype_), type(ETEXP), ftype(FTNone), parent(NULL), value((T)0), valueREF(value), VARID(-1), param((T)0)
	{
		if(otype_ == EONone)
		{
			throw;
		}
		
		this->arg.clear();
	}
	
	void create(const EXPOp& otype_)
	{
		if(otype_ == EONone)
		{
			throw;
		}
		
		otype = otype_;
		type = ETEXP;
		ftype = FTNone;
		parent = NULL;
		value = (T)0;
		valueREF = value;
		
		VARID = -1;
		param = (T)0;
		
		this->arg.clear();
	}
	
	//CREATE A FUNCTION :
	EXP(const FUNCType& ftype_, const T& paramval = (T)0) : otype(EONone), type(ETFUNC), ftype(ftype_), parent(NULL), value((T)0), valueREF(value), VARID(-1), param((T)paramval)
	{
		this->arg.clear();
	}
	
	void create(const FUNCType& ftype_, const T& paramval = (T)0)
	{
		if(ftype_ == FTNone)
		{
			throw;
		}
		
		otype = EONone;
		type = ETFUNC;
		ftype = ftype_;
		parent = NULL;
		value = (T)0;
		valueREF = value;
		
		VARID = -1;
		param = paramval;
		
		this->arg.clear();
	}
	
	EXP(const EXP& e) : value( e.getValue() ), valueREF( e.valueREF )
	{
		
		this->arg.clear();
		this->type = e.getType();
		this->otype = e.getOType();
		this->ftype = e.getFType();
		
		this->parent = e.getParent();

		
		this->VARID = e.VARID;
	
		int nbridx = e.getNbrArg();
		for(int k=0;k<nbridx;k++)
		{
			this->addArg( e.copyArg(k)); 
		}
		
		//regularize(*this);
	}
	
	~EXP()
	{
		
		for(int i=0;i<arg.size();i++)
		{
			delete this->arg[i];	
		}
	
	}
	
	T evaluate()
	{
		
		if(arg.size() >= 1)	//IT IS NOT A VAR :
		//if( this->type != ETVAR)	//IT IS NOT A VAR :
		{
			T r=(T)0;
			if(this->otype==EOProduct)	r = (T)1;
		
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
						
						case EONone:	//IT IS NOT A VAR, NOT AN OPERATION... SO IT IS A FUNC :
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
								{
									if(this->arg.size() == 1)
									{
										return sin(this->arg[0]->evaluate());
									}
									else
									{
										throw;
									}
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
						break;
					}
				
				}
				catch(std::exception& e)
				{
					std::cout << "EVALUATION :: " << e.what() << std::endl;
				}
			}
		
			return r;
		}
		else	//IT IS A VAR or a constant function :
		{
			if( this->type == ETVAR)
			{
				//this->value = this->valueREF;
				//std::cout << " VARID = " << this->VARID << " :: value/valueREF = " << value << " / " << valueREF << std::endl;
				this->value = ID2REF[this->VARID];
				return this->value;
			}
			else
			{
				if(this->type == ETFUNC )
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
						
						default :
						throw;
						break;
					}
				}
				else
				{
					throw;
				}
			}
		}
	}
	
	
	std::string toString()
	{
		if(arg.size() >= 1)
		//if( this->type != ETVAR)
		{
			std::string r;
			bool parenthesis = true;
		
			for(int i=0;i<this->arg.size();i++)
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
						//parenthesis = false;
						r += arg[i]->toString();
						if(i<arg.size()-1)
						{
							r+=std::string( " + ");
						}
						break;
					
						case EOProduct :
						//parenthesis = false;
						r += arg[i]->toString();
						if(i<arg.size()-1)
						{
							r+=std::string( " * ");
						}
						break;
						
						case EONone :
						{	
							switch(this->ftype)
							{
								case FTzero :
								{
								return std::string( " 0 ");
								}
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
								
								default :
								{
								throw;
								}
								break;
							}
						}
						break;
					}
				
				}
				catch(std::exception& e)
				{
					std::cerr << "TOSTRING :: " << e.what() << std::endl;
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
			if( this->type == ETVAR)
			{
				//this->value = this->valueREF;
				this->value = ID2REF[this->VARID];
				std::ostringstream ssi;
				ssi << "X" << this->VARID << "=" << this->value;
				return ssi.str();
			}
			else
			{
				//it may be a constant function :
				if( this->type == ETFUNC)
				{
					switch(this->ftype)
					{
						case FTzero :
						{
						return std::string( " 0 ");
						}
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
						
						default :
						throw;
						break;
					}
						
				}
				else
				{
					throw;
				}
			}
		}
	}
	
	void addArg(const EXP& arg_)
	{
		//this->arg.push_back( new EXP(arg_) );
		//this->arg[this->arg.size()-1]->setParent(this);
		EXP* ptrEXP = new EXP(arg_);
		ptrEXP->setParent(this);
		this->arg.push_back( ptrEXP );
	}
	
	void eraseArg( EXP* arg_)
	{
		bool erased = false;
		
		for(int k=0;k<this->arg.size();k++)
		{
			if(arg_ == (this->arg[k]) )
			{
				delete this->arg[k];
				this->arg.erase(this->arg.begin()+k);
				erased = true;
			}
			
			if(erased)
			{
				break;
			}
		}
		
		if(erased)
		{
#ifdef debug_lvl1		
			std::cout << "ERASE ARG : DONE." << std::endl;
#endif
		}
		else
		{
			std::cout << "ERASE ARG : PROBLEM." << std::endl;
			throw;
		}
	}
	
	void setParent( EXP* p)
	{
		this->parent = p;
	}
	
	void setParam( const T& p)
	{
		this->param = p;
	}
	
	
	EXP& operator=(const EXP& exp)
	{
		//this->~EXP();
		this->arg.clear();
		
		this->type = exp.getType();
		this->otype = exp.getOType();
		this->ftype = exp.getFType();
		this->parent = exp.getParent();
		this->value = exp.getValue();
		this->valueREF = exp.valueREF;
		this->VARID = exp.VARID;
	
		int nbridx = exp.getNbrArg();
		for(int k=0;k<nbridx;k++)
		{
			this->addArg( exp.copyArg(k) ); 
		}
		
		//regularize(*this);
		
		return *this;
	}
	
	
	EXP operator*( const EXP& exp)
	{
		EXP ret(EOProduct);
		
		ret.addArg(*this);
		ret.addArg(exp);
		
		regularize(ret);
		
		return ret;
	}
	
	EXP operator*=(const EXP& exp)
	{
		EXP ret(*this);
		
		//this->~EXP();
		this->create(EOProduct);
		
		this->addArg(ret);
		this->addArg( exp);
		
		return *this;
	}
	
	EXP operator+( const EXP& exp)
	{
		EXP ret(EOSum);
		
		ret.addArg(*this);
		ret.addArg( exp);
		
		regularize(ret);
		
		return ret;
	}
	
	EXP operator+=(const EXP& exp)
	{
		EXP ret(*this);
		
		//this->~EXP();
		this->create(EOSum);
		
		this->addArg(ret);
		this->addArg( exp);
		
		return *this;
	}
	
	EXP operator-( const EXP& exp)
	{
		EXP ret(EOSum);
		
		ret.addArg(*this);
		ret.addArg( EXP(FTmone) * exp );
		
		regularize(ret);
		
		return ret;
	}
	
	
	EXP operator-=(const EXP& exp)
	{
		EXP ret(*this);
		
		//this->~EXP();
		this->EXP(EOSum);
		
		this->addArg(ret);
		this->addArg( EXP(FTmone) * exp);
		
		return *this;
	}
	
	EXP operator/( const EXP& exp)
	{
		EXP ret(EOProduct);
		
		ret.addArg(*this);
		
		EXP temp(FTinverse);
		temp.addArg(exp);
		
		ret.addArg( temp);
		
		regularize(ret);
		
		return ret;
	}
	
	EXP operator/=( const EXP& exp)
	{
		EXP ret(*this);
		
		//this->~EXP();
		this->EXP(EOProduct);
		
		this->addArg(ret);
		
		EXP temp(FTinverse);
		temp.addArg(exp);
		this->addArg( temp);
		
		return *this;
	}
	
	
	
	
	
	//--------------------------

	EXP* getParent()	const	{	return this->parent;	}
	
	EXP* getArg(const size_t& idx)	
	{
		if(idx<arg.size())
		{
			return arg[idx];
		}
		else
		{
			throw;
		}
	}
	
	EXP copyArg(const size_t& idx)	const	
	{
		if(idx<arg.size())
		{
			return *arg[idx];
		}
		else
		{
			throw;
		}
	}
	
		
	EXPType getType()	const	{	return type;	}
	EXPOp getOType()	const	{	return otype;	}
	FUNCType getFType()	const	{	return ftype;	}
	
	T getValue()	const
	{	
		if(this->type == ETVAR)
		{
			return ID2REF[this->VARID];
		}
		
		return this->value;	
	}
	
	void setValue(const T& val)	
	{
		if(this->type == ETVAR)
		{
			this->value = val;
			ID2REF[this->VARID] = val;
		}
	}
	
	size_t getNbrArg()	const	
	{	
		return this->arg.size();
	}
	
};

/*
template<typename T>
std::string operator<<(const EXP<T>& e)
{
	return e.toString();
}
*/


template<typename T>
inline EXP<T> sin(const EXP<T>& e)
{
	EXP<T> r(FTsin);
	r.addArg( e);
	return r;
}

template<typename T>
inline EXP<T> asin(const EXP<T>& e)
{
	EXP<T> r(FTasin);
	r.addArg( e);
	return r;
}

template<typename T>
inline EXP<T> cos(const EXP<T>& e)
{
	EXP<T> r(FTcos);
	r.addArg( e);
	return r;
}

template<typename T>
inline EXP<T> acos(const EXP<T>& e)
{
	EXP<T> r(FTacos);
	r.addArg( e);
	return r;
}





#define TEST_REG
#ifdef TEST_REG
template<typename T>
EXP<T> regularize(EXP<T>& e)
{
	std::vector<EXP<T>*> pile;
	
	//let us assert that e is the root of an expression :
	EXP<T>* root= &e;
	while(root->getParent() != NULL)
	{
		root = root->getParent();
		std::cout << "REGULARIZATION :: NOT ROOT..." << std::endl;
		//it shouldn't appear...!
		throw;
	}
	
	pile.push_back(root);
	
	while(pile.size())
	{
		EXP<T>* p = pile[0];
		if(p == NULL)
		{
			pile.erase(pile.begin()+0);
			continue;
		}
		EXP<T>* pparent = p->getParent();
		bool hasBeenErased = false;
		//-------------------------------------
		//deal with that current node :
		EXPType ptype = p->getType();
		switch(ptype)
		{
			case ETVAR :
			{
				//nothing to do ...
			}
			break;
			
			case ETFUNC :
			{
				//nothing to do ...
				//TODO : see if the argument require regularization or not...
			}
			break;
			
			case ETEXP :
			{
				//depends on the kind of operation :
				EXPOp potype = p->getOType(); 
				switch(potype)
				{
					case EOId :
					{
						//let us raise its only child :
						EXP<T> tempChild( p->copyArg(0) );
						
						if(pparent != NULL)
						{
							pparent->eraseArg(p);
							pparent->addArg(tempChild);
						}
						else
						{
							
						}
						
						//this case should not appeared :
						throw;
						
					}
					break;
					
					case EOSum :
					{
						
						if(pparent != NULL)
						{
							//is it a sum too ?
							if(pparent->getOType() == EOSum)
							{
								//then let us fuse the two nodes at the parent :
								//int nbrarg = p->getNbrArg();
								//for(int k=0;k<nbrarg;k++)
								for(size_t k=p->getNbrArg();k--;)
								{
									//EXP<T> tempChild(p->copyArg(k) );
									pparent->addArg( p->copyArg(k) );
								}
								
								//we can erase the current node :
								pparent->eraseArg(p);
								hasBeenErased = true;
							}
						}
						else
						{
							//then it means that we are at the root of the computationnal graph
							// there is nothing to do.
						}
					}
					break;
					
					
					case EOProduct :
					{
						
						if(pparent != NULL)
						{
							//is it a product too ?
							if(pparent->getOType() == EOProduct)
							{
								//then let us fuse the two nodes at the parent :
								//int nbrarg = p->getNbrArg();
								//for(int k=0;k<nbrarg;k++)
								for(size_t k=p->getNbrArg();k--;)
								{
									//EXP<T> tempChild(p->copyArg(k));
									pparent->addArg( p->copyArg(k) );
								}
								
								//we can erase the current node :
								pparent->eraseArg(p);
								hasBeenErased = true;
							}
						}
						else
						{
							//then it means that we are at the root of the computationnal graph
							// there is nothing to do.
						}
					}
					break;
					
					default :
					{
						//the type is uncommon... this thing is bullshit :
						//TODO : what to try ?
						//delete p;
						pile.erase(pile.begin()+0);
						continue;
						
					}
					break;
				}
			}
			break;
		}
		
		
		//-------------------------------------
		
					
		//------------------------
		//store the child nodes that remain :
		if( !hasBeenErased && ptype != ETVAR)
		{
			//unsigned int nbrarg = p->getNbrArg();
			
			//for(unsigned int k=0;k<nbrarg;k++)
			for(size_t k=p->getNbrArg();k--;)
			{
				pile.push_back( p->getArg(k) );
			}
		}
		else if( hasBeenErased)
		{
			//we have to regularize the parent again :
			if(pparent != NULL)
			{
				pile.push_back(pparent);
			}
		}
		//------------------------
		
		
		//--------------------------------
		//let us delete the current element in the pile :
		pile.erase(pile.begin()+0);
	}
	
	return *root;
}
#else
template<typename T>
inline EXP<T> regularize(EXP<T>& e)
{
	return e;
}
#endif


#endif


