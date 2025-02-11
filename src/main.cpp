#include "core/application.hpp"

int main() {
	oys::Application app;

	while (app.isActive()) {
		app.update();
	}
}