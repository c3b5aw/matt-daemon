#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>

#include <string>
#include <csignal>
#include <iostream>

#include "utils.hpp"
#include "daemon.hpp"

Tintin_reporter *logger_singleton = nullptr;

int
open_lock() {
	/* Open for read/write by owner only (root) */
	int fd = open(LOCK_FILE, O_RDWR | O_CREAT);
	if (fd == -1)
		std::cerr << "Open lock file failed" << std::endl;

	return fd;
}

int
setup_lock() {
	Tintin_reporter logger("Matt_daemon");
	logger_singleton = &logger;

	int fd = open_lock();
	if (fd == -1)
		return 1;

	/*
	 *	Place an exclusive non blocking lock
	 *		- https://linux.die.net/man/2/flock
	*/
	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		close(fd);
		logger.error("Error file locked.");
		std::cerr << "Can't open :" << LOCK_FILE << std::endl;
		return 1;
	}

	daemonize(&logger, fd);
	return 0;
}

void
release_lock(int lock_fd) {
	if (flock(lock_fd, LOCK_UN) == -1) {
		std::cerr << "flock: " << strerror(errno) << std::endl;
		close(lock_fd);
		exit(EXIT_FAILURE);
	}

	close(lock_fd);

	if (unlink(LOCK_FILE) == -1) {
		std::cerr << "unlink: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void
daemonize(Tintin_reporter *logger, int lock_fd) {
	Daemon daemon(logger);
	daemon.create_server();

	/*
	 *	Linux Daemon Writing HOWTO
	 *		http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
	*/

	logger->info("Entering daemon mode.");

	pid_t pid = fork();
	if (pid < 0) {
		std::cerr << "fork failed" << std::endl;
		release_lock(lock_fd);
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	/* Create a new S(ession)ID for the child process, prevent orphan */
	pid_t sid = setsid();
	if (sid < 0) {
		std::cerr << "setsid failed" << std::endl;
		release_lock(lock_fd);
		exit(EXIT_FAILURE);
	}

	chdir("/");

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	logger->info("started. PID: " + std::to_string(getpid()) + ".");

	/*
	 *	Set up commons signal handlers
	 *		- https://www.tutorialspoint.com/unix/unix-signals-traps.htm
	*/
	signal(SIGHUP, sig_handler);
	signal(SIGINT,  sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGFPE, sig_handler);
	signal(SIGALRM, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2, sig_handler);

	daemon.run();

	logger->info("Quitting.");
	release_lock(lock_fd);
}

void
sig_handler(int signum) {
	if (logger_singleton != NULL) {
		std::string signal_name = "";
		switch (signum) {
			case SIGHUP:
				signal_name = "SIGHUP";
				break;
			case SIGINT:
				signal_name = "SIGINT";
				break;
			case SIGQUIT:
				signal_name = "SIGQUIT";
				break;
			case SIGFPE:
				signal_name = "SIGFPE";
				break;
			case SIGALRM:
				signal_name = "SIGALRM";
				break;
			case SIGTERM:
				signal_name = "SIGTERM";
				break;
			case SIGUSR1:
				signal_name = "SIGUSR1";
				break;
			case SIGUSR2:
				signal_name = "SIGUSR2";
				break;
			default:
				signal_name = "UNKNOWN";
				break;
		}
		logger_singleton->info("[SIGNAL] " + signal_name);
	}
}
