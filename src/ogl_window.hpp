#include<GL/glew.h>
#include <GLFW/glfw3.h>

// std
#include <string>

namespace ogl {
	class OglWindow {
	public:
		OglWindow(int w, int h, std::string name);
		~OglWindow();

		OglWindow(const OglWindow&) = delete;
		OglWindow& operator=(const OglWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }

		GLFWwindow* window;

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
	};
} // namespace ogl