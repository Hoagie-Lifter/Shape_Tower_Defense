#include <iostream>
#include "game.h"
#include <SFML/Graphics.hpp>

int main()
{
	Game g("config/config.txt");
	g.run();
	
	return 0;
}