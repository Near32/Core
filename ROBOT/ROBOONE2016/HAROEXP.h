#ifndef HAROEXP_H
#define HAROEXP_H

#include "HAROLegsEXP.h"
#include "SDL/SDL.h"

#include <map>
#include <thread>
#include <mutex>

#include "ICommand.h"
#include "../PIDController/PIDControllerM.h"

typedef Mat<EXP> (RobotPart::*Function)(void) const;







class ActionParams
{
	public :
	
	RobotPart* ptrRobotPart;
	int nbrTraj;
	std::vector<std::vector<Mat<float> > > GoalTraj;
	std::vector<Function> FunctionTrajThread2r;
	
	std::vector<Mat<EXP> > Traj2J;							//EXP of the Jacobian matrix related to the interesting variables.
	bool JacobianInitialized;
	
	
	
	
	//-----------------------------------------------------------
	//-----------------------------------------------------------
	//-----------------------------------------------------------
	//-----------------------------------------------------------
	
	
	ActionParams()
	{
		ptrRobotPart = NULL;
		JacobianInitialized = false;
		nbrTraj = 0;
	}
	
	ActionParams( RobotPart* ptrrobotpart, const std::vector<std::vector<Mat<float> > >& goaltraj, const std::vector<Function>& functiontrajthread2r) : ptrRobotPart(ptrrobotpart), GoalTraj(goaltraj), FunctionTrajThread2r( functiontrajthread2r)
	{
		JacobianInitialized = false;
		nbrTraj = GoalTraj.size();
	}
	
	~ActionParams()
	{
	
	}
	
	void JacobianInitialization()
	{
		if(!JacobianInitialized)
		{
			JacobianInitialized = true;
			
			//-------------------------------------
			//	Generations of the Jacobians EXP :
			//-------------------------------------
			clock_t timeJacobian = clock();
			Traj2J.clear();
			for(int i=nbrTraj;i--;)
			{
				Traj2J.insert( Traj2J.begin(), ptrRobotPart->generateJacobian( (ptrRobotPart->*(FunctionTrajThread2r[i]) )() ) );
				evaluate( Traj2J[i] );
			}
	
			std::cout << "ActionParams :: The JACOBIAN DERIVATION took : " << (float)(clock()-timeJacobian)/CLOCKS_PER_SEC << " seconds." << std::endl;
			//-------------------------------------
			//-------------------------------------
		}
	}
};




class HAROEXP
{
	private :
	HAROLegsEXP* harolegs;
	
	//------------
	SDL_Surface* ecran;
	
	Mat<float>* trajectories;
	std::map<int,Function> idxTraj2r;
	std::map<int,Mat<EXP> > idxTraj2J;
	bool trajREADY;
	PIDControllerM<float>* pid;
	
	//ActionParams used in the multithreaded approach :
	ActionParams moveRFootForward;	
	
	public :
	
	clock_t time;
	float runtime;
	
	HAROEXP();
	~HAROEXP();
	
	void loop();
	
	void init();
	
	void generateVelocitiesANDPUSH();
	void generateTrajectories();
	void generateTrajectoriesBASSIN();
	
	
	//Thread used to request the robot to perform an action :
	void threadAction( ActionParams& ap);
	//Creations of the ActionParam required :
	void GenerateActionParams();
	
};








#endif
