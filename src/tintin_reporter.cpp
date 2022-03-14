#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include "tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter() : __name("Matt_daemon") {
	info("Started.");
}

Tintin_reporter::Tintin_reporter(std::string name) {
	__name = name;
}

Tintin_reporter::~Tintin_reporter()
{}

void
Tintin_reporter::log(std::string msg)  {
	return __format(msg, "LOG");
}

void
Tintin_reporter::info(std::string msg) {
	return __format(msg, "INFO");
}

void
Tintin_reporter::debug(std::string msg) {
	return __format(msg, "DEBUG");
}

void
Tintin_reporter::warning(std::string msg) {
	return __format(msg, "WARNING");
}

void
Tintin_reporter::error(std::string msg) {
	return __format(msg, "ERROR");
}

void
Tintin_reporter::__format(std::string msg, std::string level) {
	std::string entry = "[";

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	/* Create oss for time string and osspath for path string */
	std::ostringstream oss, osspath;

	oss << std::put_time(&tm, "%d/%m/%Y-%H:%M:%S");
	osspath << std::put_time(&tm, "%Y/%m/%d");

	entry += oss.str() + "] [ " + level + " ] - " + __name + ": " + msg;

	__register_entry(entry, osspath.str());
}

int
Tintin_reporter::__mkdirs(std::string path) {
	/* Iterate through the path and create the missing directories */
	char buf[256];
	char *p = NULL;

	size_t len = snprintf(buf, sizeof(buf), "%s", path.c_str());
	if (len >= sizeof(buf)) {
		std::cerr << "Error: path too long" << std::endl;
		return 1;
	}

	for (p = buf + 1; *p; p++) {
		if (*p == '/') {
			*p = '\0';
			if (mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
				if (errno != EEXIST) {
					std::cerr << "Error: could not create directory" << std::endl;
					return 1;
				}
			}
			*p = '/';
		}
	}

	if (mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		if (errno != EEXIST) {
			std::cerr << "Error: could not create directory" << std::endl;
			return 1;
		}
	}

	return 0;
}

void
Tintin_reporter::__register_entry(std::string entry, std::string path) {
	struct stat buffer;
	std::string directory_path = (LOG_FOLDER + path.substr(0, path.size() - 2));
	if (stat(directory_path.c_str(), &buffer) == -1) {
		if (__mkdirs(directory_path) == 1)
			return;
	}

	std::string file_path = LOG_FOLDER + path + ".log";

	std::ofstream log_file(file_path,
							std::ios::app|std::ios::out|std::ios::binary);

	if (log_file.is_open()) {
		log_file << entry << std::endl;
		log_file.close();
	} else {
		std::cerr << "Error: Could not open log file." << file_path << std::endl;
	}
}
