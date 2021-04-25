#include "App.h"

int main() {
	if (!app::init())
		return -1;

	app::run();
	app::exit();

	return 0;
}