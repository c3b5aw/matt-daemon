#include <iostream>

#include "logger.hpp"

Logger::Logger() {
	__name = "Matt_daemon";
}

Logger::Logger(std::string name) {
	__name = name;
}

Logger::~Logger() {}

void
Logger::log(std::string msg) {
	std::cout << msg << std::endl;
}

void
Logger::info(std::string msg) {
	std::cout << msg << std::endl;
}

void
Logger::debug(std::string msg) {
	std::cout << msg << std::endl;
}

void
Logger::warning(std::string msg) {
	std::cerr << msg << std::endl;
}

void
Logger::error(std::string msg) {
	std::cerr << msg << std::endl;
}