#include "Operations.h"

/*
Mat<EXP> product(const Mat<EXP>& a, const Mat<EXP>& b)
{
	if(a.getColumn() == b.getLine() )
	{
		Mat<EXP> r(a.getLine(), b.getColumn());
		
		for(int i=1;i<=a.getLine();i++)
		{
			for(int j=1;j<=b.getColumn();j++)
			{
				
			}
		}
	}
	else
	{
		std::cerr "MATRICES SIZE DO NOT MATCHES... ABORDING." << std::endl;
		throw;
	}
};
*/

void toString(const Mat<EXP>& m)
{
	std::cout << "Matrice : " << std::endl;
	
	for(int i=1;i<=m.getLine();i++)
	{
		std::cout << "\t";
		
		for(int j=1;j<=m.getColumn();j++)
		{
			std::cout << m.get(i,j).toString() << "\t"; 
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	
}

void evaluate(const Mat<EXP>& m)
{
	std::cout << "Matrice : " << std::endl;
	
	for(int i=1;i<=m.getLine();i++)
	{
		std::cout << "\t";
		
		for(int j=1;j<=m.getColumn();j++)
		{
			std::cout << m.get(i,j).evaluate() << "\t"; 
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	
}

Mat<EXP> rotation(const EXP& expvar, int axis)
{
	EXP zero1(EOId);
	FUNC fzero(FTzero);
	attach( zero1, (EXP&)(fzero) );
	Mat<EXP> r( (EXP&)zero1, 3,3);
	
	EXP one1(EOId);
	FUNC fone(FTone);
	attach( one1, (EXP&)(fone) );
	
	FUNC fmone(FTmone);
	
	switch(axis)
	{
		case 1 :	//X axis		
		r.set(cos(expvar), 2,2);
		r.set(cos(expvar), 3,3);
		r.set(sin(expvar), 2,3);
		r.set( fmone*sin(expvar), 3,2);
		r.set( one1, 1,1);
		return r;
		break;
		
		case 2 : 	//Y axis
		r.set(cos(expvar), 1,1);
		r.set(cos(expvar), 3,3);
		r.set(sin(expvar), 1,3);
		r.set( fmone*sin(expvar), 3,1);
		r.set( one1, 2,2);
		return r;
		break;
		
		case 3 :	//Z axis
		r.set(cos(expvar), 1,1);
		r.set(cos(expvar), 2,2);
		r.set( fmone*sin(expvar), 1,2);
		r.set(sin(expvar), 2,1);
		r.set( one1, 3,3);
		return r;
		break;
	}
}


EXP* F2FDerivate(const EXP* f)
{
	switch( ((FUNC*)f)->getFType())
	{
		case FTzero :
		{
			*((FUNC*)f) = FUNC(FTzero);
			return (EXP*)f;
		}
		break;
		
		case FTcst :
		{
			*((FUNC*)f) = FUNC(FTzero);
			return (EXP*)f;
		}
		break;
		
		case FTone :
		{
			*((FUNC*)f) = FUNC(FTzero);
			return (EXP*)f;
		}
		break;
		
		case FTmone :
		{
			*((FUNC*)f) = FUNC(FTzero);
			return (EXP*)f;
		}
		break;
		
		case FTid :
		{
			*((FUNC*)f) = FUNC(FTone);
			return (EXP*)f;
		}
		break;
		
		case FTminus :
		{
			*((FUNC*)f) = FUNC(FTmone);
			return (EXP*)f;
		}
		break;
		
		case FTsquare :
		{
			delete f;
			f = new EXP(EOProduct);
			FUNC ftwo(FTcst);
			ftwo.setParam(2.0f);
			attach( (EXP&)*f, (EXP&)ftwo);
			FUNC fid(FTid);
			attach( (EXP&)*f, (EXP&)fid );
			return f->getArg(1);
		}
		break;
		
		case FTinverse :
		{
			//TODO : verify
			((FUNC*)f)->setArg( (EXP*)new FUNC(FTsquare) );
			return (EXP*)f->getArg(0);
		}
		break;
		
		case FTsin :
		{
			delete f;
			f = new FUNC(FTcos);
			return (EXP*)f;
		}
		break;
		
		case FTcos :
		{
			delete f;
			f = new EXP(EOProduct);
			FUNC fmone(FTmone);
			attach( (EXP&)*f, (EXP&)fmone );
			FUNC fsin(FTsin);
			attach( (EXP&)*f, (EXP&)fsin );
			return f->getArg(1);
		}
		break;
		
		case FTasin :
		{
			//TODO
			return (EXP*)f;
		}
		break;
		
		case FTacos :
		{
			//TODO
			return (EXP*)f;
		}
		break;
		
	}
}



EXP* derivateREC(const EXP& exp, const VAR& var)
{
	switch( exp.getType() )
	{
		case ETVAR :
		
		if( ((VAR*)&exp)->id == var.id)
		{
			//then the derivation is done with respect to that variable :
			FUNC* r = new FUNC(FTone);
			//r->setArg( (EXP*)new VAR( *((VAR*)&exp) ) );
			return (EXP*)r;
		}
		else
		{
			//then the derivation is done with respect to another variable :
			FUNC* r = new FUNC(FTzero);
			r->setArg( (EXP*)new VAR( *((VAR*)&exp) ) );
			return (EXP*)r;
		}
		
		break;
	
		case ETFUNC :
		return (EXP*)new EXP(derivateFUNCREC(exp,var));
		break;
		
		case ETEXP :
		return (EXP*)new EXP(derivateEXPREC(exp,var));
		break;
		
	}
}

EXP derivateFUNCREC(const EXP& exp, const VAR& var)
{
	//if we enter here, it means that exp is a FUNC...
	EXP* dexp = (EXP*)new FUNC( (FUNC&)exp);
	EXP* attachPoint = F2FDerivate( dexp);
	
	//there is only one argument to that function...
	//attach : make a copy of the node and take care of its EXPType :
	attach( *attachPoint, (EXP&)(*(exp.getArg(0))) );
	
	EXP r(EOProduct);
	attach( r, *dexp);
	//the copy being done and attached, we can delete it :
	delete dexp;
	
	EXP* temp = derivateREC( (EXP&)(*(exp.getArg(0))), var); 
	attach( r, *temp  );
	delete temp;
	
	return r;
}

EXP derivateEXPREC(const EXP& exp, const VAR& var)
{	
	EXP r;
	
	//let us store all the argument :
	int nbrArg = exp.getNBRArg();
	EXP** tabArg = new EXP*[nbrArg];
	for(int i=nbrArg;i--;)	tabArg[i] = exp.getArg(i);
	
	
	switch(exp.getOType())
	{
		case EOProduct :
		{
			r = EXP(EOSum);
			
			for(int i=nbrArg;i--;)
			{
				EXP ri(EOProduct);
	
				//let us attach to that node product, the arguments and the one being derivated.
				for(int j=nbrArg;j--;)	
				{
					if(j!=i)	attach(ri, (EXP&)(*tabArg[j]) );
				}
				//attach the one argument being derivated.
				EXP* temp =derivateREC( (EXP&)(*tabArg[i]), var); 
				attach(ri, *temp );
				delete temp;
				//attach the whole product node to the sum of product node that will be returned :
				attach(r, ri);
			}
		}
		break;
		
		case EOSum :
		{
			r= EXP(EOSum);
			for(int i=nbrArg;i--;)
			{
				//attach each argument being derivated.
				EXP* temp = derivateREC( (EXP&)(*tabArg[i]), var);  
				attach(r, *temp );
				delete temp;
			}
		}
		break;
		
		case EOId :
		{
			//it has only one argument...
			EXP* temp = derivateREC( (EXP&)(*tabArg[0]), var);
			r = *temp;
			delete temp;
		}
		break;
		
	}
	
	return r;
}
