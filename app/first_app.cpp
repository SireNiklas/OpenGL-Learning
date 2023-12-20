#include "first_app.hpp"

namespace ogl {

	void FirstApp::run() {
		// Game Loop
		while (!oglWindow.shouldClose()) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			// LEGACY
			glBegin(GL_TRIANGLES);
			
			glVertex2f(0.0f, 0.5f);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.5f, -0.5f);
			
			glEnd();


			/* Swap front and back buffers */
			glfwSwapBuffers(oglWindow.window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
}