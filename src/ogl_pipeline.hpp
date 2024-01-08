#pragma once
#include "ogl_errorhandler.hpp"

// GLM
#include "glm/gtc/matrix_transform.hpp"

// std
#include <vector>
#include <stdexcept>
#include <unordered_map>

namespace ogl {

	// Maybe Renamed to ogl_pipeline and OglPipeline.

#pragma region Vertex Buffer
	class OglVertexBuffer {
	private:
		unsigned int m_RendererID;
	public:
		// Make these there own Class.
		OglVertexBuffer(const void* data, unsigned int size);
		~OglVertexBuffer();

		void Bind() const;
		void Unbind() const;
	};
#pragma endregion

#pragma region Index Buffer
	class OglIndexBuffer {
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	public:
		// Make these there own Class.
		OglIndexBuffer(const unsigned int* data, unsigned int count);
		~OglIndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; }
	};
#pragma endregion

#pragma region VertexBufferLayout & Element
	struct OglVertexBufferElement {
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type) {
			switch (type) {
				default:				break;
				case GL_FLOAT:			return 4;
				case GL_UNSIGNED_INT:	return 4;
				case GL_UNSIGNED_BYTE:	return 1;
			}
			ASSERT(false);
			return 0;
		};
	};

	class OglVertexBufferLayout {
	private:
		std::vector<OglVertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		OglVertexBufferLayout() : m_Stride(0) {}

		template<typename T>
		void Push(unsigned int count) {
			std::runtime_error(false);
		}

		template<>
		void Push<float>(unsigned int count) {
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += count * OglVertexBufferElement::GetSizeOfType(GL_FLOAT); // Create function to convert size of GL type enum.
		}
		
		template<>
		void Push<unsigned int>(unsigned int count) {
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * OglVertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count; // Create function to convert size of GL type enum.
		}
		
		template<>
		void Push<unsigned char>(unsigned int count) {
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += count * OglVertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); // Create function to convert size of GL type enum.
		}

		inline const std::vector<OglVertexBufferElement> GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};
#pragma endregion

#pragma region Vertex Array Object
	class OglVertexArray {
	private:
		unsigned int m_RendererID;
	public:
		OglVertexArray();
		~OglVertexArray();

		void AddBuffer(const OglVertexBuffer& vb, const OglVertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;
	};
#pragma endregion

#pragma region Shader

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OglShader {
	private:
		std::string m_FilePath;
		unsigned int m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		OglShader(const std::string& filepath);
		~OglShader();

		void Bind() const;
		void Unbind() const;

		// Set Uniforms
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform1i(const std::string& name, int value);
	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);
		int GetUniformLocation(const std::string& name);
	};
#pragma endregion

} // namespace ogl