#ifndef HAROLEGSEXP_H
#define HAROLEGSEXP_H

#include "../ServoEXP/ServoEXP.h"
#include <vector>
#include "../PE/PE.h"

#include "ICommand.h"
#include <memory>
#include <mutex>


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
	std::vector<FrameEXP*> tW2R;
	std::vector<FrameEXP*> tW2L;
	
	int nbrR;
	int nbrL;
	
	PE* pe;
	
	std::vector<std::unique_ptr<ICommand> > listCommand;
	int varnumberR;
	int varnumberL;
	
	Mat<EXP> Tr;
	Mat<EXP> Tl;
	Mat<EXP> rbr;
	Mat<EXP> rbl;
	Mat<EXP> rrb;
	Mat<EXP> rlb;
	Mat<EXP> rarb;
	Mat<EXP> ralb;
	
	
	public :
	
	//------------------------------------------
	//------------------------------------------
	//------------------------------------------
	
	Mat<float> velocitiesR;
	Mat<float> velocitiesL;
	
	//-----------------------------------------
	//------------------------------------------
	//------------------------------------------
	
	HAROLegsEXP();
	~HAROLegsEXP();
	
	void loop();
	
	
	//------------------------------
	void constructFrames();
	
	Mat<EXP> getSE3_one2next( int numero, bool right);
	Mat<EXP> getW2FINAL(bool right);
	void setDq( const Mat<float>& dq);
	
	void stop();
	
	void addCommand( ICommand* command);
	
	
	//-----------------------------------------
	Mat<EXP> getRbassinR()	const;
	//Rbassin in Rfootr
	Mat<EXP> getRbassinL()	const;
	//Rbassin in Rfootl
	Mat<EXP> getRfootr()	const;
	//Rfootr in Rbassin
	Mat<EXP> getRfootl()	const;
	//Rfootr in Rbassin
	Mat<EXP> getRkneer()	const;
	//Rkneer in Rbassin
	Mat<EXP> getRkneel()	const;
	//Rkneer in Rbassin
	
	Mat<EXP> generateJacobian(const Mat<EXP>& v)	const;
};

#endif
