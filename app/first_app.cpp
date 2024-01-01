#include "first_app.hpp"
#include "../src/ogl_pipeline.hpp"

namespace ogl {
	// Shader Parser and loader End
	// Application Start
	void FirstApp::run() {
		// Vertex Buffer | Move this to something like ogl_pipeline.cpp & .hpp.
		float positions[] = { // This is the data we pass through glBufferData.
			0.5f, -0.5f, // 0
		   -0.5f, -0.5f, // 1
			0.5f,  0.5f, // 2
		   -0.5f,  0.5f, // 3
		};

		// Index buffer | Did this in my Godot test
		unsigned int indicies[] = {
			1, 0, 2, // The order here matters, depending on the order of your vertex positions! DON'T FORGET!
			2, 3, 1,
		};

		OglVertexArray va;
		OglVertexBuffer vb(positions, 4 * 2 * sizeof(float));
		OglIndexBuffer ib(indicies, 6);

		OglVertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		OglShader shader("D:/C++ Projects/OpenGL Learning/res/shaders/simple_shader.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
		

		// Unbound
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		float b = 0.0f;
		float increment = 0.05f;

		// Game Loop
		while (!oglWindow.shouldClose()) {
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			// Draw Req
			shader.Bind();
			shader.SetUniform4f("u_Color", 0.0f, 1.0f, b, 1.0f);

			va.Bind();
			ib.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (b > 1.0f)
				increment = -0.05f;
			else if (b < 0.0f)
				increment = 0.05f;

			b += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(oglWindow.window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
}