#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "tintin_reporter.hpp"

int		open_lock();
int		setup_lock();

void	release_lock(int lock_fd);
void	daemonize(Tintin_reporter *logger, int lock_fd);
void	sig_handler(int signum);

#endif  // UTILS_HPP_
