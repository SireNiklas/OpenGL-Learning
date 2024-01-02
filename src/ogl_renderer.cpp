#include "ogl_renderer.hpp"
#include <iostream>

namespace ogl {

#pragma region Renderer
	void OglRenderer::Draw(const OglVertexArray& va, const OglIndexBuffer& ib, const OglShader& shader) const {
		shader.Bind();
		va.Bind();
		ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // GL_UNSIGNED_INT cause we aren't using any other, BUT I can change it or even grab it depending on what I choose to do.
	}
	void OglRenderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	#pragma endregion


}