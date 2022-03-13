#include <unistd.h>

#include <iostream>

#include "utils.hpp"
#include "daemon.hpp"

int
main() {
	if (geteuid() != 0) {
		std::cerr << "you must be root to run this program" << std::endl;
		return 1;
	}
	
	return setup_lock();
}