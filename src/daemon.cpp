#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>

#include "daemon.hpp"
#include "tintin_reporter.hpp"

Daemon::Daemon(Tintin_reporter *logger)
:	logger(logger),
	__alive(true),
	__sock_fd(0) {
	memset(&__ev, 0, sizeof(__ev));
	for (int i = 0; i < 3; i++) {
		memset(&__events[i], 0, sizeof(__events[i]));
	}
}

Daemon::~Daemon() {
	for (int i = 0; i < 3; i++) {
		if (__events[i].data.fd > 0)
			close(__events[i].data.fd);
	}

	if (__sock_fd > 0)
		close(__sock_fd);

	if (__epfd > 0)
		close(__epfd);
}

void
Daemon::create_server() {
	// https://man7.org/linux/man-pages/man7/unix.7.html
	logger->info("Creating server.");

	/* Setup socket and set non blocking I/O */
	__sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (__sock_fd < 0)
		return logger->error("Failed to create socket.");
	fcntl(__sock_fd, F_SETFL, O_NONBLOCK);

	/* Set reuse address */
	int _true = 1;
	setsockopt(__sock_fd, SOL_SOCKET, SO_REUSEADDR, &_true, sizeof(_true));


	/* Parameter for bind() */
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(4242);
	addr.sin_addr.s_addr = INADDR_ANY;

	/*	Bind addr */
	if (bind(__sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return logger->error("Failed to bind socket.");

	/* Setup listener */
	if (listen(__sock_fd, 5) < 0)
		return logger->error("Failed to listen on socket.");

	/* Epoll setup */
	__epfd = epoll_create1(0);

	__ev.data.fd = __sock_fd;
	__ev.events = EPOLLIN;
	epoll_ctl(__epfd, EPOLL_CTL_ADD, __sock_fd, &__ev);

	logger->info("Server created.");
}

void
Daemon::run() {
	int nfds;
	int max_events = 10;
	int num_connections = 0;

	while (__alive) {
		nfds = epoll_wait(__epfd, __events, max_events, 0);
		for (int i = 0; i < nfds; i++) {
			if (__events[i].data.fd == __sock_fd) {
				struct sockaddr_in addr;
				socklen_t addrlen = sizeof(addr);

				int conn_fd = accept(__sock_fd, (struct sockaddr *)&addr, &addrlen);
				if (conn_fd < 0) {
					logger->error("Failed to accept connection.");
					continue;
				}

				if (num_connections >= 3) {
					logger->warning("Rejecting connection, too many connections.");
					close(conn_fd);
					continue;
				}

				num_connections++;
				fcntl(conn_fd, F_SETFL, O_NONBLOCK);

				__ev.data.fd = conn_fd;
				__ev.events = EPOLLIN;
				epoll_ctl(__epfd, EPOLL_CTL_ADD, conn_fd, &__ev);
			} else if (__events[i].events & EPOLLIN) {
				char buf[1024];
				int n = read(__events[i].data.fd, buf, sizeof(buf));
				if (n < 0) {
					logger->error("Failed to read from socket.");
				} else if (n == 0) {
					epoll_ctl(__epfd, EPOLL_CTL_DEL, __events[i].data.fd, NULL);
					close(__events[i].data.fd);
					num_connections--;
				} else {
					buf[n] = '\0';
					if (buf[n - 1] == '\n')
						buf[n - 1] = '\0';

					__action(std::string(buf));

					__ev.events = EPOLLOUT;
					__ev.data.fd = __events[i].data.fd;
					epoll_ctl(__epfd, EPOLL_CTL_MOD, __events[i].data.fd, &__ev);
				}
			} else if (__events[i].events & EPOLLOUT) {
				std::string response = "Matt_daemon: Packet successfully treated.\n";

				send(__events[i].data.fd, response.c_str(), response.size(), 0);

				__ev.events = EPOLLIN;
				__ev.data.fd = __events[i].data.fd;

				epoll_ctl(__epfd, EPOLL_CTL_MOD, __events[i].data.fd, &__ev);
			} else if (__events[i].events & EPOLLHUP) {
				epoll_ctl(__epfd, EPOLL_CTL_DEL, __events[i].data.fd, NULL);
				close(__events[i].data.fd);
				num_connections--;
			} else {
				logger->error("Unknown epoll() event.");
			}
		}
	}
}

void
Daemon::__action(std::string packet) {
	if (packet == "quit") {
		return stop();
	}

	logger->log("User input: " + packet);
}

void
Daemon::stop() {
    logger->info("Request quit.");
    __alive = false;
}
