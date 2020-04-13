#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "TextReader.h"

using namespace std;

static bool initGL();
static void registerCallback();
static void startMainLoop();
static void releaseGL();
static void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height);
static void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
static void eventDispatch();
static void updateShader();
static void render();
static void log(const string& msg);

static GLFWwindow* pWindow = nullptr;
static GLuint vao;
static GLuint shaderProgram;

static float displacement = 0.f;

static float acceleration = 0.001f;
static const float MAX_ACCELERATION = 0.5f;
static const float MIN_ACCELERATION = 0.001f;

static bool accelerationFlag = false;
static bool colorFlag = false;

int main()
{
    if (!initGL())
        return -1;

    registerCallback();

    GLfloat vertices[] = 
    {
        0.f, 0.577350259f, 0.f, // top
        1.f, 0.f, 0.f, // red

        -0.5f, -0.288675129f, 0.f, // left,
        0.f, 1.f, 0.f, // green

        0.5f, -0.288675129f, 0.f, // right
        0.f, 0.f, 1.f // blue
    };

    GLuint indices[] = 
    {  
        0, 1, 2
    };
   
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), reinterpret_cast<const void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), reinterpret_cast<const void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const string &vertexShaderSource = TextReader::read("triangle_vert.glsl");
    const char* const pRawVertexShaderSource = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &pRawVertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);

        log(infoLog);
        glfwTerminate();

        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const string& fragShaderSource = TextReader::read("triangle_frag.glsl");
    const char* const pRawFragShaderSource = fragShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &pRawFragShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);

        log(infoLog);
        glfwTerminate();

        return -1;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        
        log(infoLog);
        glfwTerminate();

        return -1;
    }

    glDeleteShader(vertexShader);
    vertexShader = 0U;

    glDeleteShader(fragmentShader);
    fragmentShader = 0U;

    startMainLoop();
    releaseGL();

    return 0;
}

bool initGL() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(800, 800, "HW4", nullptr, nullptr);

    if (!pWindow)
    {
        log("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        log("Failed to initialize GLAD");
        glfwTerminate();
        return false;
    }

    return true;
}

void registerCallback() 
{
    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);
    glfwSetKeyCallback(pWindow, keyCallback);
}

void startMainLoop() 
{
    while (!glfwWindowShouldClose(pWindow))
    {
        eventDispatch();
        updateShader();
        render();
        glfwSwapBuffers(pWindow);
    }
}

void releaseGL() 
{
    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        log("ESC Ű�� �����̽��ϴ�. ���α׷��� �����մϴ�.");
        glfwSetWindowShouldClose(pWindow, true);
    }
    else if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS))
    {
        log("SPACE Ű�� �����̽��ϴ�. �ӵ��� ���������� �����մϴ�.");
        accelerationFlag = true;
    }
    else if ((key == GLFW_KEY_SPACE) && (action == GLFW_RELEASE)) 
    {
        log("SPACE Ű�� �����Ǿ����ϴ�. �ӵ��� ���������� �����մϴ�.");
        accelerationFlag = false;
    }
    else if ((key == GLFW_KEY_1) && (action == GLFW_PRESS)) 
    {
        log("1�� Ű�� �����̽��ϴ�. ������ ����˴ϴ�.");
        colorFlag = !colorFlag;
    }
}

void eventDispatch()
{
    glfwPollEvents();
}

void updateShader() 
{
    if (accelerationFlag)
    {
        acceleration += 0.000001f;

        if (acceleration > MAX_ACCELERATION)
            acceleration = MAX_ACCELERATION;
    }
    else 
    {
        acceleration -= 0.000001f;

        if (acceleration < MIN_ACCELERATION)
            acceleration = MIN_ACCELERATION;
    }

    displacement += acceleration;

    glUseProgram(shaderProgram);
    glUniform1f(0, displacement);
    glUniform1i(1, colorFlag);

    if (colorFlag) 
    {
        const float ACCUMULATED_TIME = static_cast<float>(glfwGetTime());
        const float COLOR = ((sinf(ACCUMULATED_TIME) + 1.f) * 0.5f);

        glUniform3f(2, COLOR, 0.f, 0.f);
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void log(const string& msg)
{
    // c ǥ�� (NDEBUG: �������忡�� ���ǵȴ�.)
#ifndef NDEBUG
    cout << msg << endl;
#endif
}