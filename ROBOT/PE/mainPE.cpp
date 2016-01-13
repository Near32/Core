#include "PE.h"

int main(int argc, char* argv[])
{
	PE pe;
	
	pe.initialize();
	
	int i = 10;
	while(1)
	{
		pe.callback();
	}
	
	return 0;
}

