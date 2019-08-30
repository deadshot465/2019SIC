#include "Window.h"

int main(int argc, char* args[]) {

	ecc::GameWindow window("ECC 2019 SIC", 1024, 512, false);

	while (window.IsInit()) {
		window.Broadcast();
	}

	return 0;
}