#include "HAROEXP.h"

using namespace std;

mutex mutexRES;

#define nbrTraj 1

HAROEXP::HAROEXP()
{
	harolegs = new HAROLegsEXP();
	
	time = clock();
	runtime = 0.0f;
	
	init();
	
	trajectories = new Mat<float>[nbrTraj];
	
	generateTrajectories();
}

HAROEXP::~HAROEXP()
{
	delete harolegs;
	delete trajectories;
	SDL_Quit();
}


void HAROEXP::loop()
{
    thread tLegs( &HAROLegsEXP::loop, std::ref(*harolegs) );
    
    SDL_Event event;
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
    
    this->generateTrajectories();
    
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
                		std::cout << "COMMAND SENT." << std::endl;
                	}
                	break;
                	
                	case SDLK_z:
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandUPVAR() ); 
                	}
                	break;
                	
                	case SDLK_s:
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandDOWNVAR() );
                	}
                	break;
                	
                	case SDLK_q :
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandPREVIOUSVAR() );
                	}
                	break;
                	
                	case SDLK_d :
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandNEXTVAR() );
                	}
                	break;
                	
                	case SDLK_o:
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandUPVAR(SIDEleft) ); 
                	}
                	break;
                	
                	case SDLK_l:
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandDOWNVAR(SIDEleft) );
                	}
                	break;
                	
                	case SDLK_k :
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandPREVIOUSVAR(SIDEleft) );
                	}
                	break;
                	
                	case SDLK_m :
                	{
                		std::cout << "COMMAND SENT." << std::endl;
                		harolegs->addCommand( (ICommand*)new ICommandNEXTVAR(SIDEleft) );
                	}
                	break;
                	
                	case SDLK_t :
                	{
                		this->generateVelocitiesANDPUSH();
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
    SDL_FillRect(ecran,&r, SDL_MapRGB(ecran->format, 10,10,100) );
    
}


void HAROEXP::generateVelocitiesANDPUSH()
{
	float deltaT = 1e0f;
	
	//float currenttime = clock();
	//float dt = (float)(currenttime-time)/CLOCKS_PER_SEC;
	//time = currenttime;
	float dt = 1e-1f;
	//TODO : handle it without debugging...
	runtime += dt;
	
	//index of the current goal of the trajectory :
	int nbrIdxTraj = trajectories[0].getColumn();
	int idxTraj = (runtime/deltaT);
	if(idxTraj > nbrIdxTraj)	
		idxTraj = nbrIdxTraj;
	//we want to ensure that we will not go over the length of the trajectories.
	
	//goal that we want to follow at the given currenttime :
	Mat<float> goal[nbrTraj];
	for(int i=nbrTraj;i--;)	goal[i] = extract( trajectories[i], 1,idxTraj, 3,idxTraj);
	
	//current positions :
	Mat<EXP> r[nbrTraj];
	for(int i=nbrTraj;i--;)	r[i] = ( * (idxTraj2r[i]) ) (harolegs);
	
	//P(ID) Controller on 3D end-effector velocities:
	float p = 1.0f;
	std::vector<Mat<float> >  dx;
	for(int i=nbrTraj;i--;)	dx.insert( dx.begin(), (p*dt)*(goal[i]- EXP2floatM(r[i])) );
	
	//JACOBIANS & INVERSION :
	std::vector<Mat<float> > J;
	for(int i=nbrTraj;i--;)	J.insert( J.begin(), EXP2floatM( harolegs->generateJacobian( r[i] ) ) );
	//TODO : adjust to the trajs... :
	J[0] = extract( J[0], 1,1, 3,5);
	//J[1] = extract( J[1], 1,6, 3,10);
	
	std::vector<Mat<float> > invJ;
	for(int i=nbrTraj;i--;)
	{
		invJ.insert( invJ.begin(), invGJ( transpose(J[i])*J[i] ) );
		invJ[0].afficher();
	}
	
	//COMPUTATION OF DQ :
	std::vector<Mat<float> > dq;
	for(int i=nbrTraj;i--;)
	{
		dq.insert( dq.begin(), invJ[i] * ( transpose(J[i])*dx[i] ) );
		dq[0].afficher();
	}
	
	//PUSHING :
	//TODO : this function assume that the dq are for the all the variables beginning by the ones of the right leg...
	Mat<float> dQ( dq[0] );
	//TODO change this : only works if we have two traj, one for each legs...
	for(int i=nbrTraj;i--;)	dQ = operatorC( dQ, dq[i] );
	
	if( dQ.getLine() < 10)
	{
		dQ = operatorC( dQ, Mat<float>(0.0f, 10-dQ.getLine()+1, 1) );
	}
	
	this->setDq( dq );
	
}

void HAROEXP::generateTrajectories()
{
	
	idxTraj2r[0] = &HAROLegsEXP::getRkneer;
	idxTraj2r[1] = &HAROLegsEXP::getRkneel;
	
	//initializations :
	for(int i=nbrTraj;i--;)	trajectories[i] = EXP2floatM( ( * (idxTraj2r[i]) ) (harolegs) );
	//on the current position of the robot...
	
	Mat<float> add(0.0f,3,1);
	add.set( 5e-2f, 1,1);
	//5 centimeters on the X forward axis.
	
	//final desired position :
	for(int i=nbrTraj;i--;)	trajectories[i] = operatorL( trajectories[i], EXP2floatM( ( * (idxTraj2r[i]) ) (harolegs) )  + add);
	
	
	//TODO
	
}




