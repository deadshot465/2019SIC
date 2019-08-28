#include "Window.h"

int main(int argc, char* args[]) {

	ecc::GameWindow window("ECC 2019 SIC", 512, 256, false);

	while (window.IsInit()) {
		window.Broadcast();
	}

	return 0;
}