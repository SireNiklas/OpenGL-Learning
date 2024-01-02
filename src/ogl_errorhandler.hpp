#pragma once

#include "GL/glew.h"

// Error Handling Start
#define ASSERT(x) if (!(x)) __debugbreak(); // Macro, also uses MSVS internal break. | NOTENOTENOTE: GLMessageCallback is slightly more usefull, but not to big of an issue.

#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))	
#else
#define GLCall(x) x
#endif

namespace ogl {

	// Error Handling
	void GLClearError();
	bool GLLogCall(const char* function, const char* file, int line);
} // namespace ogl

