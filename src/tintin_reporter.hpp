#ifndef TINTIN_REPORTER_HPP_
#define TINTIN_REPORTER_HPP_

#define LOG_FOLDER "/var/log/matt_daemon/"

#include <string>

#include "tintin_reporter.hpp"

class Tintin_reporter {
	private:
		std::string	__name;

	public:
		Tintin_reporter();
		explicit Tintin_reporter(std::string name);

		~Tintin_reporter();

		void	log(std::string msg);
		void	info(std::string msg);
		void	debug(std::string msg);
		void	warning(std::string msg);
		void	error(std::string msg);

	private:
		void	__format(std::string msg, std::string level);
		void	__register_entry(std::string entry, std::string path);
		int		__mkdirs(std::string path);
};

#endif  // TINTIN_REPORTER_HPP_
