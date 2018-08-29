
#include <iostream>
#include "blah.h"
#include "Engine.h"

#define DBG BLAH_TRANSIENT_LOG("dbg")

using namespace std;

int main()
{
	//create a game engine instance and run it.
	Engine game;
	game.run();
	
	return 0;
}
