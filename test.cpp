#include"display.h"
#include"config.h"

int main()
{
	struct Formula* formula;
	char* filename = getFileName();

#ifdef DEBUG
	debug(filename);
#endif // DEBUG

#ifndef DEBUG
	calculate(filename);
#endif // !DEBUG
	//free(filename);
}