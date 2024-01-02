#pragma once

#include "ogl_pipeline.hpp"

namespace ogl {

	class OglRenderer {
	public:
		void Draw(const OglVertexArray& va, const OglIndexBuffer& ib, const OglShader& shader) const;
		void Clear() const;
	};
} // namespace ogl