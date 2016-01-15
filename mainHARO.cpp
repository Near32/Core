#include "EXP/EXP.h"
#include "FUNC/FUNC.h"
#include "VAR/VAR.h"
#include "Operations/Operations.h"
#include "ROBOT/FrameEXP/FrameEXP.h"


int main(int argc, char* argv[])
{	
	VAR x1;
	VAR x2;
	VAR x3;
	VAR x4;
	VAR x5;
	VAR ankle2foot(0.0f);
	//fixed value...
	
	//in relative manner : from frame n-1 to frame n.
	std::vector<FrameEXP*> tW2R;
	Mat<float> finalse3((float)0,6,1);
		
	float W2hY = -0.1f;//-10 centimeters.
	float h2hlY = 0.075f;	//7.5 centimeters.
	float hl2kX = 0.1f;
	float k2aX = 0.05f;
	float a2alX = 0.075f;
	float al2fX = 0.075f;
	
	// add in the class + check in reality ...
	//right hip : nuz, 80 at least offset
	finalse3.set( W2hY, 5,1);
	finalse3.set( -90, 1,1);
	finalse3.set( 90, 2,1);
	tW2R.insert( tW2R.end(), new FrameEXP( x1,3,finalse3) );
	
	std::cout << "TEST : " << std::endl;
	//evaluate(tW2R[0]->getT());
	//toString(tW2R[0]->getT());
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[0] );
	
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( -90, 1,1);
	finalse3.set( -90, 2,1);
	
	tW2R.insert( tW2R.end(), new FrameEXP( x2,3,finalse3) );
	
	tW2R[1]->getT()*tW2R[0]->getT();
	//evaluate(tW2R[1]->getT()*tW2R[0]->getT());
	//toString(tW2R[1]->getT()*tW2R[0]->getT());
	
	for(int i=tW2R.size();i--;)	delete tW2R[i];
	return 1;
	
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[1] );
	
	
	//right hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x3,3,finalse3) );
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[2] );
	
	
	//right knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2aX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x4,3,finalse3) );
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[3] );
	
	
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( 90, 1,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x5,3,finalse3) );
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[4] );
	
	
	//right heel low to foot : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( al2fX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(ankle2foot,3,finalse3) );	
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[5] );
	
	Mat<EXP> T( tW2R[0]->getT() );
	for(int i=1;i<tW2R.size();i++)	T = tW2R[i]->getT()*T;
	
	bool continuer = true;
	while(continuer)
	{
		float tempx1;
		float tempx2;
		float tempx3;
		float tempx4;
		float tempx5;
	
		std::cout << "X1 : "; std::cin >> tempx1;
		std::cout << "X2 : "; std::cin >> tempx2;
		std::cout << "X3 : "; std::cin >> tempx3;
		std::cout << "X4 : "; std::cin >> tempx4;
		std::cout << "X5 : "; std::cin >> tempx5;
	
		x1.setValue(tempx1);
		x2.setValue(tempx2);
		x3.setValue(tempx3);
		x4.setValue(tempx4);
		x5.setValue(tempx5);
	
		evaluate(T);
		toString(T);
		
		std::cout << "Quit ? (y/N) : " ;
		char quit = 0;
		std::cin >> quit;
		if( quit == 'y')	continuer = false;
		
	}
	
	for(int i=tW2R.size();i--;)	delete tW2R[i];
	
	return 0;
}
