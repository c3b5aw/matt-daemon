#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>

#include "logger.hpp"

class Logger {
	private:
		std::string	__name;

	public:
		Logger();
		Logger(std::string name);

		~Logger();

		void	log(std::string msg);
		void	info(std::string msg);
		void	debug(std::string msg);
		void	warning(std::string msg);
		void	error(std::string msg);
};

#endif