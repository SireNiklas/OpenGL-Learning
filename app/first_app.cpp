#include "first_app.hpp"
#include "../src/ogl_renderer.hpp"
#include "../src/ogl_texture.hpp"

namespace ogl {
	// Shader Parser and loader End
	// Application Start
	void FirstApp::run() {
		// Vertex Buffer | Move this to something like ogl_pipeline.cpp & .hpp.
		float positions[] = { // This is the data we pass through glBufferData.
			0.5f, -0.5f, 1.0f, 0.0f, // 0
		   -0.5f, -0.5f, 0.0f, 0.0f, // 1
			0.5f,  0.5f, 1.0f, 1.0f, // 2
		   -0.5f,  0.5f, 0.0f, 1.0f // 3
		};

		// Index buffer | Did this in my Godot test
		unsigned int indicies[] = {
			1, 0, 2, // The order here matters, depending on the order of your vertex positions! DON'T FORGET!
			2, 3, 1,
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		OglVertexArray va;
		OglVertexBuffer vb(positions, 4 * 4 * sizeof(float));
		OglIndexBuffer ib(indicies, 6);

		OglVertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		OglShader shader("D:/C++ Projects/OpenGL Learning/res/shaders/simple_shader.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);

		OglTexture texture("D:/C++ Projects/OpenGL Learning/res/texture/wall.jpg");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		// Unbound
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		OglRenderer renderer;

		float b = 0.0f;
		float increment = 0.05f;

#pragma region Game Loop
		while (!oglWindow.shouldClose()) {
			/* Render here */
			renderer.Clear();

			// Draw Req
			shader.Bind();
			shader.SetUniform4f("u_Color", 0.0f, 1.0f, b, 1.0f); // This should tech

			renderer.Draw(va, ib, shader);

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
#pragma endregion

	}
}