#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

static void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height);
static void init();
static void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
static void eventDispatch();
static void processInput();
static void render();
static void log(const string& msg);

static GLFWwindow* pWindow = nullptr; 

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	pWindow = glfwCreateWindow(800, 600, "HW1", nullptr, nullptr); 
    
    if (!pWindow)
    {
        cout << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);
    glfwSetKeyCallback(pWindow, keyCallback);

    init();

    while (!glfwWindowShouldClose(pWindow))
    {
        eventDispatch();
        render();
        glfwSwapBuffers(pWindow);
    }

    glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        glClearColor(0.7f, 0.3f, 0.2f, 1.f);
        log("1 Ű�� �����̽��ϴ�.");
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        glClearColor(0.3f, 0.7f, 0.2f, 1.f);
        log("2 Ű�� �����̽��ϴ�.");
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        glClearColor(0.3f, 0.2f, 0.7f, 1.f);
        log("3 Ű�� �����̽��ϴ�.");
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
        log("ESC Ű�� �����̽��ϴ�.");
    }
}

void init() 
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    log("1, 2, 3 �� �ƹ� Ű�� ����������. ��� ���� ���մϴ�.");
    log("ESC Ű�� ������ â�� �ݽ��ϴ�.");
}

void eventDispatch()
{
    glfwPollEvents();
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void log(const string& msg) 
{
    // c ǥ�� (NDEBUG: �������忡�� ���ǵȴ�.)
#ifndef NDEBUG
    cout << msg << endl;
#endif
}