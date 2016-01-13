#include <iostream>
#include "../Servo/Servo.h"

#include "SDL/SDL.h"

int main( int argc, char* args[] )
{
    SDL_Surface* screen = NULL;
    //Start SDL
    SDL_Init( SDL_INIT_VIDEO );

   const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo ();
   
   int systemX = videoInfo->current_w/4 ;
   int systemY = videoInfo->current_h/4 ;
   Uint8 bpp = videoInfo->vfmt->BitsPerPixel ;
   
    //Set up screen
    screen = SDL_SetVideoMode( systemX, systemY, bpp, SDL_SWSURFACE );
    if (!screen)
    {
        printf("SDL_SetVideoMode failed\n");
        return 0;
    }
   
    SDL_Rect r = {0,0,320,240};
    SDL_FillRect(screen,&r, SDL_MapRGB(screen->format, 200,200,0) );

	SDL_Event event;
	Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
    
    bool continuer = true;
    while(continuer)
    {
    
    	while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                continuer = false;
                break;
                
                case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
                	continuer = false;
                	break;
                	
                	case SDLK_SPACE:
                	{
                		
                	}
                	break;
                	
                	default:
                	{
                		
                	}
                	break;
                }
                break;
                
                case SDL_MOUSEMOTION:
                {
                	
                }
                break;
                
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                {
                
                }
                break;
            }
        }
        
        //--------------------------------
    
		//Update Screen
		SDL_Flip( screen );
		//Pause
		
		current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
        
        
        if (ellapsed_time < 20)
        {
            SDL_Delay(20 - ellapsed_time);
        }
	
	}

    //Quit SDL
    SDL_Quit();

    return 0;
}
