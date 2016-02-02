#ifndef HAROEXP_H
#define HAROEXP_H

#include "HAROLegsEXP.h"
#include "SDL/SDL.h"

#include <map>
#include <thread>
#include <mutex>

#include "ICommand.h"

typedef Mat<EXP> (*Function)(void);

class HAROEXP
{
	private :
	HAROLegsEXP* harolegs;
	
	//------------
	SDL_Surface* ecran;
	
	Mat<float>* trajectories;
	std::map<int,HAROLegsEXP::Function> idxTraj2r;
	
	
	public :
	
	clock_t time;
	float runtime;
	
	HAROEXP();
	~HAROEXP();
	
	void loop();
	
	void init();
	
	void generateVelocities();
	void generateTrajectories();
	
};

#endif
