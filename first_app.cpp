#include "first_app.hpp"

namespace oys {

	void FirstApp::run() {
		while (!oysWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
} // namespace oys