#include "HAROLegsEXP.h"

float toRad(const float& deg)
{
	return (deg*PI)/180.0f;
}

HAROLegsEXP::HAROLegsEXP()
{
	pe = new PE();
	
	
	x1r = VAR(0.0);
	x2r = VAR(0.0);
	x3r = VAR(0.0);
	x4r = VAR(0.0);
	x5r = VAR(0.0);
	ankle2footr = VAR(0.0);
	
	x1l = VAR(0.0);
	x2l = VAR(0.0);
	x3l = VAR(0.0);
	x4l = VAR(0.0);
	x5l = VAR(0.0);
	ankle2footl = VAR(0.0);
	
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
	
	Mat<EXP> Tr( regwM( tW2R[0]->getT() ) );
	Mat<EXP> Tl( regwM( tW2L[0]->getT() ) );

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
	}
	
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
	}
	
	evaluate( Tl);
	std::cout << "THE MULTIPLICATIONS TOOK : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	
	
	
	while(continuer)
	{
		
		dt = (float)(clock()-timer)/CLOCKS_PER_SEC;
		timer = clock();
		
		//TODO : re-implement the PE class so that
		// it would be using this dt value.
		pe->callback(dt);
		
		//UPDATE SERVOS :
		for(int i=1;i<=nbrR;i++)
		{
			servosR[i]->set( servosR[i]->get() + dt*velocitiesR.get(i,1) );
		}
		for(int i=1;i<=nbrL;i++)
		{
			servosL[i]->set( servosL[i]->get() + dt*velocitiesL.get(i,1) );
		}
		//----------------------------------------
		
		
		
	}
}
	
	
	//------------------------------
void HAROLegsEXP::constructFrames()
{
	Mat<float> finalse3((float)0,6,1);
	
	int pinR[5] = {21,22,23,24,25};
	int pinL[5] = {5,26,27,28,29};
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
	finalse3.set( -90, 1,1);
	finalse3.set( 90, 2,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,90) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[0] );
	*/
	//right hip : nuz, 80 at least offset
	finalse3.set( W2hY, 5,1);
	finalse3.set( toRad(-90), 1,1);
	finalse3.set( toRad(90), 2,1);
	tW2R.insert( tW2R.end(), new FrameEXP( x1r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP(x1r, pinR[idx],3,finalse3,90) );
	
	//left hip : nuz, 80 at least offset
	finalse3 *= 0.0f;
	finalse3.set( -W2hY, 5,1);
	finalse3.set( toRad(90), 1,1);
	finalse3.set( toRad(90), 2,1);
	tW2L.insert( tW2L.end(), new FrameEXP( x1l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP(x1l, pinL[idx],3,finalse3, 90) );
	idx++;
	
	//--------------------------------------------------------	
	/*
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( -90, 1,1);
	finalse3.set( -90, 2,1);
	servosR.insert( servosR.end(), new Servo( pinR[idx],3,finalse3,50) );
	idx++;
	Mat<float> temp(expM(finalse3)*tW2R[1]);
	tW2R.insert( tW2R.end(), temp );
	*/
	
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( toRad(-90), 1,1);
	finalse3.set( toRad(-90), 2,1);
	tW2R.insert( tW2R.end(), new FrameEXP( x2r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP( x2r, pinR[idx],3,finalse3,50) );
	
	//left hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( -h2hlY, 5,1);
	finalse3.set( toRad(90), 1,1);
	finalse3.set( toRad(-90), 2,1);
	tW2L.insert( tW2L.end(), new FrameEXP( x2l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x2l, pinL[idx],3,finalse3,50) );
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
	servosR.insert( servosR.end(), new ServoEXP( x3r, pinR[idx],3,finalse3,50) );
	
	//left hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x3l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x3l, pinL[idx],3,finalse3,50) );
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
	servosR.insert( servosR.end(), new ServoEXP( x4r, pinR[idx],3,finalse3,50) );
	
	//left knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2aX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x4l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x4l, pinL[idx],3,finalse3,50) );
	idx++;
	
	
	//--------------------------------------------------------
	/*
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2hllX, 4,1);
	finalse3.set( 90, 1,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[4] );
	*/
	
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( toRad(90), 1,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x5r,3,finalse3) );
	servosR.insert( servosR.end(), new ServoEXP( x5r, pinR[idx],3,finalse3,50) );
	
	//left heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( toRad(90), 1,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x5l,3,finalse3) );
	servosL.insert( servosL.end(), new ServoEXP( x5l, pinL[idx],3,finalse3,50) );
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
	finalse3.set( 90, 1,1);
	finalse3.set( 90, 2,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,90) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[0] );
	
	
	//left hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( -h2hlY, 5,1);
	finalse3.set( 90, 1,1);
	finalse3.set( -90, 2,1);
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
	finalse3.set( 90, 1,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,50) );
	idx++;
	tW2L.insert( tW2L.end(), expM(finalse3)*tW2L[4] );
	
	//left heel low to foot : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2fX, 4,1);
	servosL.insert( servosL.end(), new Servo(pinL[idx],3,finalse3,90) );
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





