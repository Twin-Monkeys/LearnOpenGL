#include "GLInitializer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <exception>

using namespace std;

namespace GLCore 
{
	/* static function */
	bool GLInitializer::initGL(
		const int renderContextVersionMajor,
		const int renderContextVersionMinor,
		const int profileMode)
	{
		if (!glfwInit())
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, renderContextVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, renderContextVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, profileMode);

		// dummy window ���� ����: 
		// GLFWwindow�� render context�� �����ϴ� ���ҵ� ������. 
		// glad�� opengl�� �Լ� ������ ����� �׷��� ����̹��κ��� �о���� ���ؼ��� 
		// bind�� render context�� �־�� �ϸ�, �ش� render context�κ��� �Լ� ����� �о��. 
		// ���� opengl �Լ� ����� �ʱ�ȭ �ϱ� ���� GLFWwindow(render context)�� �ݵ�� �ʿ���
		GLFWwindow* pWindow = glfwCreateWindow(1, 1, "dummy", nullptr, nullptr);

		if (!pWindow)
		{
			releaseGL();
			return false;
		}

		glfwMakeContextCurrent(pWindow);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			releaseGL();
			return false;
		}

		glfwDestroyWindow(pWindow);

		return true;
	}

	void GLInitializer::releaseGL()
	{
		glfwTerminate();
	}
}