#include "Window.h"

int main(int argc, char* args[]) {
	//1728, 640
	ecc::GameWindow window("ECC 2019 SIC", 1152, 810, false);

	while (window.IsInit()) {
		window.Broadcast();
	}

	return 0;
}