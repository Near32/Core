#ifndef HAROLEGSEXP_H
#define HAROLEGSEXP_H

#include "../ServoEXP/ServoEXP.h"
#include <vector>
#include "../PE/PE.h"

#include "ICommand.h"
#include <memory>
#include <mutex>

class RobotPart
{
	public :
	
	RobotPart()
	{
	
	}	
	
	~RobotPart()
	{
	
	}
	
	virtual Mat<EXP<float> > generateJacobian(const Mat<EXP<float> >& v)=0;
	virtual void setDq( const Mat<float>& dq)=0;
	
	//-----------------------------------------
	virtual Mat<EXP<float> > getRbassinR()	const	{}
	//Rbassin in Rfootr
	virtual Mat<EXP<float> > getRbassinL()	const	{}
	//Rbassin in Rfootl
	virtual Mat<EXP<float> > getRfootr()	const	{}
	//Rfootr in Rbassin
	virtual Mat<EXP<float> > getRfootl()	const	{}
	//Rfootr in Rbassin
	virtual Mat<EXP<float> > getRkneer()	const	{}
	//Rkneer in Rbassin
	virtual Mat<EXP<float> > getRkneel()	const	{}
	//Rkneer in Rbassin
};


class HAROLegsEXP : public RobotPart
{
	private :
	
	bool continuer;
	
	
	EXP<float> x1r;
	EXP<float> x2r;
	EXP<float> x3r;
	EXP<float> x4r;
	EXP<float> x5r;
	EXP<float> ankle2footr;
	
	EXP<float> x1l;
	EXP<float> x2l;
	EXP<float> x3l;
	EXP<float> x4l;
	EXP<float> x5l;
	EXP<float> ankle2footl;
	
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
	
	Mat<EXP<float> > Tr;
	Mat<EXP<float> > Tl;
	Mat<EXP<float> > rbr;
	Mat<EXP<float> > rbl;
	Mat<EXP<float> > rrb;
	Mat<EXP<float> > rlb;
	Mat<EXP<float> > rarb;
	Mat<EXP<float> > ralb;
	
	
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
	
	Mat<EXP<float> > getSE3_one2next( int numero, bool right);
	Mat<EXP<float> > getW2FINAL(bool right);
	virtual void setDq( const Mat<float>& dq)	override;
	
	void stop();
	
	void addCommand( ICommand* command);
	
	
	//-----------------------------------------
	virtual Mat<EXP<float> > getRbassinR()	const;
	//Rbassin in Rfootr
	virtual Mat<EXP<float> > getRbassinL()	const;
	//Rbassin in Rfootl
	virtual Mat<EXP<float> > getRfootr()	const;
	//Rfootr in Rbassin
	virtual Mat<EXP<float> > getRfootl()	const;
	//Rfootr in Rbassin
	virtual Mat<EXP<float> > getRkneer()	const;
	//Rkneer in Rbassin
	virtual Mat<EXP<float> > getRkneel()	const;
	//Rkneer in Rbassin
	
	virtual Mat<EXP<float> > generateJacobian(const Mat<EXP<float> >& v)	override;
};

#endif
