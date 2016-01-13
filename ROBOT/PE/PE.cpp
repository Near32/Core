#include "PE.h"

//#define debug
//#define debuglvl1
//#define debuglvl2
#define debuglvl3

#define testYAW
#define NBRCALIB 100

PE::PE(float dt_)
{

	IMU = new MPU6050();
	biasGy = new Mat<float>(0.0f,3,1);
	Gravity = Mat<float>(0.0f,3,1);
	Gravity.set( -9.81f,3,1);
	
	//------------------
	//    EKF ACC GYRO
	//------------------
	int nbrstate = 6;
	int nbrcontrol = 3;	
	int nbrobs = 2;
#ifdef testYAW
	nbrobs = 3;
#endif
	dt = dt_;
	
	stdnoise = 3e-1;
	stdnoise_obs = 1e-5;

	bool ext = false;
	bool filteron = true;
	bool noise = false;

	Xg = new Mat<float>((float)0,nbrstate,1);  
	XgCovar = new Mat<float>((float)0,nbrstate,nbrstate);  
	instanceEEKF = new EEKF<float>(nbrstate,nbrcontrol,nbrobs,dt,stdnoise,*Xg,ext,filteron,noise);

	Mat<float> Ag((float)0,nbrstate,nbrstate);
	for(int i=1;i<=nbrstate;i++)	Ag.set((float)1,i,i);
	//unstable if the velocity is propagated...
	for(int i=1;i<=nbrstate/2;i++)	Ag.set((float)-dt,i,nbrstate/2+i);
	//Ag.afficher();
	instanceEEKF->initA(Ag);
	
	Mat<float> Cg((float)0,nbrobs,nbrstate);
	for(int i=1;i<=nbrobs;i++)	Cg.set((float)1,i,i);
	//Cg.afficher();
	instanceEEKF->initC(Cg);
			
	Mat<float> Bg((float)0,nbrstate,nbrcontrol);
	for(int i=1;i<=nbrcontrol;i++)	Bg.set((float)1,i,i);
	//Bg.afficher();
	instanceEEKF->initB(Bg);
	
	
	Mat<float> Qg(0.0f,nbrstate,nbrstate);
	for(int i=1;i<=nbrstate/2;i++)	Qg.set( stdnoise, i,i);
	//bias :
	for(int i=nbrstate/2+1;i<=nbrstate;i++)	Qg.set( stdnoise*1e-5f, i,i);
	//Q.afficher();
	instanceEEKF->initQ(Qg);
	
	Mat<float> Rg(0.0f,nbrobs,nbrobs);
	for(int i=1;i<=nbrobs;i++)	Rg.set( stdnoise_obs, i,i);
	//R.afficher();
	instanceEEKF->initR(Rg);

	//-----------------------------------------
	//-----------------------------------------

		//------------------
	//    EKF ACC GYRO
	//------------------
	int nbrstateX = 6;
	int nbrcontrolX = 3;	
	int nbrobsX = 3;
	
	stdnoiseX = 1e-6;
	stdnoise_obsX = 1e1f;
	X = new Mat<float>((float)0,nbrstateX,1);  
	XCovar = new Mat<float>((float)0,nbrstateX,nbrstateX);  
	instanceEEKFPose = new EEKF<float>(nbrstateX,nbrcontrolX,nbrobsX,dt,stdnoiseX,*X,ext,filteron,noise);

	Mat<float> A((float)0,nbrstateX,nbrstateX);
	for(int i=1;i<=nbrstateX;i++)	A.set((float)1,i,i);
	//unstable if the velocity is propagated...
	for(int i=1;i<=nbrstateX/2;i++)	A.set((float)dt,i,nbrstateX/2+i);
	A.afficher();
	instanceEEKFPose->initA(A);
	
	Mat<float> C((float)0,nbrobsX,nbrstateX);
	for(int i=1;i<=nbrobsX;i++)	C.set((float)1,i,nbrstateX/2+i);
	C.afficher();
	instanceEEKFPose->initC(C);
			
	Mat<float> B((float)0,nbrstateX,nbrcontrolX);
	for(int i=1;i<=nbrcontrolX;i++)	B.set((float)dt,nbrstateX/2+i,i);
	B.afficher();
	instanceEEKFPose->initB(B);
	
	
	Mat<float> Q(0.0f,nbrstateX,nbrstateX);
	for(int i=1;i<=nbrstateX/2;i++)	Q.set( stdnoiseX, i,i);
	//bias :
	for(int i=nbrstateX/2+1;i<=nbrstateX;i++)	Q.set( stdnoiseX, i,i);
	//Q.afficher();
	instanceEEKFPose->initQ(Q);
	
	Mat<float> R(0.0f,nbrobsX,nbrobsX);
	for(int i=1;i<=nbrobsX;i++)	R.set( stdnoise_obsX, i,i);
	//R.afficher();
	instanceEEKFPose->initR(R);  
	
}

PE::~PE()
{
	delete IMU;
	delete biasGy;
	
	delete Xg;
	delete XgCovar;
	delete instanceEEKF;
	
	delete X;
	delete XCovar;
	delete instanceEEKFPose;
	
}


void PE::initialize()
{
	cout << "MPU6050 6-axis IMU - initialization... " << endl;
	I2Cdev::initialize();
	
	IMU->initialize();
	
	initializeIMU();
}

void PE::initializeIMU()
{
	//----------------
	//    ACC GYRO
	float Gacc = 9.81f;
	float rangeAcc = 2.0f*Gacc;
	float precisionAcc = pow(2.0f, 16-1);
	float rangeGyro = 1.0f*250.0f;
	float precisionGyro = pow(2.0f, 16-1);
	  
	sgyro = rangeGyro/precisionGyro;
	sacc = rangeAcc/precisionAcc;
	
	int16_t gyx = 0;
	int16_t gyy = 0;
	int16_t gyz = 0;
	float sgyx,sgyy,sgyz;

	*biasGy = Mat<float>(0.0f,3,1);
	
#ifdef debug
cout << "CALIBRATION : ... " << endl;
#endif	
	for(int i=NBRCALIB;i--;)
	{
		IMU->getRotation(&gyx,&gyy,&gyz);

		sgyx = gyx*sgyro;
		sgyy = gyy*sgyro;
		sgyz = gyz*sgyro;
		
		biasGy->set( biasGy->get(1,1)+sgyx, 1,1);
		biasGy->set( biasGy->get(2,1)+sgyy, 2,1);
		biasGy->set( biasGy->get(3,1)+sgyz, 3,1);
	}
	
	*biasGy *= (float)(1.0f/NBRCALIB);

	
#ifdef debug
cout << "CALIBRATION : DONE. " << endl;
biasGy->afficher();
#endif	
}


void PE::callback(float dt_)
{
	dt = dt_;
	
	//clock_t timer = clock();
	
	//--------------------------
	//		Retrieve values :
	//--------------------------
	int16_t acx,acy,acz;
	int16_t gyx,gyy,gyz;
	
	IMU->getMotion6(&acx,&acy,&acz,&gyx,&gyy,&gyz);
	
	//rescaling :
	acx*= -sacc;acy*= -sacc;acz*= -sacc;
	gyx*=sgyro;gyy*=sgyro;gyz*=sgyro;
	//--------------------------
	
	
	//--------------------------
	//		SLAM-Gravity field
	//-------------------------
	Mat<float> rotAG2Gl(transpose( Euler2Rot( angleX,angleY,angleZ) ) );
	Mat<float> acc(3,1);
	acc.set( acx,1,1);
	acc.set( acy,2,1);
	acc.set( acz,3,1);
	
#ifdef debuglvl3	
	cout << "Felt Acceleration by the IMU in its local frame : " << endl;
	acc.afficher();	
#endif	
	acc = rotAG2Gl * acc;
	//acc.afficher();
	//instantaneous felt accelerations in the world frame.
	
	//---------------------------
	//EEKF - Gravity field:
	//---------------------------	
	Mat<float> input(3,1);
	Mat<float> measure(2,1);
#ifdef testYAW
	measure = Mat<float>(3,1);
#endif	

	input.set( (gyx-biasGy->get(1,1)) ,1,1);
	input.set( (gyy-biasGy->get(2,1)) ,2,1);
	input.set( (gyz-biasGy->get(3,1)) ,3,1);
		
	float Rx = sqrt( (float)acx*acx + (float)acz*acz);
	float accRoll = atan2( acy, Rx);
	//float accPitch = atan2( acy*sacc*cos(accRoll), acz*sacc);
	float Ry = sqrt( (float)acy*acy + (float)acz*acz);
	float accPitch = atan2( acx, Ry);

	measure.set( accRoll, 1,1);
	measure.set( accPitch, 2,1);
#ifdef testYAW
	float accYaw = angleZ+dt*gyz;
	measure.set( accYaw,3,1);
#endif	

#ifdef debuglvl1
	cout << "measure & input : in radians" << endl;
	measure.afficher();
	input.afficher();
#endif	
	instanceEEKF->setCommand(input);
	instanceEEKF->measurement_Callback(measure);
	instanceEEKF->state_Callback();
	*Xg = instanceEEKF->getX();
	Mat<float> Angles( *Xg);
	
	while(Angles.get(1,1) > PI)
	{
	 Angles.set(Angles.get(1,1)-2*PI,1,1); 
	}
	while(Angles.get(2,1) > PI)
	{
	 Angles.set(Angles.get(2,1)-2*PI,2,1); 
	}
	while(Angles.get(3,1) > PI)
	{
	 Angles.set(Angles.get(3,1)-2*PI,3,1); 
	}

	while(Angles.get(1,1) < -PI)
	{
	 Angles.set(Angles.get(1,1)+2*PI,1,1); 
	}
	while(Angles.get(2,1) < -PI)
	{
	 Angles.set(Angles.get(2,1)+2*PI,2,1); 
	}
	while(Angles.get(3,1) < -PI)
	{
	 Angles.set(Angles.get(3,1)+2*PI,3,1); 
	}

	instanceEEKF->setX(&Angles);

	// Kalman Filter ACCGYRO: in radians
	angleX = Angles.get(1,1);			
	angleY = Angles.get(2,1);
	angleZ = Angles.get(3,1);

	
	cout << " roll : " << angleX*180/3.14156 << " ; pitch : " << angleY*180/3.14156 << " ; yaw : " << angleZ*180/3.14156 << endl;
	
	
	//--------------------------
	//		SLAM-Pose
	//-------------------------
	Mat<float> inputX(acc-Gravity);
	Mat<float> measureX(extract(*X,4,1, 6,1) );
	
#ifdef debuglvl2
	cout << "measure & input : in meters" << endl;
	measureX.afficher();
	inputX.afficher();
#endif		
	instanceEEKFPose->setCommand(inputX);
	instanceEEKFPose->measurement_Callback(measureX);
	instanceEEKFPose->state_Callback();
	*X = instanceEEKFPose->getX();
	
	cout << "Pose : " << endl;
	transpose(*X).afficher();
	
}



/* Returns the corresponding SO(3) matrix from Euler angles*/
Mat<float> PE::Euler2Rot( const float& roll, const float& pitch, const float& yaw)
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
