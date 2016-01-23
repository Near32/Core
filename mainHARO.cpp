#include "EXP/EXP.h"
#include "FUNC/FUNC.h"
#include "VAR/VAR.h"
#include "Operations/Operations.h"
#include "ROBOT/FrameEXP/FrameEXP.h"

//constexpr float PI = 3.141596f;

float toRad(const float& deg)
{
	return (deg*PI)/180.0f;
}

int main(int argc, char* argv[])
{	
	VAR x1r;
	VAR x2r;
	VAR x3r;
	VAR x4r;
	VAR x5r;
	VAR ankle2footr(0.0f);
	
	VAR x1l;
	VAR x2l;
	VAR x3l;
	VAR x4l;
	VAR x5l;
	VAR ankle2footl(0.0f);
	//fixed value...
	
	//in relative manner : from frame n-1 to frame n.
	std::vector<FrameEXP*> tW2R;
	std::vector<FrameEXP*> tW2L;
	Mat<float> finalse3((float)0,6,1);
		
	float W2hY = -0.08f;//-10 centimeters.
	float h2hlY = 0.08f;	//7.5 centimeters.
	float hl2kX = 0.08f;
	float k2aX = 0.08f;
	float a2alX = 0.08f;
	float al2fX = 0.075f;
	
	// add in the class + check in reality ...
	//right hip : nuz, 80 at least offset
	finalse3.set( W2hY, 5,1);
	finalse3.set( toRad(-90), 1,1);
	finalse3.set( toRad(90), 2,1);
	tW2R.insert( tW2R.end(), new FrameEXP( x1r,3,finalse3) );
	
	finalse3 *= 0.0f;
	finalse3.set( -W2hY, 5,1);
	finalse3.set( toRad(90), 1,1);
	finalse3.set( toRad(90), 2,1);
	tW2L.insert( tW2L.end(), new FrameEXP( x1l,3,finalse3) );
	
	std::cout << "TEST : " << std::endl;
	//evaluate(tW2R[0]->getT());
	//toString(tW2R[0]->getT());
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[0] );
	
	//right hip low  : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( h2hlY, 5,1);
	finalse3.set( toRad(-90), 1,1);
	finalse3.set( toRad(-90), 2,1);
	
	tW2R.insert( tW2R.end(), new FrameEXP( x2r,3,finalse3) );
	
	
	finalse3 *= 0.0f;
	finalse3.set( -h2hlY, 5,1);
	finalse3.set( toRad(90), 1,1);
	finalse3.set( toRad(-90), 2,1);
	
	tW2L.insert( tW2L.end(), new FrameEXP( x2l,3,finalse3) );
	
	/*
	Mat<EXP> f(tW2R[0]->getT());
	
	evaluate( f);
	toString( f);
	
	EXP* df = derivateREC( f.get(1,1), x1) ;
	//df = operatorL( df, derivateV(pos, x2) );
	
	std::cout << " df = " << df->evaluate() << std::endl;
	std::cout << " df = " << df->toString() << std::endl;
	
	delete df;
	bool dummy = true;
	EXP fnr( f.get(1,1) );
	std::cout << " f = " << fnr.toString() << std::endl;
	EXP fr( regularize( &fnr, dummy ) );
	
	std::cout << " f = " << fnr.toString() << std::endl;
	std::cout << "regul f = " << fr.toString() << std::endl;
	
	df = derivateREC( fr, x1 );
	std::cout << " dfr = " << df->evaluate() << std::endl;
	std::cout << " dfr = " << df->toString() << std::endl;
	
	std::cout << " reg dfr = " << regularize( df, dummy).toString() << std::endl;
	std::cout << " reg dfr 2 = " << regularize( df, dummy).toString() << std::endl;
	std::cout << " reg dfr 3 = " << regularize( df, dummy).toString() << std::endl;
	std::cout << " reg dfr = " << regularize( df, dummy).evaluate() << std::endl;
	//evaluate(tW2R[1]->getT()*tW2R[0]->getT());
	//toString(tW2R[1]->getT()*tW2R[0]->getT());
	
	for(int i=tW2R.size();i--;)	delete tW2R[i];
	return 1;
	*/
	
	/* test for fzero deletion */
	/*
	FUNC fzero(FTzero);
	EXP e( fzero+fzero*fzero+(cos(x1)+fzero)*cos(x1) );
	//Mat<EXP> t( EXP(fzero), 2,1);
	Mat<EXP> t( 2,1);
	EXP efzero(EOId);
	attach( efzero, (EXP&)fzero);
	t.set( efzero, 1,1);
	t.set( e, 2,1);
	t = t*transpose(t);
	 
	std::cout << "EXP before reg : " << e.toString() << std::endl;
	
	regw(e);
	
	std::cout << "EXP after reg : " << e.toString() << std::endl;
	
	evaluate(t);
	toString(t);
	t = regwM(t);
	toString( t );
	t = regwM(t);
	toString( t );
	
	
	for(int i=tW2R.size();i--;)	delete tW2R[i];
	return 1;
	*/
	
	//idx++;
	//tW2R.insert( tW2R.end(), expM(finalse3)*tW2R[1] );
	
	
	//right hl to knee : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x3r,3,finalse3) );
	
	finalse3 *= 0.0f;
	finalse3.set( hl2kX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x3l,3,finalse3) );
	
	
	//right knee to heel : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( k2aX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x4r,3,finalse3) );
	
	
	finalse3 *= 0.0f;
	finalse3.set( k2aX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x4l,3,finalse3) );
	
	
	//right heel to heel low : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( toRad(90), 1,1);
	tW2R.insert( tW2R.end(), new FrameEXP(x5r,3,finalse3) );

	finalse3 *= 0.0f;
	finalse3.set( a2alX, 4,1);
	finalse3.set( toRad(90), 1,1);
	tW2L.insert( tW2L.end(), new FrameEXP(x5l,3,finalse3) );
	
	
	//right heel low to foot : nuz, offset : half
	finalse3 *= 0.0f;
	finalse3.set( al2fX, 4,1);
	tW2R.insert( tW2R.end(), new FrameEXP(ankle2footr,3,finalse3) );	

	finalse3 *= 0.0f;
	finalse3.set( al2fX, 4,1);
	tW2L.insert( tW2L.end(), new FrameEXP(ankle2footl,3,finalse3) );	
	
	Mat<EXP> Tr( regwM( tW2R[0]->getT() ) );
	Mat<EXP> Tl( regwM( tW2L[0]->getT() ) );

	evaluate( Tr );
	evaluate( Tr );
	
	clock_t time = clock();
	for(int i=1;i<3;i++)
	{
		evaluate(tW2R[i]->getT());
		//toString( regwM( tW2R[i]->getT() ) );
		Mat<EXP> temp( regwM(tW2R[i]->getT()) );
		//Mat<EXP> temp( tW2R[i]->getT() );
		//toString( temp); 
		Tr = product( regwM(Tr), temp );
		//T = product( T, temp );
	}
	
	evaluate( Tr);
	std::cout << "THE MULTIPLICATIONS TOOK : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	time = clock();
	
	for(int i=1;i<6;i++)
	{
		evaluate(tW2L[i]->getT());
		//toString( regwM( tW2R[i]->getT() ) );
		Mat<EXP> temp( regwM(tW2L[i]->getT()) );
		//Mat<EXP> temp( tW2R[i]->getT() );
		//toString( temp); 
		Tl = product( regwM(Tl), temp );
		//T = product( T, temp );
	}
	
	evaluate( Tl);
	std::cout << "THE MULTIPLICATIONS TOOK : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	
	
	Mat<EXP> poseRFr( extract( Tr, 1,4, 3,4) );
	Mat<EXP> poseRFl( extract( Tl, 1,4, 3,4) );
	
	/*
	evaluate(poseRF);
	EXP t( poseRF.get(1,1) );
	regw(t);
	
	std::cout << "THE regularization TOOK : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds." << std::endl;
	std::cout << " ///////////////// \t /////////////////////// \t ///////////////////////" << std::endl;
	std::cout << " ///////////////// \t /////////////////////// \t ///////////////////////" << std::endl;	
	std::cout << " ///////////////// \t /////////////////////// \t ///////////////////////" << std::endl;	
	
	
	std::cout << t.toString() << std::endl;
	std::cout << "Evaluation : " << t.evaluate() << " and originally : " << poseRF.get(1,1).evaluate() << std::endl;
	
	FUNC fone(FTone);
	FUNC fmone(FTmone);
	EXP f( fone*sin(x2) * ( cos(x1)*( ( cos(x2) * fmone ) * sin(x3) ) * x1 * fone ) );
	regw(f);
	std::cout << f.toString() << std::endl;
	*/
	//toString( regwM(poseRF) );

	poseRFr = regwM(poseRFr);
	Mat<EXP> dPoseRFrdX1( regwM( derivateV( poseRFr, x1r) ) );
	Mat<EXP> dPoseRFrdX2( regwM( derivateV( poseRFr, x2r) ) );
	Mat<EXP> dPoseRFrdX3( regwM( derivateV( poseRFr, x3r) ) );
	Mat<EXP> dPoseRFrdX4( regwM( derivateV( poseRFr, x4r) ) );
	Mat<EXP> dPoseRFrdX5( regwM( derivateV( poseRFr, x5r) ) );
	
	poseRFl = regwM(poseRFl);
	Mat<EXP> dPoseRFldX1( regwM( derivateV( poseRFl, x1l) ) );
	Mat<EXP> dPoseRFldX2( regwM( derivateV( poseRFl, x2l) ) );
	Mat<EXP> dPoseRFldX3( regwM( derivateV( poseRFl, x3l) ) );
	Mat<EXP> dPoseRFldX4( regwM( derivateV( poseRFl, x4l) ) );
	Mat<EXP> dPoseRFldX5( regwM( derivateV( poseRFl, x5l) ) );
	
	Mat<EXP> dPoseRFrdQ( operatorL( dPoseRFrdX1, dPoseRFrdX2) );
	//Mat<EXP> dPoseRFrdQ( operatorL( dPoseRFrdX2, dPoseRFrdX3) );
	dPoseRFrdQ = operatorL( dPoseRFrdQ, dPoseRFrdX3);
	dPoseRFrdQ = operatorL( dPoseRFrdQ, dPoseRFrdX4);
	dPoseRFrdQ = operatorL( dPoseRFrdQ, dPoseRFrdX5);
	
	Mat<EXP> dPoseRFldQ( operatorL( dPoseRFldX1, dPoseRFldX2) );
	//Mat<EXP> dPoseRFldQ( operatorL( dPoseRFldX2, dPoseRFldX3) );
	dPoseRFldQ = operatorL( dPoseRFldQ, dPoseRFldX3);
	dPoseRFldQ = operatorL( dPoseRFldQ, dPoseRFldX4);
	dPoseRFldQ = operatorL( dPoseRFldQ, dPoseRFldX5);
	
	
	//std::cout << dPoseRFdX1.get(1,1).toString() << std::endl;
	evaluate(dPoseRFrdQ);
	evaluate(dPoseRFldQ);
	//dPoseRFdQ = extract( dPoseRFdQ, 1,1, 3,3);
	
	FUNC vfzx(FTcst);
	vfzx.setParam(0.1f);
	//5cm/s 
	FUNC fzero(FTzero);
	Mat<EXP> v(fzero,3,1);
	//v.set( vfzx, 3,1);
	v.set( vfzx, 1,1);
	//go forward and up at 5cm/sec on each axis = sqrt(50) cm/s on 45°.
	Mat<float> dprdq( EXP2floatM( dPoseRFrdQ ) );
	Mat<float> invdprdq( invGJ( transpose(dprdq)*dprdq ) );
	Mat<float> vfloat( EXP2floatM( v ) );
	
	Mat<float> dqr( invdprdq*(transpose(dprdq)*vfloat) );
	dprdq.afficher();
	invdprdq.afficher();
	vfloat.afficher();
	dqr.afficher();
	
	bool continuer = true;
	while(continuer)
	{
		float tempx1;
		float tempx2;
		float tempx3;
		float tempx4;
		float tempx5;
	
		std::cout << "X1r : "; std::cin >> tempx1;
		std::cout << "X2r : "; std::cin >> tempx2;
		std::cout << "X3r : "; std::cin >> tempx3;
		std::cout << "X4r : "; std::cin >> tempx4;
		std::cout << "X5r : "; std::cin >> tempx5;
	
		x1r.setValue(tempx1);
		x2r.setValue(tempx2);
		x3r.setValue(tempx3);
		x4r.setValue(tempx4);
		x5r.setValue(tempx5);
	
		evaluate(poseRFr);
		//toString(poseRF);
		
		std::cout << "Quit ? (y/N) : " ;
		char quit = 0;
		std::cin >> quit;
		if( quit == 'y')	continuer = false;
		
	}
	
	for(int i=tW2R.size();i--;)	delete tW2R[i];
	
	return 0;
}
