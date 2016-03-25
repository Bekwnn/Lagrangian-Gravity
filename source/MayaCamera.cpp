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
};

MayaCamera::MayaCamera() :
    mImpl(new MayaCameraImpl),
	bIsPlaying(false)
{ }

MayaCamera::~MayaCamera()
{ }

void MayaCamera::mouseDown(atlas::math::Point2 const& point,
    CameraMovements movement)
{
    mImpl->movement = movement;
    mImpl->lastPos = point;
}

void MayaCamera::mouseDrag(atlas::math::Point2 const& point)
{
    USING_ATLAS_MATH_NS;
    USING_GLM_NS;

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
	return mImpl->dollyMatrix * mImpl->trackMatrix * mImpl->tumbleMatrix;
}