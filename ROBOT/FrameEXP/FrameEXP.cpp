#include "FrameEXP.h"

FrameEXP::FrameEXP() : Frame()
{
	variable = NULL;
	T = NULL;
}

FrameEXP::FrameEXP(const VAR& variable_, int idxParameter_, const Mat<float>& se3Init) : Frame(idxParameter_,se3Init)
{
	variable = new VAR(variable_);
	
	generateT();
}

FrameEXP::~FrameEXP()
{
	if(variable != NULL)	delete variable;
	if(T != NULL)	delete T;
	
}
	
//-----------------------------
void FrameEXP::setVAR(const VAR& v)
{
	if(variable == NULL)
	{
		variable = new VAR(v);
	}
	else
	{
		delete variable;
		variable = new VAR(v);
	}
}

void FrameEXP::generateT()
{
	Mat<EXP> se3EXP(se3.getLine(),se3.getColumn());
	for(int i = this->se3.getLine();i--;)
	{
		if(i+1 == this->idxParameter)
		{
			se3EXP.set( *(this->variable), i+1,1);
		}
		else
		{
			EXP temp(EOId);
			FUNC ftemp(FTcst);
			ftemp.setParam( this->se3.get(i+1,1) );
			attach( temp, (EXP&)ftemp);
			
			se3EXP.set( temp, i+1,1);
		}
	}
	
	Mat<EXP> Rxe( rotation( se3EXP.get(1,1), 1) );
	Mat<EXP> Rye( rotation( se3EXP.get(2,1), 2) );
	Mat<EXP> Rze( rotation( se3EXP.get(3,1), 3) );
	Mat<EXP> te( extract( &se3EXP,4,1,6,1) );
	
	EXP zero3(EOId);
	FUNC fzero(FTzero);
	attach( zero3, (EXP&)fzero );
	
	EXP one1(EOId);
	FUNC fone(FTone);
	attach( one1, (EXP&)fone );
	
	T =  new Mat<EXP>( operatorL( operatorC( Rze*Rye*Rxe, Mat<EXP>( zero3,1,3) ), operatorC( te ,Mat<EXP>( one1,1,1) ) ) );
}


