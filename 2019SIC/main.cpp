#include "Window.h"

int main(int argc, char* args[]) {
	ecc::GameWindow window("ECC 2019 SIC", 1728, 640, false);

	while (window.IsInit()) {
		window.Broadcast();
	}

	return 0;
}