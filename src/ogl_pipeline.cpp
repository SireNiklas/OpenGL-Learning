#include "ogl_pipeline.hpp"

// std
#include <iostream>
#include <fstream>
#include <sstream>

namespace ogl {
#pragma region Vertex Buffer
	// Vertex Buffer
	OglVertexBuffer::OglVertexBuffer(const void* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &m_RendererID)); // Has the reference of the pointer because OpenGL works as a state machine?
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		// 6 * sizeof(float) is defining the size of the data {positions} in bytes.
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Static vs Dynamic | Static draw once, Dynamic draw many types.
	}

	OglVertexBuffer::~OglVertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OglVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OglVertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
#pragma endregion

#pragma region Index Buffer
	OglIndexBuffer::OglIndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
	{
		// Have to be very careful, the size may make or break.
		ASSERT(sizeof(unsigned int) == sizeof(GLuint));

		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	OglIndexBuffer::~OglIndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OglIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

	}

	void OglIndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	}
#pragma endregion

#pragma region Vertext Array Object
	OglVertexArray::OglVertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	OglVertexArray::~OglVertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void OglVertexArray::AddBuffer(const OglVertexBuffer& vb, const OglVertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			offset += element.count * OglVertexBufferElement::GetSizeOfType(element.type);
		}
	}
	void OglVertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void OglVertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
#pragma endregion

#pragma region Shader
	OglShader::OglShader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
	{

		ShaderProgramSource source = ParseShader(filepath);
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	OglShader::~OglShader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}

	unsigned int OglShader::CompileShader(unsigned int type, const std::string& source) {
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		// Error Handling
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE) {
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)_malloca(length * sizeof(char)); // alloca allocates on the stack not the heap.
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			throw std::runtime_error("Failed to compile " + std::string((type == GL_VERTEX_SHADER ? "vertex" : "fragment")) + " shader!");
			std::cout << message << std::endl;
			GLCall(glDeleteShader(id));
			return EXIT_FAILURE;
		}

		return id;
	}

	ShaderProgramSource OglShader::ParseShader(const std::string& filepath) {
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

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int OglShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
		GLCall(unsigned int program = glCreateProgram());
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}

	void OglShader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void OglShader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void OglShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}	
	
	void OglShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void OglShader::SetUniform1f(const std::string& name, float value)
	{
		GLCall(glUniform1f(GetUniformLocation(name), value));
	}

	void OglShader::SetUniform1i(const std::string& name, int value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	int OglShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
			std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

		m_UniformLocationCache[name] = location;
		return location;
	}
#pragma endregion

}