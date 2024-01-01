#include "ogl_pipeline.hpp"
#include "ogl_renderer.hpp"


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
}