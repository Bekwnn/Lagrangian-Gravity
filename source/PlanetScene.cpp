#include "PlanetScene.hpp"

#include <atlas/core/GLFW.hpp>
#include <atlas/core/Macros.hpp>

#define GRAV 6.674e-11

PlanetScene::PlanetScene() :
	mIsPlaying(false),
	mLastTime(0.0)
{
	glEnable(GL_DEPTH_TEST);

	planet = new Planet(1e15);
	moon = new Planet();
	auto translB = glm::translate(atlas::math::Matrix4(1.0f), glm::vec3(0.f, 0.f, 25.f));
	moon->transformGeometry(translB);
	moon->r = 25.f;
	moon->thetadot = 2.f;
}

PlanetScene::~PlanetScene()
{
	delete planet;
	delete moon;
}

void PlanetScene::renderScene()
{
	float grey = 161.0f / 255.0f;
	glClearColor(grey, grey, grey, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mView = mCamera.getCameraMatrix();
	mGrid.renderGeometry(mProjection, mView);
	planet->renderGeometry(mProjection, mView);
	moon->renderGeometry(mProjection, mView);
}

void PlanetScene::updateScene(double time)
{
	if (mIsPlaying)
	{
		// Handle the timing stuff here.
		mTime.deltaTime = (float)time - mTime.currentTime;
		mTime.totalTime += (float)time;
		mTime.currentTime = (float)time;

		//TODO lagrangian mechanics
		moon->rdotdot = mTime.deltaTime * ( moon->r * moon->thetadot * moon->thetadot - (planet->mass * GRAV) / (moon->r * moon->r) );
		moon->rdot = moon->rdot + moon->rdotdot * mTime.deltaTime;
		moon->r = moon->r + moon->rdot * mTime.deltaTime;

		moon->thetadotdot = mTime.deltaTime * (-2.f * moon->rdot * moon->thetadot / moon->r);
		moon->thetadot = moon->thetadot + moon->thetadotdot * mTime.deltaTime;
		moon->theta = moon->theta + moon->thetadot * mTime.deltaTime;

		auto posMat = glm::translate(atlas::math::Matrix4(1.0f), glm::vec3(moon->r*sinf(moon->theta), 0.f, moon->r*cosf(moon->theta)));
		moon->transformGeometry(posMat);
	}
}

void PlanetScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_R:
			mCamera.resetCamera();
			break;

		case GLFW_KEY_SPACE:
			if (mIsPlaying)
			{
				mLastTime = glfwGetTime();
			}
			else
			{
				glfwSetTime(mLastTime);
				mIsPlaying = !mIsPlaying;
				mCamera.bIsPlaying = mIsPlaying;
			}

		default:
			break;
		}
	}
}

void PlanetScene::mousePressEvent(int button, int action, int modifiers, double xPos, double yPos)
{
	USING_ATLAS_MATH_NS;

	if (button == GLFW_MOUSE_BUTTON_LEFT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TUMBLE);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TRACK);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			// first click.
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::DOLLY);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (action != GLFW_PRESS)
	{
		mIsDragging = false;
		mCamera.mouseUp();
	}
}

void PlanetScene::mouseMoveEvent(double xPos, double yPos)
{
	if (mIsDragging)
	{
		mCamera.mouseDrag(atlas::math::Point2(xPos, yPos));
	}
}

void PlanetScene::screenResizeEvent(int width, int height)
{
	glViewport(0, 0, width, height);
	mProjection = glm::perspective(glm::radians(45.0),
		(double)width / height, 1.0, 1000.0);
}
