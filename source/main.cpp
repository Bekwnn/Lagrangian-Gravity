#include <atlas/utils/Application.hpp>

#include "PlanetScene.hpp"

int main(void)
{
	APPLICATION.createWindow(800, 800, "Lagrangian Planets");
	APPLICATION.addScene(new PlanetScene);
	APPLICATION.runApplication();

	return 0;
}
