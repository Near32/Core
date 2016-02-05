#include "HAROLegsEXP.h"

/*<<<<<<< HEAD
HAROLegsEXP::HAROLegsEXP() : x1r(VAR(0.0)), x2r(VAR(0.0)), x3r(VAR(0.0)), x4r(VAR(0.0)), x5r(VAR(0.0)), ankle2footr(VAR(0.0)), x1l(VAR(0.0)), x2l(VAR(0.0)), x3l(VAR(0.0)), x4l(VAR(0.0)), x5l(VAR(0.0)), ankle2footl(VAR(0.0)), Tr(Mat<EXP>(4,4)), Tl(Mat<EXP>(4,4)), rbr(Mat<EXP>(4,1)), rbl(Mat<EXP>(4,1)), rrb(Mat<EXP>(4,1)), rlb(Mat<EXP>(4,1)), rarb(Mat<EXP>(4,1)), ralb(Mat<EXP>(4,1))
=======
#define moveRAD 2.5f

HAROLegsEXP::HAROLegsEXP() : x1r(VAR(0.0)), x2r(VAR(0.0)), x3r(VAR(0.0)), x4r(VAR(0.0)), x5r(VAR(0.0)), ankle2footr(VAR(0.0)), x1l(VAR(0.0)), x2l(VAR(0.0)), x3l(VAR(0.0)), x4l(VAR(0.0)), x5l(VAR(0.0)), ankle2footl(VAR(0.0))
>>>>>>> 18d222c75bb91562084fb2b8f5c9487498c5ebb5*/
#define moveRAD 2.5f

extern mutex mutexRES;


HAROLegsEXP::HAROLegsEXP() : x1r(VAR(0.0)), x2r(VAR(0.0)), x3r(VAR(0.0)), x4r(VAR(0.0)), x5r(VAR(0.0)), ankle2footr(VAR(0.0)), x1l(VAR(0.0)), x2l(VAR(0.0)), x3l(VAR(0.0)), x4l(VAR(0.0)), x5l(VAR(0.0)), ankle2footl(VAR(0.0)), Tr(Mat<EXP>(4,4)), Tl(Mat<EXP>(4,4)), rbr(Mat<EXP>(4,1)), rbl(Mat<EXP>(4,1)), rrb(Mat<EXP>(4,1)), rlb(Mat<EXP>(4,1)), rarb(Mat<EXP>(4,1)), ralb(Mat<EXP>(4,1))
{
	varnumberR = 0;
	varnumberL = 0;
	
	pe = new PE();
	
	constructFrames();
	
	nbrR = servosR.size();
	nbrL = servosL.size();
	
	velocitiesR = Mat<float>((float)0,nbrR,1);
	velocitiesL = Mat<float>((float)0,nbrL,1);
	
	continuer = true;
}

HAROLegsEXP::~HAROLegsEXP()
{
	for(int i=servosR.size();i--;)	delete servosR[i];
	for(int i=servosL.size();i--;)	delete servosL[i];
	
	for(int i=tW2R.size();i--;)	delete tW2R[i];
	for(int i=tW2L.size();i--;)	delete tW2L[i];
	
	delete pe;
}
	
void HAROLegsEXP::loop()
{
	clock_t timer = clock();
	float dt;
	
	Tr = regwM( tW2R[0]->getT() );
	Tl = regwM( tW2L[0]->getT() );

	evaluate( Tr );
	evaluate( Tr );
	
	clock_t time = clock();
	for(int i=1;i<3;i++)
	{
		evaluate(tW2R[i]->getT());
		//toString( regwM( tW2R[i]->getT() ) );
		Mat<EXP> temp( regwM(tW2R[i]->getT()) );
		//Mat<EXP> temp( tW2R[i]->getT() );
		//toString( temp); 
		Tr = product( regwM(Tr), temp );
		//T = product( T, temp );
		
		if(i==3)
		{
			rarb = extract(Tr, 4,1, 4,4);
		}
	}
	rrb = extract( Tr, 1,4, 4,4);
	FUNC fmone( FTmone);
	Mat<EXP> mT(Tr.getLine(),Tr.getColumn());
	for(int i=1;i<=mT.getLine();i++)
	{
		for(int j=1;j<=mT.getColumn();j++)
		{
			mT.set( fmone*Tr.get(i,j), i,j);
		}
	}
	
	rbr = ( mT) * rrb;
	
	evaluate( Tr);
	std::cout << "THE MULTIPLICATIONS TOOK : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	time = clock();
	
	for(int i=1;i<6;i++)
	{
		evaluate(tW2L[i]->getT());
		//toString( regwM( tW2R[i]->getT() ) );
		Mat<EXP> temp( regwM(tW2L[i]->getT()) );
		//Mat<EXP> temp( tW2R[i]->getT() );
		//toString( temp); 
		Tl = product( regwM(Tl), temp );
		//T = product( T, temp );
		
		if(i==3)
		{
			ralb = extract(Tl, 4,1, 4,4);
		}
	}
	rlb = extract( Tl, 1,4, 4,4);
	
	for(int i=1;i<=mT.getLine();i++)
	{
		for(int j=1;j<=mT.getColumn();j++)
		{
			mT.set( fmone*Tl.get(i,j), i,j);
		}
	}
	rbl = ( mT )*rlb;
	
	evaluate( Tl);
	std::cout << "THE MULTIPLICATIONS TOOK : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	
	
	
	while(continuer)
	{
		
		dt = (float)(clock()-timer)/CLOCKS_PER_SEC;
		timer = clock();
		
		//TODO : re-implement the PE class so that
		// it would be using this dt value.
		//pe->callback(dt);
		
		//UPDATE SERVOS :
		/*for(int i=1;i<=nbrR;i++)
		{
			mutexRES.lock();
			if(velocitiesR.get(i,1) != 0.0f)
			{
				servosR[i]->set( servosR[i]->get() + dt*velocitiesR.get(i,1) );
			}
			mutexRES.unlock();
		}
		for(int i=1;i<=nbrL;i++)
		{
			if(velocitiesL.get(i,1) != 0.0f)
			{
				servosL[i]->set( servosL[i]->get() + dt*velocitiesL.get(i,1) );
			}
		}*/
		//----------------------------------------
		
		//----------------------------------------
		//gestion des commandes :
		if( listCommand.size() >= 1)
		{
			mutexRES.lock();
			switch( listCommand[0]->type)
			{
				default :
				{
					std::cout << "COMMAND RECEIVED : ICTnone..." << std::endl;
				}
				break;
				
				case ICTnextvar :
				{
					if(listCommand[0]->side == SIDEright)
					{
						std::cout << "COMMAND RECEIVED : ICTnextvar... : " << varnumberR << " -> ";
						if( varnumberR < 5)	varnumberR++;
						std::cout << varnumberR << std::endl;
					}
					else
					{
						std::cout << "COMMAND RECEIVED : ICTnextvar... : " << varnumberL << " -> ";
						if( varnumberL < 5)	varnumberL++;
						std::cout << varnumberL << std::endl;
					}
				}
				break;
				
				case ICTpreviousvar :
				{
					if(listCommand[0]->side == SIDEright)
					{
						std::cout << "COMMAND RECEIVED : ICTpreviousvar... : " << varnumberR << " -> ";
						if( varnumberR > 1)	varnumberR--;
						std::cout << varnumberR << std::endl;
					}
					else
					{
						std::cout << "COMMAND RECEIVED : ICTpreviousvar... : " << varnumberL << " -> ";
						if( varnumberL > 1)	varnumberL--;
						std::cout << varnumberL << std::endl;
					}
				}
				break;
				
				case ICTupvar :
				{
					if(listCommand[0]->side == SIDEright)
					{
						std::cout << "COMMAND RECEIVED : ICTupvar... : " << varnumberR << " = ";
					
						switch(varnumberR)
						{
							case 1:
							{
								x1r.setValue( x1r.getValue()+toRad(moveRAD) );
								std::cout << x1r.evaluate() << std::endl;
							}
							break;
						
							case 2:
							{
								x2r.setValue( x2r.getValue()+toRad(moveRAD) );
								std::cout << x2r.evaluate() << std::endl;
							}
							break;
						
							case 3:
							{
								x3r.setValue( x3r.getValue()+toRad(moveRAD) );
								std::cout << x3r.evaluate() << std::endl;
							}
							break;
						
							case 4:
							{
								x4r.setValue( x4r.getValue()+toRad(moveRAD) );
								std::cout << x4r.evaluate() << std::endl;
							}
							break;
						
							case 5:
							{
								x5r.setValue( x5r.getValue()+toRad(moveRAD) );
								std::cout << x5r.evaluate() << std::endl;
							}
							break;
						}
					}
					else
					{
						std::cout << "COMMAND RECEIVED : ICTupvar... : " << varnumberL << " = ";
					
						switch(varnumberL)
						{
							case 1:
							{
								x1l.setValue( x1l.getValue()+toRad(moveRAD) );
								std::cout << x1l.evaluate() << std::endl;
							}
							break;
						
							case 2:
							{
								x2l.setValue( x2l.getValue()+toRad(moveRAD) );
								std::cout << x2l.evaluate() << std::endl;
							}
							break;
						
							case 3:
							{
								x3l.setValue( x3l.getValue()+toRad(moveRAD) );
								std::cout << x3l.evaluate() << std::endl;
							}
							break;
						
							case 4:
							{
								x4l.setValue( x4l.getValue()+toRad(moveRAD) );
								std::cout << x4l.evaluate() << std::endl;
							}
							break;
						
							case 5:
							{
								x5l.setValue( x5l.getValue()+toRad(moveRAD) );
								std::cout << x5l.evaluate() << std::endl;
							}
							break;
						}
					}
					
				}
				break;
				
				case ICTdownvar :
				{
					if(listCommand[0]->side == SIDEright)
					{
						std::cout << "COMMAND RECEIVED : ICTdownvar... : " << varnumberR << " = ";
					
						switch(varnumberR)
						{
							case 1:
							{
								x1r.setValue( x1r.getValue()-toRad(moveRAD) );
								std::cout << x1r.evaluate() << std::endl;
							}
							break;
						
							case 2:
							{
								x2r.setValue( x2r.getValue()-toRad(moveRAD) );
								std::cout << x2r.evaluate() << std::endl;
							}
							break;
						
							case 3:
							{
								x3r.setValue( x3r.getValue()-toRad(moveRAD) );
								std::cout << x3r.evaluate() << std::endl;
							}
							break;
						
							case 4:
							{
								x4r.setValue( x4r.getValue()-toRad(moveRAD) );
								std::cout << x4r.evaluate() << std::endl;
							}
							break;
						
							case 5:
							{
								x5r.setValue( x5r.getValue()-toRad(moveRAD) );
								std::cout << x5r.evaluate() << std::endl;
							}
							break;
						}
					}
					else
					{
						std::cout << "COMMAND RECEIVED : ICTdownvar... : " << varnumberL << " = ";
					
						switch(varnumberL)
						{
							case 1:
							{
								x1l.setValue( x1l.getValue()-toRad(moveRAD) );
								std::cout << x1l.evaluate() << std::endl;
							}
							break;
						
							case 2:
							{
								x2l.setValue( x2l.getValue()-toRad(moveRAD) );
								std::cout << x2l.evaluate() << std::endl;
							}
							break;
						
							case 3:
							{
								x3l.setValue( x3l.getValue()-toRad(moveRAD) );
								std::cout << x3l.evaluate() << std::endl;
							}
							break;
						
							case 4:
							{
								x4l.setValue( x4l.getValue()-toRad(moveRAD) );
								std::cout << x4l.evaluate() << std::endl;
							}
							break;
						
							case 5:
							{
								x5l.setValue( x5l.getValue()-toRad(moveRAD) );
								std::cout << x5l.evaluate() << std::endl;
							}
							break;
						}
					}
				}
				break;
			}
			mutexRES.unlock();
			
			//deletion :			
			listCommand.erase( listCommand.begin()+0);
			
			for(int i = servosR.size();i--;)
			{
				mutexRES.lock();
				servosR[i]->setVARVALUE();
				mutexRES.unlock();
			}
	
			for(int i = servosL.size();i--;)
			{
				mutexRES.lock();
				servosL[i]->setVARVALUE();
				mutexRES.unlock();
			}
		}
		
	}
}

void HAROLegsEXP::setDq( const Mat<float>& dq)
{
	for(int i=1;i<=5;i++)
	{
		mutexRES.lock();
		switch(i)
		{
			case 1:
			{
				x1r.setValue( x1r.getValue()+toRad(dq.get(1,1)) );
				//std::cout << x1r.evaluate() << std::endl;
			}
			break;
	
			case 2:
			{
				x2r.setValue( x2r.getValue()+toRad(dq.get(2,1)) );
				//std::cout << x2r.evaluate() << std::endl;
			}
			break;
	
			case 3:
			{
				x3r.setValue( x3r.getValue()+toRad(dq.get(3,1)) );
				//std::cout << x3r.evaluate() << std::endl;
			}
			break;
	
			case 4:
			{
				x4r.setValue( x4r.getValue()+toRad(dq.get(4,1)) );
				//std::cout << x4r.evaluate() << std::endl;
			}
			break;
	
			case 5:
			{
				x5r.setValue( x5r.getValue()+toRad(dq.get(5,1)) );
				//std::cout << x5r.evaluate() << std::endl;
			}
			break;
		}
		mutexRES.unlock();
	}
	
	for(int i=1;i<=5;i++)
	{
		mutexRES.lock();
		switch(i)
		{
			case 1:
			{
				x1l.setValue( x1l.getValue()+toRad(dq.get(6,1)) );
				//std::cout << x1l.evaluate() << std::endl;
			}
			break;
	
			case 2:
			{
				x2l.setValue( x2l.getValue()+toRad(dq.get(7,1)) );
				//std::cout << x2l.evaluate() << std::endl;
			}
			break;
	
			case 3:
			{
				x3l.setValue( x3l.getValue()+toRad(dq.get(8,1)) );
				//std::cout << x3l.evaluate() << std::endl;
			}
			break;
	
			case 4:
			{
				x4l.setValue( x4l.getValue()+toRad(dq.get(9,1)) );
				//std::cout << x4l.evaluate() << std::endl;
			}
			break;
	
			case 5:
			{
				x5l.setValue( x5l.getValue()+toRad(dq.get(10,1)) );
				//std::cout << x5l.evaluate() << std::endl;
			}
			break;
		}
		mutexRES.unlock();
	}
	
	for(int i = servosR.size();i--;)
	{
		mutexRES.lock();
		servosR[i]->setVARVALUE();
		mutexRES.unlock();
	}
	
	for(int i = servosL.size();i--;)
	{
		mutexRES.lock();
		servosL[i]->setVARVALUE();
		mutexRES.unlock();
	}
}
	
	
	//------------------------------
void HAROLegsEXP::constructFrames()
{
	Mat<float> finalse3((float)0,6,1);
	
	int pinL[5] = {25,24,23,22,21};
	int pinR[5] = {5,26,27,28,29};
	int idx=0;
	
	//in relative manner : from frame n-1 to frame n.
	//tW2R.insert( tW2R.end(), expM(finalse3) );
	//tW2L.insert( tW2L.end(), expM(finalse3) );
	
	/*	
	float W2hY = -0.1f;//-10 centimeters.
	float h2hlY = 0.075f;	//7.5 centimeters.
	float hl2kX = 0.1f;
	float k2hlX = 0.05f;
	float hl2hllX = 0.075f;
	float hl2fX = 0.075f;
	*/
	float W2hY = -0.08f;//-10 centimeters.
	float h2hlY = 0.08f;	//7.5 centimeters.
	float hl2kX = 0.08f;
	float k2aX = 0.08f;
	float a2alX = 0.08f;
	float al2fX = 0.075f;
	
	//--------------------------------------------------------
	//TODO : inversion in Servomotors with respect to Servo class :
	// add in the class + check in reality ...
	//right hip : nuz, 80 at least offset
	/*
	finalse3.set( W2hY, 5,1);
	finalse3.set( -90.0f, 1,1);
	finalse3.set( 90.0f, 2,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,90.0f) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[0] );
	*/
	//right hip : nuz, 80 at least offset
	finalse3.set( W2hY, 5,1);
	finalse3.set( toRad(-90.0f), 1,1);
	finalse3.set( toRad(90.0f), 2,1);
	tW2R.insert( tW2R.end(), new FrameEXP( x1r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP(x1r, pinR[idx],3,finalse3,120) );
	servosR[servosR.size()-1]->inverted = true;
	x1r.setValue( toRad(20.0f) );
	servosR[servosR.size()-1]->setVARVALUE();
	
	//left hip : nuz, 80 at least offset
	finalse3 *= 0.0f;
	finalse3.set( -W2hY, 5,1);
	finalse3.set( toRad(90.0f), 1,1);
	finalse3.set( toRad(90.0f), 2,1);
	tW2L.insert( tW2L.end(), new FrameEXP( x1l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP(x1l, pinL[idx],3,finalse3, 120) );
	servosL[servosL.size()-1]->inverted = true;
	x1l.setValue( toRad(-20.0f) );
	servosL[servosL.size()-1]->setVARVALUE();
	idx++;
	
	//--------------------------------------------------------	
	/*
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( -90.0f, 1,1);
	finalse3.set( -90.0f, 2,1);
	servosR.insert( servosR.end(), new Servo( pinR[idx],3,finalse3,50) );
	idx++;
	Mat<float> temp(expM(finalse3)*tW2R[1]);
	tW2R.insert( tW2R.end(), temp );
	*/
	
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( toRad(-90.0f), 1,1);
	finalse3.set( toRad(-90.0f), 2,1);
	tW2R.insert( tW2R.end(), new FrameEXP( x2r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP( x2r, pinR[idx],3,finalse3,120) );
	servosR[servosR.size()-1]->inverted = true;
	x2r.setValue( toRad(20) );
	servosR[servosR.size()-1]->setVARVALUE();
	
	//left hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( -h2hlY, 5,1);
	finalse3.set( toRad(90.0f), 1,1);
	finalse3.set( toRad(-90.0f), 2,1);
	tW2L.insert( tW2L.end(), new FrameEXP( x2l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x2l, pinL[idx],3,finalse3,120) );
	servosL[servosL.size()-1]->inverted = true;
	x2l.setValue( toRad(-20) );
	servosL[servosL.size()-1]->setVARVALUE();
	idx++;
	
	//--------------------------------------------------------	
	/*
	//right hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[2] );
	*/
	
	//right hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x3r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP( x3r, pinR[idx],3,finalse3,120) );
	servosR[servosR.size()-1]->inverted = false;
	x3r.setValue( toRad(0) );
	servosR[servosR.size()-1]->setVARVALUE();
	
	//left hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x3l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x3l, pinL[idx],3,finalse3,120) );
	servosL[servosL.size()-1]->inverted = false;
	x3l.setValue( toRad(0) );
	servosL[servosL.size()-1]->setVARVALUE();
	idx++;
	
	//--------------------------------------------------------
	/*	
	//right knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2hlX, 4,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[3] );
	*/
	
	
	//right knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2aX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x4r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP( x4r, pinR[idx],3,finalse3,120) );
	servosR[servosR.size()-1]->inverted = true;
	x4r.setValue( toRad(0) );
	servosR[servosR.size()-1]->setVARVALUE();
	
	//left knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2aX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x4l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x4l, pinL[idx],3,finalse3,120) );
	servosL[servosL.size()-1]->inverted = true;
	x4l.setValue( toRad(0) );
	servosL[servosL.size()-1]->setVARVALUE();
	idx++;
	
	
	//--------------------------------------------------------
	/*
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2hllX, 4,1);
	finalse3.set( 90.0f, 1,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[4] );
	*/
	
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( toRad(90.0f), 1,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x5r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP( x5r, pinR[idx],3,finalse3,120) );
	servosR[servosR.size()-1]->inverted = true;
	x5r.setValue( toRad(0) );
	servosR[servosR.size()-1]->setVARVALUE();
	
	//left heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( toRad(90.0f), 1,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x5l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x5l, pinL[idx],3,finalse3,120) );
	servosR[servosR.size()-1]->inverted = true;
	x5l.setValue( toRad(0) );
	servosL[servosR.size()-1]->setVARVALUE();
	idx++;
	
	//--------------------------------------------------------
	/*
	//right heel low to foot : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2fX, 4,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );	
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[5] );
	*/
	
	//right heel low to foot :
	finalse3 *= 0.0f;
	finalse3.set( al2fX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(ankle2footr,3,finalse3) );
	
	//left heel low to foot :
	finalse3 *= 0.0f;
	finalse3.set( al2fX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(ankle2footl,3,finalse3) );
	
	
	//------------------------------------------
	//------------------------------------------
	
	//LEFT LEG :
	/*
	idx=0;
	//left hip : nuz, 80 at least offset
	finalse3.set( -W2hY, 5,1);
	finalse3.set( 90.0f, 1,1);
	finalse3.set( 90.0f, 2,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,90.0f) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[0] );
	
	
	//left hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( -h2hlY, 5,1);
	finalse3.set( 90.0f, 1,1);
	finalse3.set( -90.0f, 2,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,50) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[1] );
	
	
	//left hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,50) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[2] );
	
	
	//left knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2hlX, 4,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,50) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[3] );
	
	
	//left heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2hllX, 4,1);
	finalse3.set( 90.0f, 1,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,50) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[4] );
	
	//left heel low to foot : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2fX, 4,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,90.0f) );
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[5] );
	*/
}


Mat<EXP> HAROLegsEXP::getSE3_one2next( int numero, bool right)
{
	Mat<EXP> r(4,4);
	
	if(numero < tW2R.size() )
	{
		r = (right ? tW2R[numero]->getT() : tW2L[numero]->getT() );
	}
	
	return r;
}

Mat<EXP> HAROLegsEXP::getW2FINAL(bool right)
{
	/*
	Mat<float> r( (float)0,4,1 );
	for(int i=1;i<=4;i++)	r.set( 1.0f,i,i);
	
	for(int i=tW2R.size();i--;)		r *= (right? tW2R[i] : tW2L[i]);
	
	return r;
	*/
	return (right? tW2R[tW2R.size()-1]->getT() : tW2L[tW2L.size()-1]->getT() );
}


void HAROLegsEXP::stop()
{
	continuer = false;
}


void HAROLegsEXP::addCommand( ICommand* command)
{
	listCommand.insert( listCommand.begin(), std::unique_ptr<ICommand>(command) );
}


Mat<EXP> HAROLegsEXP::getRbassinR()	const
{
	return rbr;
}

Mat<EXP> HAROLegsEXP::getRbassinL()	const
{
	return rbl;
}

Mat<EXP> HAROLegsEXP::getRfootr()	const
{
	return rrb;
}

Mat<EXP> HAROLegsEXP::getRfootl()	const
{
	return rlb;
}


Mat<EXP> HAROLegsEXP::getRkneer()	const
{
	return rarb;
}

Mat<EXP> HAROLegsEXP::getRkneel()	const
{
	return ralb;
}

Mat<EXP> HAROLegsEXP::generateJacobian(const Mat<EXP>& v)	const
{
	Mat<EXP> dvdRX1( regwM( derivateV( v, x1r) ) );
	Mat<EXP> dvdRX2( regwM( derivateV( v, x2r) ) );
	Mat<EXP> dvdRX3( regwM( derivateV( v, x3r) ) );
	Mat<EXP> dvdRX4( regwM( derivateV( v, x4r) ) );
	Mat<EXP> dvdRX5( regwM( derivateV( v, x5r) ) );
	
	Mat<EXP> dvdLX1( regwM( derivateV( v, x1l) ) );
	Mat<EXP> dvdLX2( regwM( derivateV( v, x2l) ) );
	Mat<EXP> dvdLX3( regwM( derivateV( v, x3l) ) );
	Mat<EXP> dvdLX4( regwM( derivateV( v, x4l) ) );
	Mat<EXP> dvdLX5( regwM( derivateV( v, x5l) ) );
	
	Mat<EXP> dvdQ( operatorL( dvdRX1, dvdRX2) );
	dvdQ = operatorL( dvdQ, dvdRX3);
	dvdQ = operatorL( dvdQ, dvdRX4);
	dvdQ = operatorL( dvdQ, dvdRX5);
	
	dvdQ = operatorL( dvdQ, dvdLX1);
	dvdQ = operatorL( dvdQ, dvdLX2);
	dvdQ = operatorL( dvdQ, dvdLX3);
	dvdQ = operatorL( dvdQ, dvdLX4);
	dvdQ = operatorL( dvdQ, dvdLX5);
	
	return dvdQ;
}



