#ifndef DAEMON_HPP_
#define DAEMON_HPP_

#define LOCK_FILE "/var/lock/matt_daemon.lock"

#include <sys/epoll.h>

#include <string>

#include "tintin_reporter.hpp"

class Daemon {
	public:
		Tintin_reporter *logger;

	private:
		bool	__alive;

		int		__sock_fd;
		int		__epfd;

		struct epoll_event __ev;
		struct epoll_event __events[3];

	public:
		explicit Daemon(Tintin_reporter *logger);
		~Daemon();

		void	run();
		void	create_server();

	private:
		void	__action(std::string packet);
};

#endif  // DAEMON_HPP_
