#ifndef HAROLEGSEXP_H
#define HAROLEGSEXP_H

#include "../ServoEXP/ServoEXP.h"
#include <vector>
#include "../PE/PE.h"

class HAROLegsEXP
{
	private :
	
	bool continuer;
	
	
	VAR x1r;
	VAR x2r;
	VAR x3r;
	VAR x4r;
	VAR x5r;
	VAR ankle2footr;
	
	VAR x1l;
	VAR x2l;
	VAR x3l;
	VAR x4l;
	VAR x5l;
	VAR ankle2footl;
	
	std::vector<ServoEXP*> servosR;
	std::vector<ServoEXP*> servosL;
	std::vector<FrameEXP*> > tW2R;
	std::vector<FrameEXP*> > tW2L;
	
	Mat<float> velocitiesR;
	Mat<float> velocitiesL;
	
	int nbrR;
	int nbrL;
	
	PE* pe;
	
	public :
	
	HAROLegsEXP();
	~HAROLegsEXP();
	
	void loop();
	
	
	//------------------------------
	void constructFrames();
	
	Mat<EXP> getSE3_one2next( int numero, bool right);
	Mat<EXP> getW2FINAL(bool right);
	
	
	void stop();
	
};

#endif
