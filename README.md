# CPPND: Paranoia -- a process monitoring tool for Linux systems

Paranoia is a small footprint tool which can help the paranoid audit events related to process creation / destruction on their
Linux systems. Paranoia works by using the kernel's netlink connector mechanism to subscribe to events and output them to stdout
in a formatted way for easy consumption. You could, for instance, use Paranoia to log such events to disk, an Elasticsearch cluster,
or another storage device, for auditing purposes. You could also use Paranoia as an education means for understanding what goes on
under the hood when you type different commands into your shell; or when you execute different applications.

## Supported Operating Sytems
At present, Paranoia is only supported on Linux systems.

## Dependencies for Running Locally
* cmake >= 3.16
  * [click here for installation instructions](https://cmake.org/install/)
* make >= 4.2
  * make is installed by default on most Linux distros
* gcc/g++ >= 9.3
  * gcc / g++ is installed by default on most Linux distros
* nlohmann/json >= 3.7.3
  * available on [github](https://github.com/nlohmann/json) (automatically 
  downloaded using cmake's `FetchContent` -- see instructions below)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Be paranoid: `./paranoia` (requires access to create raw sockets -- you may need to be root)
                          
## Type of events monitored
Presently, the following proc event types:
- exec
- fork
- exit

## Output formats supported
Two output formts are available for your convenience. `text` is the default,
and mostly useful for human consumption.
With the following environment variable set, the output will be formatted in `json`:
```
PARANOIA_OUTPUT_FORMAT=json
```

## Coming soon
- Tests!
- Support for more event types (e.g., ptrace, sid)

## Related material
- https://bewareofgeek.livejournal.com/2945.html
- https://www.kernel.org/doc/html/latest/driver-api/connector.html
