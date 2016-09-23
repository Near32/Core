#ifndef HAROLEGS_H
#define HAROLEGS_H

#include "../Servo/Servo.h"
#include <vector>
#include "../PE/PE.h"

class HAROLegs
{
	private :
	
	bool continuer;
	
	std::vector<Servo*> servosR;
	std::vector<Servo*> servosL;
	std::vector<Mat<float> > tW2R;
	std::vector<Mat<float> > tW2L;
	
	Mat<float> velocitiesR;
	Mat<float> velocitiesL;
	
	int nbrR;
	int nbrL;
	
	PE* pe;
	
	public :
	
	HAROLegs();
	~HAROLegs();
	
	void loop();
	
	
	//------------------------------
	void constructFrames();
	
	Mat<float> getSE3_one2next( int numero, bool right);
	Mat<float> getW2FINAL(bool right);
	
	
	void stop();
	
};

#endif
