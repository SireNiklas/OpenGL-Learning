#include "first_app.hpp"

// Engine
#include "../src/ogl_renderer.hpp"
#include "../src/ogl_texture.hpp"

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace ogl {
	// Shader Parser and loader End
	// Application Start
	void FirstApp::run() {
		// Vertex Buffer | Move this to something like ogl_pipeline.cpp & .hpp.
		float positions[] = { // This is the data we pass through glBufferData.
			0.5f, -0.5f, 1.0f, 0.0f, // 0
		   -0.5f, -0.5f, 0.0f, 0.0f, // 1
			0.5f,  0.5f, 1.0f, 1.0f, // 2
		   -0.5f,  0.5f, 0.0f, 1.0f  // 3
		};

		// Index buffer | Did this in my Godot test
		unsigned int indicies[] = {
			1, 0, 2, // The order here matters, depending on the order of your vertex positions! DON'T FORGET!
			2, 3, 1,
		};

		// By default GL uses GLBlendEquation: GL_FUNC_ADD if no blend function is set.
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // src = GL_SRC_ALPHA, dest = GL_ONE_MINUS_SRC_ALPHA | src alpha = 0 | dest = 1 - src(0) = 1 | Blending is IMPORTANT to more research into it!
		GLCall(glEnable(GL_BLEND)); // Disabled by default in OpenGL

		OglVertexArray va;
		OglVertexBuffer vb(positions, 4 * 4 * sizeof(float));

		OglVertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		OglIndexBuffer ib(indicies, 6);

		// Creating a "view".
		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f); // This has a 4x3. (Fits our current window) || Math is cool?

		OglShader shader("D:/C++ Projects/OpenGL Learning/res/shaders/simple_shader.shader");
		shader.Bind();

		shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
		shader.SetUniformMat4f("u_MVP", proj);

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