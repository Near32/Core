#include "HAROEXP.h"

using namespace std;

//mutex velocityMutex;


HAROEXP::HAROEXP()
{
	harolegs = new HAROLegsEXP();
	
	init();
}

HAROEXP::~HAROEXP()
{
	delete harolegs;
	SDL_Quit();
}


void HAROEXP::loop()
{
    thread tLegs( &HAROLegsEXP::loop, std::ref(*harolegs) );
    
    SDL_Event event;
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
    
    bool continuer = true;
    while(continuer)
    {
    	start_time = SDL_GetTicks();
		
		//traitement/récupération des events.
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

		
        //--------------------------------------
        //--------------------------------------
        //--------------------------------------
        
        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
        
        //--------------------------------------
        //--------------------------------------
        //--------------------------------------
                        
        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 20)
        {
            SDL_Delay(20 - ellapsed_time);
        }
    }
    
    harolegs->stop();
    
    if(tLegs.joinable())
    	tLegs.join();
}

void HAROEXP::init()
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("HARO", NULL);
	ecran = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo ();
   
	int systemX = videoInfo->current_w/4 ;
	int systemY = videoInfo->current_h/4 ;
	Uint8 bpp = videoInfo->vfmt->BitsPerPixel ;
   
    //Set up screen
    ecran = SDL_SetVideoMode( systemX, systemY, bpp, SDL_SWSURFACE );
    if (!ecran)
    {
        printf("SDL_SetVideoMode failed\n");
        exit(0);
    }
   
    SDL_Rect r = {0,0,320,240};
    SDL_FillRect(ecran,&r, SDL_MapRGB(ecran->format, 200,200,0) );
    
}





