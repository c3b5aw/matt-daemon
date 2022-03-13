#ifndef UTILS_HPP_
#define UTILS_HPP_

int		open_lock();
int		setup_lock();

void	release_lock(bool exit_on_success);
void	daemonize();

#endif