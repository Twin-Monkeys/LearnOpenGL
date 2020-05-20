#include "HW15Scene.h"
#include "VertexAttributeListFactory.h"
#include "TextureUtil.h"
#include "Constant.h"
#include <iostream>
#include <random>

using namespace std;
using namespace glm;
using namespace GLCore;
using namespace Poodle; 

/* constructor */
HW15Scene::HW15Scene(GLWindow& window) : Scene(window)
{
    __init();
}

/* member function */
void HW15Scene::onKey(const int key, const int scancode, const int action, const int mods) 
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        cout << "ESC Ű�� �����̽��ϴ�. ���α׷��� �����մϴ�." << endl;
        getWindow().setCloseFlag(true);
    }

    if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS))
    {
        cout << "�����̽� Ű�� �����̽��ϴ�. Cube�� ���ġ�˴ϴ�." << endl;
        __setCubes();
    }
}

void HW15Scene::onUpdate(const float deltaTime) 
{
    static float elapsedTime = 0.f;
    elapsedTime += deltaTime;

    const GLWindow& WINDOW = getWindow();
    const float WIDTH = static_cast<float>(WINDOW.getWidth());
    const float HEIGHT = static_cast<float>(WINDOW.getHeight());

    __projectionMat = perspective(quarter_pi<float>(), (WIDTH / HEIGHT), 0.1f, 200.f);

    for (auto& cube : __cubes)
    {
        const vec3& ROTATION = (elapsedTime * cube.rotationSpeed);
        Transform& transform = cube.transform;

        transform.setRotation(ROTATION);
        transform.updateMatrix(); 
    }
}

void HW15Scene::onRender() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    __pTexture->activate(0);

    __pShaderProgram->setUniform1i("tex", 0);
    __pShaderProgram->setUniformMatrix4f("viewMat", __viewMat);
    __pShaderProgram->setUniformMatrix4f("projectionMat", __projectionMat);
    
    for (const auto& cube : __cubes)
    {
        __pShaderProgram->setUniformMatrix4f("modelMat", cube.transform.getModelMatrix());
        __pShaderProgram->bind();

        __pVao->draw();
    }

    getWindow().swapBuffers();
}

void HW15Scene::__init() 
{
    glfwSwapInterval(1); // VSYNC 0: off, 1: on
    glEnable(GL_DEPTH_TEST);

    constexpr GLfloat vertices[] =
    {
        // top
        // 0 (top left)
        -0.5f, 0.5f, 0.5f, // position
        0.f, 1.f,          // tex-coordinate
        // 1 (top right)
        0.5f, 0.5f, 0.5f,
        1.f, 1.f,
        // 2 (bottom right)
        0.5f, 0.5f, -0.5f,
        1.f, 0.f,
        // 3 (bottom left)
        -0.5f, 0.5f, -0.5f,
        0.f, 0.f,

        // bottom
        // 4 (top left) 
        -0.5f, -0.5f, 0.5f,
        0.f, 1.f,
        // 5 (top right)
        0.5f, -0.5f, 0.5f,
        1.f, 1.f,
        // 6 (bottom right)
        0.5f, -0.5f, -0.5f,
        1.f, 0.f,
        // 7 (bottom left)
        -0.5f, -0.5f, -0.5f,
        0.f, 0.f,

        // left
        // 8 (3) 
        -0.5f, 0.5f, -0.5f,
        0.f, 1.f,
        // 9 (0)
        -0.5f, 0.5f, 0.5f,
        1.f, 1.f,
        // 10 (4)
        -0.5f, -0.5f, 0.5f,
        1.f, 0.f,
        // 11 (7)
        -0.5f, -0.5f, -0.5f,
        0.f, 0.f,

        // right
        // 12 (2) 
        0.5f, 0.5f, -0.5f,
        0.f, 1.f,
        // 13 (1) 
        0.5f, 0.5f, 0.5f,
        1.f, 1.f,
        // 14 (5) 
        0.5f, -0.5f, 0.5f,
        1.f, 0.f,
        // 15 (6) 
        0.5f, -0.5f, -0.5f,
        0.f, 0.f,

        // front
        // 16 (3)
        -0.5f, 0.5f, -0.5f,
        0.f, 1.f,
        // 17 (2) 
        0.5f, 0.5f, -0.5f,
        1.f, 1.f,
        // 18 (6) 
        0.5f, -0.5f, -0.5f,
        1.f, 0.f,
        // 19 (7) 
        -0.5f, -0.5f, -0.5f,
        0.f, 0.f,

        // back
        // 20 (0)
        -0.5f, 0.5f, 0.5f,
        0.f, 1.f,
        // 21 (1) 
        0.5f, 0.5f, 0.5f,
        1.f, 1.f,
        // 22 (5) 
        0.5f, -0.5f, 0.5f,
        1.f, 0.f,
        // 23 (4)
        -0.5f, -0.5f, 0.5f,
        0.f, 0.f
    };

    constexpr GLuint indices[] =
    {
        0, 3, 2, 0, 2, 1,       // top
        4, 7, 6, 4, 6, 5,       // bottom
        8, 10, 9, 8, 11, 10,    // left
        12, 14, 13, 12, 15, 14, // right
        16, 18, 17, 16, 19, 18, // front
        20, 22, 21, 20, 23, 22 // back
    };

    __pVao = make_shared<VertexArray>(
        VertexAttributeListFactory::get(VertexAttributeFlag::POS3 | VertexAttributeFlag::TEXCOORD2),
        make_shared<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW),
        make_shared<IndexBuffer>(indices, sizeof(indices), GL_STATIC_DRAW),
        static_cast<GLsizei>(size(vertices)));

    __pTexture = TextureUtil::createTexture2DFromImage("./res/container.jpg");
    __pTexture->setParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    __pTexture->setParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    __pTexture->setParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    __pTexture->setParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    __pShaderProgram = make_shared<ShaderProgram>("rectangle_vert.glsl", "rectangle_frag.glsl");
    
    __viewMat = translate(__viewMat, vec3{ 0.f, 0.f, -50.f });

    __setCubes();
}

void HW15Scene::__setCubes() 
{
    static default_random_engine generator;
    static const uniform_real_distribution<float> RAND_POSITION(Constant::MIN_POSITION, Constant::MAX_POSITION);
    static const uniform_real_distribution<float> RAND_SIDE(Constant::MIN_SIDE, Constant::MAX_SIDE);
    static const uniform_real_distribution<float> RAND_ROTATION_SPEED(Constant::MIN_ROTATION_SPEED, Constant::MAX_ROTATION_SPEED);

    for (auto& cube : __cubes)
    {
        Transform& transform = cube.transform;

        transform.setPosition(
            RAND_POSITION(generator), RAND_POSITION(generator), RAND_POSITION(generator));
        
        transform.setScale(RAND_SIDE(generator));

        cube.rotationSpeed.x = RAND_ROTATION_SPEED(generator);
        cube.rotationSpeed.y = RAND_ROTATION_SPEED(generator);
        cube.rotationSpeed.z = RAND_ROTATION_SPEED(generator);
    }
}