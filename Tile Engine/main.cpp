#include "TileEngine.h"



int main(void)
{
	if(!al_init())
	{
		return -1;
	}

	Engine* engine = new Engine(); 
	
	engine->start();

	return 0;
}