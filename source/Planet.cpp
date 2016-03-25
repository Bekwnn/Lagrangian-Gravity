#include "Planet.hpp"
#include "ShaderPaths.hpp"

Planet::Planet(float mass, float radius, int subDivsX, int subDivsY) :
	mBall(radius, subDivsX, subDivsY),
	mass(mass),
	r(0.f), rdot(0.f), rdotdot(0.f),
	theta(0.f), thetadot(0.f), thetadotdot(0.f)
{
	USING_ATLAS_MATH_NS;
	USING_ATLAS_GL_NS;

	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	std::vector<ShaderInfo> shaders
	{
		{ GL_VERTEX_SHADER, shaderDir + "ball.vs.glsl" },
		{ GL_FRAGMENT_SHADER, shaderDir + "ball.fs.glsl" }
	};

	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();

	mUniforms.insert(UniformKey("mvpMat",
		mShaders[0]->getUniformVariable("mvpMat")));

	mBall.createBuffers();

	transformGeometry(atlas::math::Matrix4(1.0f));

	mShaders[0]->disableShaders();
}

Planet::~Planet()
{
}

void Planet::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
{
	mShaders[0]->enableShaders();

	auto mvpMat = projection * view * mModel;
	glUniformMatrix4fv(mUniforms["mvpMat"], 1, GL_FALSE, &mvpMat[0][0]);

	mBall.drawPrimitive();

	mShaders[0]->disableShaders();
}

void Planet::transformGeometry(atlas::math::Matrix4 const & t)
{
	mModel = t;
}
