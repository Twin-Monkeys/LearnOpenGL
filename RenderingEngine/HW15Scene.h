#pragma once

#include <glad/glad.h>
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Cube.h"

class HW15Scene : public Poodle::Scene 
{
public:
	/* constructor */
	explicit HW15Scene(GLCore::GLWindow& window);

	/* member function */
	virtual void onKey(const int key, const int scancode, const int action, const int mods) override;
	virtual void onUpdate(const float deltaTime) override;
	virtual void onRender() override;

private:
	/* member variable */
	std::shared_ptr<GLCore::VertexArray> __pVao;
	std::shared_ptr<GLCore::ShaderProgram> __pShaderProgram;
	std::shared_ptr<GLCore::Texture2D> __pTexture;
	std::array<Poodle::Cube, 10> __cubes;
	glm::mat4 __viewMat{ 1.f };
	glm::mat4 __projectionMat{ 1.f };

	/* member function */
	void __init();
	void __setCubes();
};