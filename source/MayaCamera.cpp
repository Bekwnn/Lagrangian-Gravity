#include "MayaCamera.hpp"

#include <iostream>

struct MayaCamera::MayaCameraImpl
{
    MayaCameraImpl() :
        dolly(100.0f),
        tumbleVector(45.0f),
        trackVector(0.0f),
        lastPos(0.0f),
        movement(MayaCamera::CameraMovements::IDLE)
    {
        resetMatrices();
    }

    void resetAll()
    {
        dolly = 100.0f;
        tumbleVector = atlas::math::Vector(45.0f);
        trackVector = atlas::math::Vector(0.0f);
        resetMatrices();
    }

    void resetMatrices()
    {
        USING_ATLAS_MATH_NS;
        USING_GLM_NS;

        dollyMatrix = translate(Matrix4(1.0f), Vector(0, 0, -1.0f * dolly));
        trackMatrix = Matrix4(1.0f);
        tumbleMatrix = 
            rotate(Matrix4(1.0f), radians(tumbleVector.x), Vector(1, 0, 0)) *
            rotate(Matrix4(1.0f), radians(tumbleVector.y), Vector(0, 1, 0));
    }

    float dolly;
    atlas::math::Vector tumbleVector;
    atlas::math::Vector trackVector;
    atlas::math::Point2 lastPos;
    MayaCamera::CameraMovements movement;

    atlas::math::Matrix4 dollyMatrix;
    atlas::math::Matrix4 tumbleMatrix;
    atlas::math::Matrix4 trackMatrix;

	// spline movement
	atlas::math::Vector position;
	glm::quat rotation;
	atlas::math::Matrix4 positionMatrix;
	atlas::math::Matrix4 rotationMatrix;
};

MayaCamera::MayaCamera() :
    mImpl(new MayaCameraImpl),
	bIsPlaying(false),
	waypointIndex(0),
	curCurveTime(0.f)
{ }

MayaCamera::~MayaCamera()
{ }

// update call for spline movement
void MayaCamera::updateCameraRail(atlas::utils::Time const & t)
{
	USING_ATLAS_MATH_NS;
	USING_GLM_NS;

	if (!bIsPlaying) return;

	//update time and waypoint parameters
	curCurveTime += t.deltaTime;
	if (curCurveTime > transitionDurations[waypointIndex])
	{
		curCurveTime -= transitionDurations[waypointIndex];
		waypointIndex++;
		if (waypointIndex > waypoints.size() - 1) waypointIndex = 0;
	}

	Vector lastCameraPos = mImpl->position;
	mImpl->position = getHermitePos(waypointIndex, (curCurveTime/transitionDurations[waypointIndex]), 1.f);
	mImpl->positionMatrix = translate(Matrix4(1.f), mImpl->position - lastCameraPos);

	//camera always looks at the origin
	mImpl->rotationMatrix = glm::lookAt(mImpl->position, Vector(0.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f));

}

void MayaCamera::mouseDown(atlas::math::Point2 const& point,
    CameraMovements movement)
{
	if (bIsPlaying) return;

    mImpl->movement = movement;
    mImpl->lastPos = point;
}

void MayaCamera::mouseDrag(atlas::math::Point2 const& point)
{
    USING_ATLAS_MATH_NS;
    USING_GLM_NS;

	if (bIsPlaying) return; // lock camera controls if animation is playing

    float deltaX = point.x - mImpl->lastPos.x;
    float deltaY = point.y - mImpl->lastPos.y;

    switch (mImpl->movement)
    {
    case CameraMovements::DOLLY: // zoom
        mImpl->dolly -= 0.0008f * deltaX;
        mImpl->dollyMatrix = 
            translate(Matrix4(1.0), Vector(0, 0, -1.0f * mImpl->dolly));
        break;

    case CameraMovements::TUMBLE: // rotate around
        mImpl->tumbleVector.x += 0.005f * deltaY;
        mImpl->tumbleVector.y += 0.005f * deltaX;
        mImpl->tumbleMatrix =
            rotate(mat4(1.0), radians(mImpl->tumbleVector.x), vec3(1, 0, 0)) *
            rotate(mat4(1.0), radians(mImpl->tumbleVector.y), vec3(0, 1, 0));
        break;

    case CameraMovements::TRACK: // pan
        mImpl->trackVector.x += 0.0005f * deltaX;
        mImpl->trackVector.y -= 0.0005f * deltaY;
        mImpl->trackMatrix = translate(mat4(1.0), mImpl->trackVector);
        break;

    case CameraMovements::IDLE:
        break;
    }
}

void MayaCamera::mouseUp()
{
    mImpl->movement = CameraMovements::IDLE;
}

void MayaCamera::resetCamera()
{
    mImpl->resetAll();
}

atlas::math::Matrix4 MayaCamera::getCameraMatrix()
{
	if (bIsPlaying) return mImpl->positionMatrix * mImpl->rotationMatrix;
	else			return mImpl->dollyMatrix * mImpl->trackMatrix * mImpl->tumbleMatrix;
}

atlas::math::Vector MayaCamera::getHermitePos(int indexStartAt, float t, float tangentSize)
{
	USING_ATLAS_MATH_NS;
	if (waypoints.size() < 4) return Vector(0.f, 0.f, 0.f);

	float t2 = t*t;
	float t3 = t2*t;

	Vector m0vec = (waypoints[(indexStartAt + 1) % waypoints.size()] - waypoints[(waypoints.size() + indexStartAt - 1) % waypoints.size()]);
	Vector m1vec = (waypoints[(indexStartAt + 2) % waypoints.size()] - waypoints[indexStartAt]);

	Vector p0 = (2.f*t3 - 3*t2 + 1)*waypoints[indexStartAt];
	Vector m0 = (t3 - 2.f*t2 + t)*tangentSize*m0vec;
	Vector p1 = (-2.f*t3 + 3.f*t2)*waypoints[(indexStartAt + 1) % waypoints.size()];
	Vector m1 = (t3 - t2)*tangentSize*m1vec;

	return p0 + m0 + p1 + m1;
}
