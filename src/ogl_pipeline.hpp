#pragma once

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
} // namespace ogl