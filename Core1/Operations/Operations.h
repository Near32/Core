#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../EXP.h"
#include "../MAT/Mat2.h"


template<typename T>
void toString(const Mat<EXP<T> >& m)
{
	std::cout << "Matrice : " << std::endl;
	
	for(int i=1;i<=m.getLine();i++)
	{
		std::cout << "\t";
		
		for(int j=1;j<=m.getColumn();j++)
		{
			std::cout << m(i,j).toString() << "\t"; 
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	
}

template<typename T>
void evaluate(const Mat<EXP<T> >& m)
{
	std::cout << "Matrice : " << std::endl;
	
	for(int i=1;i<=m.getLine();i++)
	{
		std::cout << "\t";
		
		for(int j=1;j<=m.getColumn();j++)
		{
			std::cout << m(i,j).evaluate() << "\t"; 
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	
}


template<typename T>
void regularize(Mat<EXP<T> >& m)
{	
	for(int i=1;i<=m.getLine();i++)
	{
		for(int j=1;j<=m.getColumn();j++)
		{
			regularize( m(i,j) );
		}
	}
}











//-----------------------------------------------------------------------------








template<typename T>
Mat<T> EXP2floatM( Mat<EXP<T> >& m)
{
	Mat<T> r(m.getLine(),m.getColumn());
	
	for(int i=1;i<=m.getLine();i++)
	{
		for(int j=1;j<=m.getColumn();j++)
		{
			r.set( m.get(i,j).evaluate(), i,j);
		}
	}

	return r;
}


template<typename T>
Mat<EXP<T> > product(const Mat<EXP<T> >& a, const Mat<EXP<T> >& b)
{
	if(a.getColumn() == b.getLine() )
	{
		return a*b;
	}
	else
	{
		std::cerr << "MATRICES SIZE DO NOT MATCHES... ABORTING." << std::endl;
		throw;
	}
}


template<typename T>
Mat<EXP<T> > regwM(const Mat<EXP<T> >& m)
{
	Mat<EXP<T> > r(m);
	
	for(int i=1;i<=r.getLine();i++)
	{
		for(int j=1;j<=r.getColumn();j++)
		{
			r(i,j) = regularize( r(i,j)  );
		}
	}	
	
	return r;
}



template<typename T>
Mat<EXP<T> > rotation(const EXP<T>& expvar, int axis)
{
	Mat<EXP<T> > r( EXP<T>(FTzero), 3,3);
	
	EXP<T> one(FTone);
	EXP<T> mone(FTmone);
	
	switch(axis)
	{
		case 1 :	//X axis		
		r.set(cos(expvar), 2,2);
		r.set(sin(expvar), 3,2);
		
		r.set(cos(expvar), 3,3);
		r.set(mone*sin(expvar), 2,3);
		
		r.set( one, 1,1);
		return r;
		break;
		
		case 2 : 	//Y axis
		r.set(cos(expvar), 1,1);
		r.set( mone*sin(expvar), 3,1);
		
		r.set(cos(expvar), 3,3);
		r.set(sin(expvar), 1,3);
		
		r.set( one, 2,2);
		return r;
		break;
		
		case 3 :	//Z axis
		r.set(cos(expvar), 1,1);
		r.set(sin(expvar), 2,1);

		r.set(cos(expvar), 2,2);
		r.set( mone*sin(expvar), 1,2);

		r.set( one, 3,3);
		return r;
		break;
		
		default :
		throw;
		break;
	}
	
	
}



//--------------------------------------------------------------------------




template<typename T>
EXP<T> F2FDerivate(const EXP<T>& f);

template<typename T>
EXP<T> derivateREC(const EXP<T>& exp, const EXP<T>& var);

template<typename T>
EXP<T> derivateFUNCREC(const EXP<T>& exp, const EXP<T>& var);

template<typename T>
EXP<T> derivateEXPREC(const EXP<T>& exp, const EXP<T>& var);

template<typename T>
Mat<EXP<T> > derivateV(const Mat<EXP<T> >& v, const EXP<T>& var);




template<typename T>
EXP<T> F2FDerivate(const EXP<T>& f)
{
	switch( f.getFType() )
	{
		case FTzero :
		{
			EXP<T> ret(FTzero);
			return ret;
		}
		break;
		
		case FTcst :
		{
			EXP<T> ret(FTzero);
			return ret;
		}
		break;
		
		case FTone :
		{
			EXP<T> ret(FTzero);
			return ret;
		}
		break;
		
		case FTmone :
		{
			EXP<T> ret(FTzero);
			return ret;
		}
		break;
		
		case FTid :
		{
			EXP<T> ret(FTone);
			return ret;
		}
		break;
		
		case FTminus :
		{
			EXP<T> ret(FTmone);
			return ret;
		}
		break;
		
		case FTsquare :
		{
			EXP<T> ret(EOProduct);
			EXP<T> ftwo(FTcst);
			ftwo.setParam(2.0f);
			
			ret.addArg( ftwo);
			
			EXP<T> fid(f.copyArg(0));
			
			ret.addArg( fid );
			
			return ret;
		}
		break;
		
		case FTinverse :
		{
			//TODO : verify
			EXP<T> ret(EOProduct);
			EXP<T> invX2(FTinverse);
			EXP<T> X(FTsquare);
			EXP<T> fmone(FTmone);
			
			X.addArg( f.copyArg(0) );
			invX2.addArg(X);
			
			ret.addArg(invX2);
			ret.addArg(fmone);
			
			return ret;
		}
		break;
		
		case FTsin :
		{
			EXP<T> ret(FTcos);
			ret.addArg( f.copyArg(0) );
			
			return ret;
		}
		break;
		
		case FTcos :
		{
			EXP<T> ret(EOProduct);
			EXP<T> fmone(FTmone);
			EXP<T> fsin(FTsin);
			
			fsin.addArg(f.copyArg(0));
			
			ret.addArg( fsin );
			ret.addArg( fmone);
			
			return ret;
		}
		break;
		
		case FTasin :
		{
			//TODO
			return EXP<T>(FTasin);
		}
		break;
		
		case FTacos :
		{
			//TODO
			return EXP<T>(FTacos);
		}
		break;
		
	}
}


template<typename T>
EXP<T> derivateREC(const EXP<T>& exp, const EXP<T>& var)
{
	switch( exp.getType() )
	{
		case ETVAR :
		{
			if( exp.VARID == var.VARID)
			{
				EXP<T> ret(FTone);
				return ret;
			}
			else
			{
				EXP<T> ret(FTzero);
				return ret;
			}
		}
		break;
	
		case ETFUNC :
		{
			return derivateFUNCREC<T>(exp,var);
		}
		break;
		
		case ETEXP :
		{
			return derivateEXPREC<T>(exp,var);
		}
		break;
		
	}
	
	
}

template<typename T>
EXP<T> derivateFUNCREC(const EXP<T>& exp, const EXP<T>& var)
{
	//if we enter here, it means that exp is a FUNC...
	EXP<T> derivExpf( F2FDerivate( exp) );
	
	//there is only one argument to that function, or none...
	int nbrarg = exp.getNbrArg();
	if(nbrarg == 1)
	{
		EXP<T> ret(EOProduct);
		ret.addArg( derivExpf);
	
		EXP<T> tempFollowingBranchX( derivateREC( exp.copyArg(0), var) ); 
		ret.addArg( tempFollowingBranchX);
		
		return ret;
	}
	else if( nbrarg == 0)
	{
		//no need for a product, it is only valuable for constant function...
		return derivExpf;
	}
	else
	{
		throw;
	}
}


template<typename T>
EXP<T> derivateEXPREC(const EXP<T>& exp, const EXP<T>& var)
{	
	//let us store all the argument :
	int nbrArg = exp.getNbrArg();
	std::vector<EXP<T> > tabArg;
	for(int i=nbrArg;i--;)	tabArg.push_back( exp.copyArg(i) );
	
	
	switch(exp.getOType())
	{
		case EOProduct :
		{
			EXP<T> ret(EOSum);
			
			for(int i=nbrArg;i--;)
			{
				EXP<T> ri(EOProduct);
	
				//let us attach to that node product, the arguments and the one being derivated.
				for(int j=nbrArg;j--;)	
				{
					if(j!=i)
					{
						ri.addArg( tabArg[j] );
					}
				}
				
				//attach the one argument being derivated.
				ri.addArg( derivateREC( tabArg[i], var) ); 
				
				ret.addArg( ri);
			}
			
			return ret;
		}
		break;
		
		case EOSum :
		{
			EXP<T> ret(EOSum);
			for(int i=nbrArg;i--;)
			{
				//attach each argument being derivated.
				ret.addArg( derivateREC( tabArg[i], var) );  
			}
			
			return ret;
		}
		break;
		
		case EOId :
		{
			//it has only one argument...
			if( tabArg.size() == 1)
			{
				return derivateREC( tabArg[0], var);
			}
			else
			{
				throw;
			}
			
		}
		break;
		
	}
}



template<typename T>
Mat<EXP<T> > derivateV(const Mat<EXP<T> >& v, const EXP<T>& var)
{
	Mat<EXP<T> > r(EXP<T>(FTzero), v.getLine(), v.getColumn(), v.getDepth() );
	
	for(int k=1;k<=v.getDepth();k++)
	{
		for(int i=1;i<=v.getLine();i++)
		{
			for(int j=1;j<=v.getColumn();j++)
			{
				r(i,j,k) = derivateREC( v(i,j,k), var);
				regularize( r(i,j,k) );
			}
		}
	}
	
	return r;
}


#endif
