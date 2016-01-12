#include "Operations.h"

EXP derivateREC(const EXP& exp, const VAR& var)
{
	switch( exp.getType() )
	{
		case ETVAR :
		
		if( *((VAR*)&exp)->id == var.id)
		{
			//then the derivation is done with respect to that variable :
			FUNC r(FTId);
			r.setArg( (EXP*)new VAR( *((VAR*)&exp) ) );
			return r;
		}
		else
		{
			//then the derivation is done with respect to another variable :
			FUNC r(FTzero);
			r.setArg( (EXP*)new VAR( *((VAR*)&exp) ) );
			return r;
		}
		
		break;
	
		case ETFUNC :
		return derivateFUNCREC(exp,var);
		break;
		
		case ETEXP :
		return derivateEXPREC(exp,var);
		break;
		
	}
}
