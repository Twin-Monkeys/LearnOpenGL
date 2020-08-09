#include "LightingTestScene.h"
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
LightingTestScene::LightingTestScene(GLWindow& window) : Scene(window) 
{
	__init();
}

/* member function */
void LightingTestScene::onKey(const int key, const int scancode, const int action, const int mods) 
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        cout << "[ESC Ȱ��] ���α׷� ����" << endl;
        getWindow().setCloseFlag(true);
    }

    if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS))
    {
        cout << "[SPACE] ���콺 Ŀ�� ���" << endl;
        __mouseCursor = !__mouseCursor;
        showMouseCursor(__mouseCursor);
    }

    if ((key == GLFW_KEY_W) && (action == GLFW_PRESS))
    {
        cout << "[W Ȱ��] ī�޶� ����" << endl;
        __wPressed = true;
    }

    if ((key == GLFW_KEY_W) && (action == GLFW_RELEASE))
    {
        cout << "[W ����]" << endl;
        __wPressed = false;
    }

    if ((key == GLFW_KEY_S) && (action == GLFW_PRESS))
    {
        cout << "[S Ȱ��] ī�޶� ����" << endl;
        __sPressed = true;
    }

    if ((key == GLFW_KEY_S) && (action == GLFW_RELEASE))
    {
        cout << "[S ����]" << endl;
        __sPressed = false;
    }

    if ((key == GLFW_KEY_A) && (action == GLFW_PRESS))
    {
        cout << "[A Ȱ��] ī�޶� ���� �̵�" << endl;
        __aPressed = true;
    }

    if ((key == GLFW_KEY_A) && (action == GLFW_RELEASE))
    {
        cout << "[A ����]" << endl;
        __aPressed = false;
    }

    if ((key == GLFW_KEY_D) && (action == GLFW_PRESS))
    {
        cout << "[D Ȱ��] ī�޶� ���� �̵�" << endl;
        __dPressed = true;
    }

    if ((key == GLFW_KEY_D) && (action == GLFW_RELEASE))
    {
        cout << "[D ����]" << endl;
        __dPressed = false;
    }

    if ((key == GLFW_KEY_Q) && (action == GLFW_PRESS))
    {
        cout << "[Q Ȱ��] ī�޶� �ϰ�" << endl;
        __qPressed = true;
    }

    if ((key == GLFW_KEY_Q) && (action == GLFW_RELEASE))
    {
        cout << "[Q ����]" << endl;
        __qPressed = false;
    }

    if ((key == GLFW_KEY_E) && (action == GLFW_PRESS))
    {
        cout << "[E Ȱ��] ī�޶� ���" << endl;
        __ePressed = true;
    }

    if ((key == GLFW_KEY_E) && (action == GLFW_RELEASE))
    {
        cout << "[E ����]" << endl;
        __ePressed = false;
    }
}

void LightingTestScene::onUpdate(const float deltaTime) 
{
    Transform& cameraTransform = __pCamera->getTransform();
    const GLfloat TRANSLATION_STEP = (deltaTime * Constant::Camera::TRANSLATION_STEP);

    if (__wPressed)
        cameraTransform.advanceZ(-TRANSLATION_STEP);
    if (__sPressed)
        cameraTransform.advanceZ(TRANSLATION_STEP);
    if (__aPressed)
        cameraTransform.advanceX(-TRANSLATION_STEP);
    if (__dPressed)
        cameraTransform.advanceX(TRANSLATION_STEP);
    if (__qPressed)
        cameraTransform.advanceY(-TRANSLATION_STEP);
    if (__ePressed)
        cameraTransform.advanceY(TRANSLATION_STEP);

    __pCamera->update();

    const float ANGLE = (deltaTime * __cube.rotationSpeed);
    
    Transform& transform = __cube.transform;
    transform.rotateLocal(ANGLE, ANGLE, ANGLE);
    transform.updateMatrix();
}

void LightingTestScene::onRender() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    __pShaderProgram->setUniform3f("material.ambient", 0.3f, 0.3f, 0.2f);
    __pShaderProgram->setUniform3f("material.diffuse", 1.f, 1.f, 0.6f);
    __pShaderProgram->setUniform3f("material.specular", 1.f, 1.f, 1.f);
    __pShaderProgram->setUniform1f("material.shininess", 160.f);
    __pShaderProgram->setUniform3f("light.ambient", 0.7f, 0.2f, 0.2f);
    __pShaderProgram->setUniform3f("light.diffuse", 0.5f, 0.1f, 0.1f);
    __pShaderProgram->setUniform3f("light.specular", 1.f, 0.5f, 0.5f);

    const vec3 DIRECTION = normalize(vec3(1.f, 0.f, 0.f));
    __pShaderProgram->setUniform3f("light.direction", DIRECTION.x, DIRECTION.y, DIRECTION.z);

    const vec4 CAMERA_DIRECTION = __pCamera->getTransform().getBasisZ();
    __pShaderProgram->setUniform3f("camera.direction", CAMERA_DIRECTION.x, CAMERA_DIRECTION.y, CAMERA_DIRECTION.z);

    __pShaderProgram->setUniformMatrix4f("viewMat", __pCamera->getViewMatrix());
    __pShaderProgram->setUniformMatrix4f("projectionMat", __pCamera->getProjectionMatrix());
    __pShaderProgram->setUniformMatrix4f("modelMat", __cube.transform.getModelMatrix());
    __pShaderProgram->bind();
    
    __pVao->draw();

    getWindow().swapBuffers();
}

void LightingTestScene::onResize(const int width, const int height) 
{
    __super::onResize(width, height);
    __pCamera->setAspectRatio(width, height);
}

void LightingTestScene::onMouseButton(const int button, const int action, const int mods) 
{
    
}

void LightingTestScene::onMouseMove(const double xPos, const double yPos) 
{
    if (!__mouseMoved)
    {
        __mouseXPos = xPos;
        __mouseYPos = yPos;
        __mouseMoved = true;
    }

    const GLfloat PITCH = (static_cast<GLfloat>(__mouseYPos - yPos) * 0.002f);
    const GLfloat YAW = (static_cast<GLfloat>(__mouseXPos - xPos) * 0.002f);

    Transform& transform = __pCamera->getTransform();
    transform.rotateFPS(PITCH, YAW);

    __mouseXPos = xPos;
    __mouseYPos = yPos;
}

void LightingTestScene::onScroll(const double delta) 
{
    __pCamera->adjustFov(-(static_cast<float>(delta) * 0.03f));
}

void LightingTestScene::__init() 
{
    showMouseCursor(false);

    glfwSwapInterval(1); // VSYNC 0: off, 1: on
    glEnable(GL_DEPTH_TEST);

    constexpr GLfloat vertices[] =
    {
        // top
        // 0 (top left)
        -0.5f, 0.5f, 0.5f, // position
        0.f, 1.f, 0.f, // normal
        // 1 (top right)
        0.5f, 0.5f, 0.5f,
        0.f, 1.f, 0.f, 
        // 2 (bottom right)
        0.5f, 0.5f, -0.5f,
        0.f, 1.f, 0.f,
        // 3 (bottom left)
        -0.5f, 0.5f, -0.5f,
        0.f, 1.f, 0.f,

        // bottom
        // 4 (top left) 
        -0.5f, -0.5f, 0.5f,
        0.f, -1.f, 0.f,
        // 5 (top right)
        0.5f, -0.5f, 0.5f,
        0.f, -1.f, 0.f,
        // 6 (bottom right)
        0.5f, -0.5f, -0.5f,
        0.f, -1.f, 0.f,
        // 7 (bottom left)
        -0.5f, -0.5f, -0.5f,
        0.f, -1.f, 0.f,

        // left
        // 8 (3) 
        -0.5f, 0.5f, -0.5f,
        -1.f, 0.f, 0.f,
        // 9 (0)
        -0.5f, 0.5f, 0.5f,
        -1.f, 0.f, 0.f,
        // 10 (4)
        -0.5f, -0.5f, 0.5f,
        -1.f, 0.f, 0.f,
        // 11 (7)
        -0.5f, -0.5f, -0.5f,
        -1.f, 0.f, 0.f,

        // right
        // 12 (2) 
        0.5f, 0.5f, -0.5f,
        1.f, 0.f, 0.f,
        // 13 (1) 
        0.5f, 0.5f, 0.5f,
        1.f, 0.f, 0.f,
        // 14 (5) 
        0.5f, -0.5f, 0.5f,
        1.f, 0.f, 0.f,
        // 15 (6) 
        0.5f, -0.5f, -0.5f,
        1.f, 0.f, 0.f,

        // front
        // 16 (3)
        -0.5f, 0.5f, 0.5f,
        0.f, 0.f, 1.f,
        // 17 (2) 
        0.5f, 0.5f, 0.5f,
        0.f, 0.f, 1.f,
        // 18 (6) 
        0.5f, -0.5f, 0.5f,
        0.f, 0.f, 1.f,
        // 19 (7) 
        -0.5f, -0.5f, 0.5f,
        0.f, 0.f, 1.f,

        // back
        // 20 (0)
        -0.5f, 0.5f, -0.5f,
        0.f, 0.f, -1.f,
        // 21 (1) 
        0.5f, 0.5f, -0.5f,
        0.f, 0.f, -1.f,
        // 22 (5) 
        0.5f, -0.5f, -0.5f,
        0.f, 0.f, -1.f,
        // 23 (4)
        -0.5f, -0.5f, -0.5f,
        0.f, 0.f, -1.f
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
        VertexAttributeListFactory::get(VertexAttributeFlag::POS3 | VertexAttributeFlag::NORMAL3),
        make_shared<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW),
        make_shared<IndexBuffer>(indices, sizeof(indices), GL_STATIC_DRAW),
        static_cast<GLsizei>(size(vertices)));

    __pCamera = make_unique<PerspectiveCamera>();
    __pCamera->setNear(0.1f);
    __pCamera->setFar(200.f);
    __pCamera->getTransform().advanceZ(5.f);

    __cube.rotationSpeed = 0.5f;

    __pShaderProgram = make_shared<ShaderProgram>("lighting_test_rectangle_vert.glsl", "lighting_test_rectangle_frag.glsl");
}