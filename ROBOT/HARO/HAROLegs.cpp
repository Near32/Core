#include "HAROLegs.h"

HAROLegs::HAROLegs()
{
	pe = new PE();
	constructFrames();
	
	nbrR = servosR.size();
	nbrL = servosL.size();
	
	velocitiesR = Mat<float>((float)0,nbrR,1);
	velocitiesL = Mat<float>((float)0,nbrL,1);
	
	continuer = true;
}

HAROLegs::~HAROLegs()
{
	for(int i=servosR.size();i--;)	delete servosR[i];
	for(int i=servosL.size();i--;)	delete servosL[i];
	
	delete pe;
}
	
void HAROLegs::loop()
{
	clock_t timer = clock();
	float dt;
	
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
void HAROLegs::constructFrames()
{
	Mat<float> finalse3((float)0,6,1);
	
	int pinR[5] = {21,22,23,24,25};
	int pinL[5] = {5,26,27,28,29};
	int idx=0;
	
	//in relative manner : from frame n-1 to frame n.
	tW2R.insert( tW2R.end(), expM(finalse3) );
	tW2L.insert( tW2L.end(), expM(finalse3) );
		
	float W2hY = -0.1f;//-10 centimeters.
	float h2hlY = 0.075f;	//7.5 centimeters.
	float hl2kX = 0.1f;
	float k2hlX = 0.05f;
	float hl2hllX = 0.075f;
	float hl2fX = 0.075f;
	
	
	//TODO : inversion in Servomotors with respect to Servo class :
	// add in the class + check in reality ...
	//right hip : nuz, 80 at least offset
	finalse3.set( W2hY, 5,1);
	finalse3.set( -90, 1,1);
	finalse3.set( 90, 2,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,90) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[0] );
	
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( -90, 1,1);
	finalse3.set( -90, 2,1);
	
	servosR.insert( servosR.end(), new Servo( pinR[idx],3,finalse3,50) );
	idx++;
	Mat<float> temp(expM(finalse3)*tW2R[1]);
	tW2R.insert( tW2R.end(), temp );
	
	
	//right hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[2] );
	
	
	//right knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2hlX, 4,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[3] );
	
	
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2hllX, 4,1);
	finalse3.set( 90, 1,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );
	idx++;
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[4] );
	
	
	//right heel low to foot : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2fX, 4,1);
	servosR.insert( servosR.end(), new Servo(pinR[idx],3,finalse3,50) );	
	tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[5] );
	
	
	//------------------------------------------
	//------------------------------------------
	
	//LEFT LEG :
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
	
}


Mat<float> HAROLegs::getSE3_one2next( int numero, bool right)
{
	Mat<float> r(4,4);
	
	if(numero < tW2R.size() )
	{
		r = (right ? tW2R[numero] : tW2L[numero] );
	}
	
	return r;
}

Mat<float> HAROLegs::getW2FINAL(bool right)
{
	/*
	Mat<float> r( (float)0,4,1 );
	for(int i=1;i<=4;i++)	r.set( 1.0f,i,i);
	
	for(int i=tW2R.size();i--;)		r *= (right? tW2R[i] : tW2L[i]);
	
	return r;
	*/
	return (right? tW2R[tW2R.size()-1] : tW2L[tW2L.size()-1]);
}


void HAROLegs::stop()
{
	continuer = false;
}





