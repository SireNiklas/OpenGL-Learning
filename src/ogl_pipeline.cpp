#include "ogl_pipeline.hpp"

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
}