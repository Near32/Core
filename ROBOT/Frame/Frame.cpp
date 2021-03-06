#include "Frame.h"

Frame::Frame() : idxParameter(1), se3(Mat<float>((float)0,6,1))
{
	Rx = Euler2Rot(se3.get(1,1),0.0f,0.0f);
	Ry = Euler2Rot(0.0f, se3.get(2,1), 0.0f);
	Rz = Euler2Rot(0.0f, 0.0f, se3.get(3,1));
	t = extract(&se3,4,1,6,1);
	
	hasChanged = false;
	
	SE3 = expM(se3);
	//SE3 = operatorL( operatorC( Rz*Ry*Rx, Mat<float>((float)0,1,3) ), operatorC(t,Mat<float>((float)1,1,1) ) );
}
	
Frame::Frame(int idxParameter_, const Mat<float>& se3Init) : idxParameter(idxParameter_), se3(se3Init)
{
	Rx = Euler2Rot(se3.get(1,1),0.0f,0.0f);
	Ry = Euler2Rot(0.0f, se3.get(2,1), 0.0f);
	Rz = Euler2Rot(0.0f, 0.0f, se3.get(3,1));
	t = extract(&se3,4,1,6,1);
	
	hasChanged = false;
	
	SE3 = expM(se3);
	//SE3 = operatorL( operatorC( Rz*Ry*Rx, Mat<float>((float)0,1,3) ), operatorC(t,Mat<float>((float)1,1,1) ) );
}
	
Frame::~Frame()
{

}
	
	//-----------------------------
Mat<float> Frame::getSE3()
{
	if(hasChanged)
	{
		hasChanged = false;
		SE3 = expM(se3);
	}
	return SE3;
}

Mat<float> Frame::getT()
{
	return t;
}

Mat<float> Frame::getR()
{
	return Rz*Ry*Rx;
}
	
void Frame::setIDX(int idx)
{
	if(idx >= 1 && idx <= 6)
	{
		idxParameter = idx;
	}
	else
	{
		cerr << "FRAME : setIdx : idx not correct." << endl;
	}
}

int Frame::getIDX()
{
	return idxParameter;
}

	
void Frame::setParam(float param)
{
	se3.set( param, idxParameter, 1);
	hasChanged = true;
}

float Frame::getParam()
{
	return se3.get( idxParameter, 1);
}

	//-----------------------------
Mat<float> Frame::Euler2Rot(const float& roll, const float& pitch, const float& yaw)
{
	Mat<float> rx(0.0f, 3,3);
	rx.set( cos(roll), 2,2);
	rx.set( cos(roll), 3,3);
	rx.set( -sin(roll), 2,3);
	rx.set( sin(roll), 3,2);
	rx.set( 1.0f, 1,1);
	
	Mat<float> ry(0.0f, 3,3);
	ry.set( cos(pitch), 1,1);
	ry.set( cos(pitch), 3,3);
	ry.set( -sin(pitch), 3,1);
	ry.set( sin(pitch), 1,3);
	ry.set( 1.0f, 2,2);
	
	Mat<float> rz(0.0f, 3,3);
	rz.set( cos(yaw), 1,1);
	rz.set( cos(yaw), 2,2);
	rz.set( -sin(yaw), 1,2);
	rz.set( sin(yaw), 2,1);
	rz.set( 1.0f, 3,3);
	
	return rz*(ry*rx);
}
