#include "../src/ogl_window.hpp"

namespace ogl {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		// Create window using constructor
		OglWindow oglWindow{WIDTH, HEIGHT, "Hello OpenGL"};

	};
} // namespace ogl