#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

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
	return __format(msg, "LOG");
}

void
Logger::info(std::string msg) {
	return __format(msg, "INFO");
}

void
Logger::debug(std::string msg) {
	return __format(msg, "DEBUG");
}

void
Logger::warning(std::string msg) {
	return __format(msg, "WARNING");
}

void
Logger::error(std::string msg) {
	return __format(msg, "ERROR");
}

void
Logger::__format(std::string msg, std::string level) {
	std::string entry = "[";

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");

	entry += oss.str() + "] [" + level + "] - " + __name + ": " + msg + ".";

	if (level == "WARNING" || level == "ERROR") {
		std::cerr << entry << std::endl;
	} else {
		std::cout << entry << std::endl;
	}
}