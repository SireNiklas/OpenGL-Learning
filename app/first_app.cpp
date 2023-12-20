#include "first_app.hpp"

// std
#include <iostream>
#include <fstream>
#include <sstream>

namespace ogl {

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	static ShaderProgramSource ParseShader(const std::string& filepath) {
		std::ifstream stream(filepath);
		if (!stream)
			throw std::runtime_error("Failed to open file: " + filepath);

		std::cout << filepath << std::endl;

		enum class ShaderType {
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;

				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str()};
	}

	static unsigned int CompileShader(unsigned int type, const std::string& source) {
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// Error Handling
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char)); // alloca allocates on the stack not the heap.
			glGetShaderInfoLog(id, length, &length, message);
			throw std::runtime_error("Failed to compile " + std::string((type == GL_VERTEX_SHADER ? "vertex" : "fragment")) + " shader!");
			std::cout << message << std::endl;
			glDeleteShader(id);
			return EXIT_FAILURE;
		}

		return id;
	}

	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	void FirstApp::run() {
		// Vertex Buffer | Move this to something like ogl_pipeline.cpp & .hpp.
		float positions[6] = { // This is the data we pass through glBufferData.
			0.0f,  0.5f,
		   -0.5f, -0.5f,
			0.5f, -0.5f
		};

		unsigned int buffer; // GL Unsigned int, special to GLFW. | Defined the size indepently of the platform it is running on.
		glGenBuffers(1, &buffer); // Has the reference of the pointer because OpenGL works as a state machine?
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		// 6 * sizeof(float) is defining the size of the data {positions} in bytes.
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // Static vs Dynamic | Static draw once, Dynamic draw many types.

		glEnableVertexAttribArray(0);
		// Type is what type is the information, here we have float.
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		ShaderProgramSource source = ParseShader("D:/C++ Projects/OpenGL Learning/res/shaders/simple_shader.shader");

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader);

		// Game Loop
		while (!oglWindow.shouldClose()) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 3); // 0 = where we start, 3 = vertecies. | The current bound buffer is going to be used here, the one we defined above.

			// LEGACY
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

		glDeleteProgram(shader);
	}
}