#include "ogl_texture.hpp"

#include "stb_image/stb_image.h"

namespace ogl {
	OglTexture::OglTexture(const std::string& path) : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Height(0), m_BPP(0)
	{
		// Flip upside down to start at bottom left | BOTTOM LEFT is (0,0) or ORIGIN.
		stbi_set_flip_vertically_on_load(1); // Depends on the texture you use.
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		// Display SETTINGS: These four must be specified, the are the defaults. | glTextureParameteri and glTexParameteri ARE VERY DIFFERENT!!!
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		// GL_RGBA8 is deprecated but will work, should specify the actual size.
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	OglTexture::~OglTexture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void OglTexture::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void OglTexture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	}
};
