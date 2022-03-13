#ifndef DAEMON_HPP_
#define DAEMON_HPP_

#define LOCK_FILE "/var/lock/matt_daemon.lock"

#include <sys/epoll.h>

#include "logger.hpp"

class Daemon
{
	public:
		Logger logger;

	private:
		bool	__alive;

		int		__sock_fd;
		int		__epfd;

		struct epoll_event __ev;
		struct epoll_event __events[3];

	public:
		Daemon();
		~Daemon();

		void	run();
		void	create_server();
};

#endif