#ifndef HAROEXP_H
#define HAROEXP_H

#include "HAROLegsEXP.h"
#include "SDL/SDL.h"

#include <map>
#include <thread>
#include <mutex>

#include "ICommand.h"

typedef Mat<EXP> (HAROLegsEXP::*Function)(void) const;

class HAROEXP
{
	private :
	HAROLegsEXP* harolegs;
	
	//------------
	SDL_Surface* ecran;
	
	Mat<float>* trajectories;
	std::map<int,Function> idxTraj2r;
	std::map<int,Mat<EXP> > idxTraj2J;
	
	public :
	
	clock_t time;
	float runtime;
	
	HAROEXP();
	~HAROEXP();
	
	void loop();
	
	void init();
	
	void generateVelocitiesANDPUSH();
	void generateTrajectories();
	
};

#endif
