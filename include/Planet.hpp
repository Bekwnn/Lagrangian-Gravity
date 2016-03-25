#ifndef PLANET_HPP
#define PLANET_HPP

#include <atlas/utils/Geometry.hpp>
#include <atlas/primitives/Sphere.hpp>
#include <atlas/utils/Time.hpp>

class Planet : public atlas::utils::Geometry
{
public:
	Planet(float mass = 1.0f, float radius = 1.0f, int subDivsX = 20, int subDivsY = 20);
	~Planet();

	void renderGeometry(atlas::math::Matrix4 projection,
		atlas::math::Matrix4 view) override;
	void transformGeometry(atlas::math::Matrix4 const& t) override;

	float mass;
	float r;
	float rdot;
	float rdotdot;
	float theta;
	float thetadot;
	float thetadotdot;

private:

	atlas::primitives::Sphere mBall;
};

#endif
