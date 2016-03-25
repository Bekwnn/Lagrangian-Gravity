#include "Grid.hpp"
#include "ShaderPaths.hpp"

#include <atlas/gl/Shader.hpp>
#include <atlas/gl/ErrorCheck.hpp>

Grid::Grid() :
    mNumVertex(100)
{
    USING_ATLAS_GL_NS;
    USING_ATLAS_MATH_NS;

    mModel = Matrix4(1.0f);

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    Vector4* vertices = new Vector4[mNumVertex];
    int n = mNumVertex / 4;

    // Generate the coordinates for each point on the grid
    for (int i = 0; i < n; i++)
    {
        vertices[4 * i] = Vector4((-12.0f + i), 0.0f, -12.0f, 1.0f);
        vertices[4 * i + 1] = Vector4((-12.0f + i), 0.0f, 12.0f, 1.0f);
        vertices[4 * i + 2] = Vector4(-12.0f, 0.0f, (-12.0f + i), 1.0f);
        vertices[4 * i + 3] = Vector4(12.0f, 0.0f, (-12.0f + i), 1.0f);
    }

    // Create the vao and vbo.

    glGenBuffers(1, &mBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * mNumVertex, vertices,
        GL_STATIC_DRAW);

    std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

    std::vector<ShaderInfo> shaders
    {
        { GL_VERTEX_SHADER, shaderDir + "grid.vs.glsl" },
        { GL_FRAGMENT_SHADER, shaderDir + "grid.fs.glsl" }
    };

    mShaders.push_back(ShaderPointer(new Shader));
    mShaders[0]->compileShaders(shaders);
    mShaders[0]->linkShaders();

    GLuint var;
    var = mShaders[0]->getUniformVariable("MVP");
    mUniforms.insert(UniformKey("MVP", var));

    var = mShaders[0]->getUniformVariable("color");
    mUniforms.insert(UniformKey("color", var));

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    mShaders[0]->disableShaders();

    delete[] vertices;
}

Grid::~Grid()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mBuffer);
}

void Grid::renderGeometry(atlas::math::Matrix4 projection,
    atlas::math::Matrix4 view)
{
    USING_ATLAS_MATH_NS;

    mShaders[0]->enableShaders();

    glBindVertexArray(mVao);

    Matrix4 mvp = projection * view * mModel;
    glUniformMatrix4fv(mUniforms["MVP"], 1, GL_FALSE, &mvp[0][0]);

    glUniform4f(mUniforms["color"], 0, 0, 0, 1);
    int n = (mNumVertex / 2) - 2;
    glDrawArrays(GL_LINES, n, 4);

    float rgb = (127.0f / 255.0f);
    glUniform4f(mUniforms["color"], rgb, rgb, rgb, 1);
    glDrawArrays(GL_LINES, 0, mNumVertex);

    mShaders[0]->disableShaders();
}