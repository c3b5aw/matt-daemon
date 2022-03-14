# matt-daemon

@42School Project

Code a daemon

**Note:** ../100

[![build](https://github.com/c3b5aw/matt-daemon/actions/workflows/build.yml/badge.svg)](https://github.com/c3b5aw/matt-daemon/actions/workflows/build.yml)
[![cpplint](https://github.com/c3b5aw/matt-daemon/actions/workflows/lint.yml/badge.svg)](https://github.com/c3b5aw/matt-daemon/actions/workflows/lint.yml)

## Author

- [@c3b5aw](https://www.github.com/c3b5aw)

## Features - from subject

- [+] Require root rights
- [+] Daemonize
- [+] Listen on port 4242
- [+] Tintin_reporter for journalization
- [+] Log to /var/log/matt-daemon.log
	- [+] [BONUS] Report system using archive
- [+] Only one instance of the daemon is allowed to run
- [+] matt_daemon.lock in /var/lock
- [+] Error message on matt_daemon.lock file when attempting to start second daemon
- [+] when matt_daemon stops, remove matt_daemon.lock
- [+] close with quit command
- [+] any other input must be logged
- [+] max 3 clients at the same time
- [+] when matt_daemon receive a signal, it must be logged