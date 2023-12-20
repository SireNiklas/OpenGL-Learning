#include "first_app.hpp"

namespace ogl {

	void FirstApp::run() {

		// Vertex Buffer | Move this to something like ogl_pipeline.cpp & .hpp.
		float positions[6] = { // This is the data we pass through glBufferData.
			0.0f,  0.5f,
		   -0.5f, -0.5f,
			0.5f, -0.5f
		};

		GLuint buffer; // GL Unsigned int, special to GLFW. | Defined the size indepently of the platform it is running on.
		glGenBuffers(1, &buffer); // Has the reference of the pointer because OpenGL works as a state machine?
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		// 6 * sizeof(float) is defining the size of the data {positions} in bytes.
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // Static vs Dynamic | Static draw once, Dynamic draw many types.
		
		// OpenGL doesn't know how the read the data we gave it.

		// Game Loop
		while (!oglWindow.shouldClose()) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			//glDrawArrays(GL_TRIANGLES, 0, 3); // 0 = where we start, 3 = vertecies. | The current bound buffer is going to be used here, the one we defined above.

			// LEGACY | 
			//glBegin(GL_TRIANGLES);
			//glVertex2f(0.0f, 0.5f);
			//glVertex2f(-0.5f, -0.5f);
			//glVertex2f(0.5f, -0.5f);
			//glEnd();

			/* Swap front and back buffers */
			glfwSwapBuffers(oglWindow.window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
}