#ifndef DAEMON_HPP_
#define DAEMON_HPP_

#define LOCK_FILE "/var/log/matt_daemon.lock"

#include "logger.hpp"

class Daemon
{
	public:
		Logger logger;
	
	public:
		Daemon();
		~Daemon();

		void	run();
		void	signal_handler(int signum);

};

#endif