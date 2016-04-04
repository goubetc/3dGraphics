#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "../include/practical_04_scene.hpp"
#include <sstream>

int main(int argc, char* argv[]) {
	Viewer viewer(1280, 720);

	initialize_practical_04_scene(viewer);

	while(viewer.isRunning()) {
		viewer.handleEvent();
		viewer.animate();
		viewer.draw();
		viewer.display();
	}

	return EXIT_SUCCESS;
}
