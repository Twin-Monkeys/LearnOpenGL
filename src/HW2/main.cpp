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
static void framebufferSizeCallback(GLFWwindow* pWindow, const int width, const int height);
static void keyCallback(GLFWwindow* pWindow, const int key, const int scancode, const int action, const int mods);
static void eventDispatch();
static void render();
static void log(const string& msg);

static GLFWwindow* pWindow = nullptr;
static GLuint vao;
static GLuint shaderProgram;

int main()
{
    if (!initGL())
        return -1;

    registerCallback();

    cout << "ȭ�鿡 ���ﰢ���� �׸��� ���α׷��Դϴ�." << endl;
    cout << "���� ����(float Ÿ�� �Ǽ�)�� �Է��ϼ���: ";
    
    float side = 0;
    cin >> side;

    const float vertices[] = 
    { 
        // top
        0.f, ((sqrtf(3.f) / 3.f) * side), 0.f,

        // left
       -(0.5f * side), -((sqrtf(3.f) / 6.f) * side), 0.f,

        // right
        (0.5f * side), -((sqrtf(3.f) / 6.f) * side), 0.f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), reinterpret_cast<const void *>(0));

    glEnableVertexAttribArray(0);
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

    glDeleteShader(vertexShader);
    vertexShader = 0U;

    glDeleteShader(fragmentShader);
    fragmentShader = 0U;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) 
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        
        log(infoLog);
        glfwTerminate();

        return -1;
    }

    glUseProgram(shaderProgram);
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);

    startMainLoop();
    releaseGL();

    return 0;
}

bool initGL() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pWindow = glfwCreateWindow(600, 600, "HW2", nullptr, nullptr);

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
        render();
        glfwSwapBuffers(pWindow);
    }
}

void releaseGL() 
{
    glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* const pWindow, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* const pWindow, const int key, const int scancode, const int action, const int mods)
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        glfwSetWindowShouldClose(pWindow, true);
        log("ESC Ű�� �����̽��ϴ�.");
    }
}

void eventDispatch()
{
    glfwPollEvents();
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void log(const string& msg)
{
    // c ǥ�� (NDEBUG: �������忡�� ���ǵȴ�.)
#ifndef NDEBUG
    cout << msg << endl;
#endif
}