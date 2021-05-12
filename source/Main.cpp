#include "App.h"

int main() {
	if (!app::init()) {
		system("pause");
		return -1;
	}

	app::run();
	app::exit();

	return 0;
}