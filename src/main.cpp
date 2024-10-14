#include "first_app.hpp"

// std
#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
	oys::FirstApp app{};

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}