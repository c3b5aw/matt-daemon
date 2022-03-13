#include <unistd.h>

#include <iostream>

#include "daemon.hpp"
#include "logger.hpp"

Daemon::Daemon() {
	logger = Logger("Matt_daemon");
}
Daemon::~Daemon() {}

void
Daemon::run() {
	;
}