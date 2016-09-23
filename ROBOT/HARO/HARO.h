#ifndef HARO_H
#define HARO_H

#include "HAROLegs.h"
#include "SDL/SDL.h"

#include <thread>
#include <mutex>


class HARO
{
	private :
	HAROLegs* harolegs;
	
	//------------
	SDL_Surface* ecran;
	
	
	public :
	
	HARO();
	~HARO();
	
	void loop();
	
	void init();
	
};

#endif
