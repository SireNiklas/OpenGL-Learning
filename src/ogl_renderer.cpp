#include "ogl_renderer.hpp"
#include <iostream>

namespace ogl {

	// Error Handling
	void GLClearError() {
		while (glGetError()); // While GL_Get_Error is not equal to GL_NO_ERROR otherwise known as 0.
	}

	bool GLLogCall(const char* function, const char* file, int line) { // Go about this in a smart way such as looking at the actual vlaue such as 0x0500 or 0x0501 and go down the number line sequentially or just what is important.
		while (GLenum error = glGetError()) {
			std::cout << "[OpenGL ERROR: INVALID ENUM (" << error << "): " << function << " " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}
}