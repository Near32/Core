#ifndef FRAME_H
#define FRAME_H

//#include "../Mat/Mat.h"
#include "../../Core1/MAT/Mat2.h"
#include "../MVG/MVG.h"

class Frame
{
	protected :
	Mat<float> se3;
	
	int idxParameter;
	/* idx € [1..6] == [nux...tz]*/
	
	Mat<float> Rx;
	Mat<float> Ry;
	Mat<float> Rz;
	Mat<float> t;
	
	Mat<float> SE3;
	bool hasChanged;
	
	public :
	
	Frame();
	Frame(int idxParameter_, const Mat<float>& se3Init);
	~Frame();
	
	//-----------------------------
	Mat<float> getSE3();
	Mat<float> getT();
	Mat<float> getR();
	
	void setIDX(int idx);
	int getIDX();
	
	void setParam(float param);
	float getParam();
	//-----------------------------
	static Mat<float> Euler2Rot(const float& roll, const float& pitch, const float& yaw);
	
};
#endif
