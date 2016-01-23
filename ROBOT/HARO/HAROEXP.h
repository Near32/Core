#ifndef HAROEXP_H
#define HAROEXP_H

#include "HAROLegsEXP.h"
#include "SDL/SDL.h"

#include <thread>
#include <mutex>


class HAROEXP
{
	private :
	HAROLegsEXP* harolegs;
	
	//------------
	SDL_Surface* ecran;
	
	
	public :
	
	HAROEXP();
	~HAROEXP();
	
	void loop();
	
	void init();
	
};

#endif
