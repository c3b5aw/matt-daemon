# matt-daemon

- Require root rights
- Daemonize
- Listen on port 4242
- Tintin_reporter for journalization
- Log to /var/log/matt-daemon.log - Report system is archiving
- Only one instance of the daemon is allowed to run
- matt_daemon.lock in /var/lock
- Error message on matt_daemon.lock file when attempting to start second daemon
- when matt_daemon stops, remove matt_daemon.lock
- close with quit command
- any other input must be logged
- max 3 clients at the same time
- when matt_daemon receive a signal, it must be logged