#ifndef PLANETSCENE_HPP
#define PLANETSCENE_HPP

#include <atlas/utils/Scene.hpp>
#include "Planet.hpp"
#include "MayaCamera.hpp"
#include "Grid.hpp"

class PlanetScene : public atlas::utils::Scene
{
public:
	PlanetScene();
	~PlanetScene();

	void renderScene() override;
	virtual void updateScene(double time) override;

	void keyPressEvent(int key, int scancode, int action, int mods) override;
	void mousePressEvent(int button, int action, int modifiers, double xPos, double yPos) override;
	void mouseMoveEvent(double xPos, double yPos) override;

	void screenResizeEvent(int width, int height) override;

private:

	Planet* planet;
	Planet* moon;

	bool mIsPlaying;
	bool mIsDragging;
	double mLastTime;

	Grid mGrid;
	MayaCamera mCamera;
};

#endif
