#include <iostream>
#include "game.h"
#include <SFML/Graphics.hpp>

int main()
{
	Game g("bin/config/config.txt");
	g.run();
	
	return 0;
}