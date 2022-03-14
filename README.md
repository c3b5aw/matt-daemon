# matt-daemon

@42School Project

Code a daemon

**Note:** ../100

[![build](https://github.com/c3b5aw/matt-daemon/actions/workflows/build.yml/badge.svg)](https://github.com/c3b5aw/matt-daemon/actions/workflows/build.yml)
[![cpplint](https://github.com/c3b5aw/matt-daemon/actions/workflows/lint.yml/badge.svg)](https://github.com/c3b5aw/matt-daemon/actions/workflows/lint.yml)

## Author

- [@c3b5aw](https://www.github.com/c3b5aw)

## Features - from subject

- [x] Require root rights
- [x] Daemonize
- [x] Listen on port 4242
- [x] Tintin_reporter for journalization
- [x] Log to /var/log/matt-daemon.log
	- [x] [BONUS] Report system using archive
- [x] Only one instance of the daemon is allowed to run
- [x] matt_daemon.lock in /var/lock
- [x] Error message on matt_daemon.lock file when attempting to start second daemon
- [x] when matt_daemon stops, remove matt_daemon.lock
- [x] close with quit command
- [x] any other input must be logged
- [x] max 3 clients at the same time
- [x] when matt_daemon receive a signal, it must be logged