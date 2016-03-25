#ifndef LAB02_INCLUDE_GRID_HPP
#define LAB02_INCLUDE_GRID_HPP

#pragma once

#include <atlas/utils/Geometry.hpp>

class Grid : public atlas::utils::Geometry
{
public:
    Grid();
    ~Grid();

    void renderGeometry(atlas::math::Matrix4 projection, 
        atlas::math::Matrix4 view) override;

private:
    GLuint mBuffer;
    GLuint mVao;
    int mNumVertex;
};

#endif