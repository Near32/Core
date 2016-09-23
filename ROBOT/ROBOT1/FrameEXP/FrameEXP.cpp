#include "FrameEXP.h"

FrameEXP::FrameEXP() : Frame()
{
	variable = NULL;
	T = NULL;
}

FrameEXP::FrameEXP(const EXP<float>& variable_, int idxParameter_, const Mat<float>& se3Init) : Frame(idxParameter_,se3Init)
{
	variable = new EXP<float>(variable_);
	
	generateT();
}

FrameEXP::~FrameEXP()
{
	if(variable != NULL)	delete variable;
	if(T != NULL)	delete T;
	
}
	
//-----------------------------
void FrameEXP::setVAR(const EXP<float>& v)
{
	if(variable == NULL)
	{
		variable = new EXP<float>(v);
	}
	else
	{
		delete variable;
		variable = new EXP<float>(v);
	}
}

void FrameEXP::generateT()
{
	Mat<EXP<float> > se3EXP(se3.getLine(),se3.getColumn());
	for(int i = 0;i<this->se3.getLine();i++)
	{
		if(i+1 == this->idxParameter)
		{
			se3EXP.set( *(this->variable), i+1,1);
		}
		else
		{
			EXP<float> temp(FTcst, this->se3.get(i+1,1));
			se3EXP.set( temp, i+1,1);
		}
	}
	
	Mat<EXP<float> > Rxe( rotation( se3EXP.get(1,1), 1) );
	Mat<EXP<float> > Rye( rotation( se3EXP.get(2,1), 2) );
	Mat<EXP<float> > Rze( rotation( se3EXP.get(3,1), 3) );
	Mat<EXP<float> > te( extract( &se3EXP,4,1,6,1) );
	
	EXP<float> zero3(FTzero);
	
	EXP<float> one1(FTone);
	
	/*
	//toString(Rxe);
	evaluate(Rxe);
	//toString(Rye);
	evaluate(Rye);
	toString(se3EXP);
	//toString(Rze);
	evaluate(Rze);
	*/
	
	T =  new Mat<EXP<float> >( operatorL( operatorC( product( Rxe, product(Rye,Rze) ), Mat<EXP<float> >( zero3,1,3) ), operatorC( te ,Mat<EXP<float> >( one1,1,1) ) ) );
	
	//evaluate( *T);
}


