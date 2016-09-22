#include "HAROEXP.h"

using namespace std;

mutex mutexRES;
mutex mutexKin;

int nbrTraj=2;

HAROEXP::HAROEXP()
{
	harolegs = new HAROLegsEXP();
	
	time = clock();
	runtime = 0.0f;
	
	init();
	
	trajectories = new Mat<float>[nbrTraj];
	pid = new PIDControllerM<float>[nbrTraj];
	trajREADY = false;
	
	//generateTrajectories();
	//GenerateActionParams();
}

HAROEXP::~HAROEXP()
{
	delete harolegs;
	
	delete[] trajectories;
	delete[] pid;
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
                	
                	case SDLK_v :
                	{
                		//this->generateVelocitiesANDPUSH();
                		if(trajREADY)
                		{
                			trajREADY = false;
                		}
                		else
                		{
                			trajREADY = true;
                		}
                	}
                	break;
                	
                	case SDLK_t:
                	{
                		this->generateTrajectories();
                		//this->generateTrajectoriesBASSIN();
                		//trajREADY = true;
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

		if(trajREADY)
		{
			this->generateVelocitiesANDPUSH();
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
	clock_t time = clock();
	
	float deltaT = 5e0f;
	
	//float currenttime = clock();
	//float dt = (float)(currenttime-time)/CLOCKS_PER_SEC;
	//time = currenttime;
	float dt = 1e-1f;
	//TODO : handle it without debugging...
	runtime += dt;
	
	//index of the current goal of the trajectory :
	int nbrIdxTraj = trajectories[0].getColumn();
	int idxTraj = (runtime/deltaT)+1;
	std::cout << " NBR IDX TRAJ before regul = " << idxTraj << std::endl;
	if(idxTraj > nbrIdxTraj)	
		idxTraj = nbrIdxTraj;
		
	std::cout << " NBR IDX TRAJ = " << idxTraj << std::endl;
	//we want to ensure that we will not go over the length of the trajectories.
	
	//current positions :
	std::vector<Mat<EXP> > r;
	for(int i=nbrTraj;i--;)	r.insert( r.begin(), extract( (harolegs->*(idxTraj2r[i])) (), 1,1, 3,1) );
	
	//goal that we want to follow at the given currenttime :
	std::vector<Mat<float> > goal;
	for(int i=nbrTraj;i--;)	
	{
		goal.insert( goal.begin(), extract( trajectories[i], 1,idxTraj, 3,idxTraj) );
		std::cout << "The Current goal/position are : " << std::endl;
		transpose(goal[0]).afficher();
		transpose( EXP2floatM( r[i]) ).afficher();
	}
	
	
	//P(ID) Controller on 3D end-effector velocities:
	float p = 1.0f;
	float i = 0.5f;
	float d = 0.0f;
	std::vector<Mat<float> >  dx;
	for(int i=nbrTraj;i--;)
	{
		pid[i].set( p,i,d);
		pid[i].setConsigne( goal[i] );
		//dx.insert( dx.begin(), ((1.0f)*p*dt)*(goal[i]- EXP2floatM( r[i])) );
		dx.insert( dx.begin(), pid[i].update(EXP2floatM( r[i]), dt) );
		std::cout << "The Current PID dx is : " << std::endl;
		transpose(dx[0]).afficher();
	}
	
	std::cout << "The INIT took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	time = clock();
	
	//JACOBIANS & INVERSION :
	std::vector<Mat<float> > J;
	for(int i=nbrTraj;i--;)	J.insert( J.begin(), EXP2floatM( idxTraj2J[i] ) );
	//for(int i=nbrTraj;i--;)	J.insert( J.begin(), extract( EXP2floatM( harolegs->generateJacobian( r[i] ) ), 1,1, 3,1) );
	//TODO : adjust to the trajs... :
	//J[0] = extract( J[0], 1,1, 3,5);
	//J[1] = extract( J[1], 1,6, 3,10);
	
	std::vector<Mat<float> > invJ;
	for(int i=nbrTraj;i--;)
	{
		invJ.insert( invJ.begin(), invGJ( transpose(J[i])*J[i] ) );
		//invJ[0].afficher();
	}
	
	std::cout << "The JACOBIAN INVERSION took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	time = clock();
	
	//COMPUTATION OF DQ :
	std::vector<Mat<float> > dq;
	for(int i=nbrTraj;i--;)
	{
		dq.insert( dq.begin(), invJ[i] * ( transpose(J[i])*dx[i] ) );
		//dq[0].afficher();
	}
	
	//PUSHING :
	//TODO : this function assume that the dq are for the all the variables beginning by the ones of the right leg...
	Mat<float> dQ( dq[0] );
	//TODO change this : only works if we have two traj, one for each legs...
	for(int i=1;i<nbrTraj;i++)	dQ = operatorC( dQ, dq[i] );
	
	if( dQ.getLine() < 10)
	{
		dQ = operatorC( dQ, Mat<float>(0.0f, 10-dQ.getLine()+1, 1) );
	}
	
	harolegs->setDq( dQ );
	dQ.afficher();
	
	std::cout << "The PUSHING took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
}



void HAROEXP::threadAction( ActionParams& ap )
{
	clock_t time = clock();
	
	int nbrTrajThread = ap.FunctionTrajThread2r.size();
	float deltaT = 5e-1f;
	
	float currenttime = 0.0f;
	float dt = 0.0f;
	
	//index of the current goal of the trajectory :
	int nbrStepInTraj = ap.GoalTraj[0][0].getColumn();
	int idxTraj = 0;
	
	//current positions :
	std::vector<Mat<EXP> > r;
	mutexKin.lock();
	for(int i=nbrTrajThread;i--;)	
	{
		r.insert( r.begin(), extract( (ap.ptrRobotPart->*(ap.FunctionTrajThread2r[i])) (), 1,1, 3,1) );
	}
	mutexKin.unlock();
	
	
	//-----------------------------------------------------------
	//P(ID) Controller on 3D end-effector velocities:
	//-----------------------------------------------------------
	//-----------------------------------------------------------
	float p = 1.0f;
	float i = 0.5f;
	float d = 0.0f;
	
	std::vector<Mat<float> >  dx;
	PIDControllerM<float>* pids = new PIDControllerM<float>[nbrTrajThread];
	for(int i=nbrTrajThread;i--;)
	{
		pids[i].set( p,i,d);
		pids[i].setConsigne( ap.GoalTraj[i][idxTraj] );
		dx.insert( dx.begin(), pids[i].update(EXP2floatM( r[i]), dt) );
	}
	//-----------------------------------------------------------
	//-----------------------------------------------------------
	
	ap.JacobianInitialization();
	
	
	std::cout << "THREAD ACTION :: The INIT took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	
	clock_t elapsedTime = clock();
	while(idxTraj <= nbrStepInTraj)
	{
		elapsedTime -= clock();
		dt = elapsedTime/CLOCKS_PER_SEC;
		elapsedTime = clock();
		
		currenttime += dt;
		idxTraj = (int)(currenttime/deltaT);
		std::cout << " THREAD ACTION :: IDX TRAJ = " << idxTraj << " / " << nbrStepInTraj << std::endl;
		
		mutexKin.lock();
		//-----------------------------------------------------------
		//P(ID) Controller on 3D end-effector velocities:
		//-----------------------------------------------------------
		//-----------------------------------------------------------	
		for(int i=nbrTrajThread;i--;)
		{
			pids[i].setConsigne( ap.GoalTraj[i][idxTraj] );
			dx.insert( dx.begin(), pids[i].update(EXP2floatM( r[i]), dt) );
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		//JACOBIANS & INVERSION :
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		time = clock();
		
		std::vector<Mat<float> > J;
		for(int i=nbrTrajThread;i--;)
		{
			J.insert( J.begin(), EXP2floatM( ap.Traj2J[i] ) );
		}
		mutexKin.unlock();
		
		std::vector<Mat<float> > invJ;
		for(int i=nbrTrajThread;i--;)
		{
			invJ.insert( invJ.begin(), invGJ( transpose(J[i])*J[i] ) );
		}
	
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		std::cout << " THREAD ACTION :: The JACOBIAN INVERSION took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
		
	
		//-----------------------------------------------------------
		//COMPUTATION OF DQ :
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		time = clock();
		
		std::vector<Mat<float> > dq;
		for(int i=nbrTrajThread;i--;)
		{
			dq.insert( dq.begin(), invJ[i] * ( transpose(J[i])*dx[i] ) );
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
	
		//-----------------------------------------------------------
		//PUSHING :
		//-----------------------------------------------------------
		Mat<float> dQ( dq[0] );
		for(int i=1;i<nbrTrajThread;i++)	
		{
			dQ += dq[i];
		}
		ap.ptrRobotPart->setDq( dQ );
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		std::cout << " THREAD ACTION :: The COMPUTATION OF DQ + PUSHING took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
		
		//-----------------------------------------------------------
		
		dx.clear();
	
	}
	
	delete[] pids;
	
	std::cout << " THREAD ACTION :: END OF ACTION . " << std::endl;
}



void HAROEXP::GenerateActionParams()
{
	//TODO !!!
}

void HAROEXP::generateTrajectories()
{
	std::cout << "GENERATION OF TRAJECTORIES : ... " << std::endl;
	
	//idxTraj2r[0] = &HAROLegsEXP::getRkneer;
	idxTraj2r[0] = &RobotPart::getRkneer;
	//idxTraj2r[1] = &HAROLegsEXP::getRkneel;
	idxTraj2r[1] = &RobotPart::getRkneel;
	
	std::cout << " ... " ;
	//initializations :
	for(int i=nbrTraj;i--;)
	{
		trajectories[i] = EXP2floatM( extract( ( harolegs->*(idxTraj2r[i]) )(), 1,1, 3,1) );
		std::cout << "Traj : " << i << std::endl;
		trajectories[i].afficher();
	}
	
	//evaluate( this->harolegs->getRkneer() );
	//trajectories[0] = EXP2floatM(  this->harolegs->getRkneer() );
	//on the current position of the robot...
	
	std::cout << " ... " ;
	
	Mat<float> add1(0.0f,3,1);
	Mat<float> add2(0.0f,3,1);
	add1.set( -1e-1f, 1,1);
	add2.set( 5e-2f, 1,1);
	//5 centimeters on the X forward axis.
	
	std::cout << " ... " ;
	//final desired position :
	for(int i=nbrTraj;i--;)	
	{
		trajectories[i] += ((i+1)%2?add1:add2);
		
		/*
		trajectories[i] = operatorL( trajectories[i], extract( EXP2floatM( (  harolegs->*(idxTraj2r[i]) )() ), 1,1, 3,1)  + add );
		*/
		for(int j=1;j<=10;j++)
		{
			trajectories[i] = operatorL( trajectories[i], extract( EXP2floatM( (  harolegs->*(idxTraj2r[i]) )() ), 1,1, 3,1) );
			trajectories[i] = operatorL( trajectories[i], extract( EXP2floatM( (  harolegs->*(idxTraj2r[i]) )() ), 1,1, 3,1)  + ((j+i+1)%2? add1 : add2/*(-1.0f)*add*/ ) );
		}
		
		trajectories[i].afficher();
		
	}
	
	std::cout << " OKAY." << std::endl;
	
	
	std::cout << "GENERATION OF JACOBIANS : ... " << std::endl;
	clock_t time = clock();
	
	for(int i=nbrTraj;i--;)
	{
		idxTraj2J[i] = harolegs->generateJacobian( extract( (harolegs->*(idxTraj2r[i]) )(), 1,1, 3,1)  );
		//TODO : only works if nbrtraj == 2
		idxTraj2J[i] = extract( idxTraj2J[i], 1,i*5+1, 3,i*5+5);
		evaluate( idxTraj2J[i] );
	}
	
	std::cout << "The JACOBIAN DERIVATION took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;

	std::cout << " OKAY." << std::endl;
	//TODO
	
}

void HAROEXP::generateTrajectoriesBASSIN()
{
	std::cout << "GENERATION OF TRAJECTORIES for BASSIN : ... " << std::endl;
	
	//idxTraj2r[0] = &HAROLegsEXP::getRbassinR;
	idxTraj2r[0] = &RobotPart::getRbassinR;
	//idxTraj2r[1] = &HAROLegsEXP::getRbassinL;
	idxTraj2r[1] = &RobotPart::getRbassinL;
	
	std::cout << " ... " ;
	//initializations :
	for(int i=nbrTraj;i--;)
	{
		trajectories[i] = EXP2floatM( extract( ( harolegs->*(idxTraj2r[i]) )(), 1,1, 3,1) );
		std::cout << "Traj : " << i << std::endl;
		trajectories[i].afficher();
	}
	
	//evaluate( this->harolegs->getRkneer() );
	//trajectories[0] = EXP2floatM(  this->harolegs->getRkneer() );
	//on the current position of the robot...
	
	std::cout << " ... " ;
	
	Mat<float> add1(0.0f,3,1);
	Mat<float> add2(0.0f,3,1);
	add1.set( -5e-2f, 1,1);
	add2.set( 5e-2f, 1,1);
	//5 centimeters on the X forward axis.
	
	std::cout << " ... " ;
	//final desired position :
	for(int i=nbrTraj;i--;)	
	{
		trajectories[i] += ((i+1)%2?add1:add2);
		
		/*
		trajectories[i] = operatorL( trajectories[i], extract( EXP2floatM( (  harolegs->*(idxTraj2r[i]) )() ), 1,1, 3,1)  + add );
		*/
		for(int j=1;j<=10;j++)
		{
			trajectories[i] = operatorL( trajectories[i], extract( EXP2floatM( (  harolegs->*(idxTraj2r[i]) )() ), 1,1, 3,1) );
			trajectories[i] = operatorL( trajectories[i], extract( EXP2floatM( (  harolegs->*(idxTraj2r[i]) )() ), 1,1, 3,1)  + ((j+i+1)%2? add1 : add2/*(-1.0f)*add*/ ) );
		}
		
		trajectories[i].afficher();
		
	}
	
	std::cout << " OKAY." << std::endl;
	
	
	std::cout << "GENERATION OF JACOBIANS : ... " << std::endl;
	clock_t time = clock();
	
	for(int i=nbrTraj;i--;)
	{
		idxTraj2J[i] = harolegs->generateJacobian( extract( (harolegs->*(idxTraj2r[i]) )(), 1,1, 3,1)  );
		//TODO : only works if nbrtraj == 2
		idxTraj2J[i] = extract( idxTraj2J[i], 1,i*5+1, 3,i*5+5);
		evaluate( idxTraj2J[i] );
	}
	
	std::cout << "The JACOBIAN DERIVATION took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;

	std::cout << " OKAY." << std::endl;
	//TODO
	
}




