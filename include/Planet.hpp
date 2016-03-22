#ifndef PLANET_HPP
#define PLANET_HPP

#include <atlas/primitives/Sphere.hpp>

class Planet : public atlas::primitives::Sphere
{
public:
	Planet(float radius = 1.0f, int subDivsX = 20, int subDivsY = 20);
	~Planet();
};

#endif
