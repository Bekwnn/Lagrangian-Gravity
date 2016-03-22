#ifndef PLANETSCENE_HPP
#define PLANETSCENE_HPP

#include <atlas/utils/Scene.hpp>
#include "Planet.hpp"

class PlanetScene : public atlas::utils::Scene
{
public:
	PlanetScene();
	~PlanetScene();

	Planet* planetA;
	Planet* planetB;
};

#endif
