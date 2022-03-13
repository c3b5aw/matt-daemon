#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>

#include <csignal>
#include <iostream>

#include "utils.hpp"
#include "daemon.hpp"

int
open_lock() {
	/* Open for read/write by owner only (root) */
	int fd = open(LOCK_FILE, O_RDWR | O_CREAT);
	if (fd == -1)
		std::cerr << "open lock file failed" << std::endl;
	return fd;
}

int
setup_lock() {
	int fd = open_lock();
	if (fd == -1)
		return 1;

	/*
	 *	Place an exclusive non blocking lock
	 *		- https://linux.die.net/man/2/flock
	*/
	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		std::cerr << "could not lock lock file." << std::endl;
		return 1;
	}

	daemonize();
	return 0;
}

void
release_lock(bool exit_on_success) {
	int fd = open_lock();
	if (fd == -1)
		return;

	if (flock(fd, LOCK_UN) == -1) {
		std::cerr << "flock: " << strerror(errno) << std::endl;
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);

	if (unlink(LOCK_FILE) == -1) {
		std::cerr << "unlink: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (exit_on_success)
		exit(EXIT_SUCCESS);
}

Logger *logger_singleton = NULL;

void
sig_handler(int signum) {
	if (logger_singleton != NULL)
		logger_singleton->info("signal received " + std::to_string(signum));
}

void
daemonize() {
	/*
	 *	Linux Daemon Writing HOWTO
	 *		http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
	*/

	// pid_t pid = fork();
	// if (pid < 0) {
	// 	std::cerr << "fork failed" << std::endl;
	// 	release_lock(false);
	// 	exit(EXIT_FAILURE);
	// } else if (pid > 0) {
	// 	exit(EXIT_SUCCESS);
	// }

	// umask(0);

	// /* Create a new S(ession)ID for the child process, prevent orphan */
	// pid_t sid = setsid();
	// if (sid < 0) {
	// 	std::cerr << "setsid failed" << std::endl;
	// 	release_lock(false);
	// 	exit(EXIT_FAILURE);
	// }

	// chdir("/");

	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
	// close(STDERR_FILENO);

	Daemon daemon;

	logger_singleton = &daemon.logger;
	/*
	 *	Set up commons signal handlers
	 *		- https://www.tutorialspoint.com/unix/unix-signals-traps.htm
	*/
	signal(SIGHUP, sig_handler);
	signal(SIGINT,  sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGFPE, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(SIGALRM, sig_handler);

	daemon.create_server();
	daemon.run();
}