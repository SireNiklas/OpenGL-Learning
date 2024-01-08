#pragma once

#include "ogl_renderer.hpp"

namespace ogl {
	class OglTexture {
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	public:
		OglTexture(const std::string& path);
		~OglTexture();

		// Texture Slots. Important.
		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	};
};