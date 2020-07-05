# Paranoia: a process monitoring tool for Linux systems

Paranoia is a small footprint tool which can help the paranoid audit events related to process creation / destruction on their
Linux systems. Paranoia works by using the kernel's netlink connector mechanism to subscribe to events and output them to stdout
in a formatted way for easy consumption. You could, for instance, use Paranoia to log such events to disk, an Elasticsearch cluster,
or another storage device, for auditing purposes. You could also use Paranoia as an educational means for understanding what goes on
under the hood when you type different commands into your shell, or when you execute different applications.

## Supported Operating Sytems
At present, Paranoia is only supported on Linux systems.

## Dependencies for Running Locally
* A recent linux kernel (at least 4.18)
* `cmake >= 3.16`
  * [Click here for installation instructions](https://cmake.org/install/).
* `make >= 4.2`
  * `make` is installed by default on most Linux distros. Checkout your distro documentation for
  more information.
* `gcc >= 9.3`
  * `gcc` is installed by default on most Linux distros. Check out your distro documentation 
  for more information.
* `nlohmann/json >= 3.7.3`
  * Automatically downloaded using cmake's `FetchContent` -- see instructions [below](#basic-build-instructions).
  * [nlohmann/json](https://github.com/nlohmann/json) source code is also available on github.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Be paranoid: `./paranoia` (you may need to be root)
                          
## Type of events monitored
Presently, visibility is provided into the following proc event types:
- exec
- fork
- exit

## Output formats supported
Two output formats are available for your convenience. `text` is the default;
it is mostly useful for human consumption.

With the following environment variable set, the output will be formatted as `json`:
```
PARANOIA_OUTPUT_FORMAT=json
```

### Example of output formatted as `text`:
```
2020-07-05 03:26:32 UTC : FORK → [ child_cmd:"-bash" child_pid:"72211" child_tgid:"72211" parent_cmd:"-bash" parent_pid:"71823" parent_tgid:"71823" ]
2020-07-05 03:26:32 UTC : EXEC → [ cmd:"sleep 10" pid:"72211" tgid:"72211" ]
2020-07-05 03:26:42 UTC : EXIT → [ cmd:"sleep 10" exit_code:"0" exit_signal:"17" parent_cmd:"-bash" parent_pid:"71823" parent_tgid:"71823" pid:"72211" tgid:"72211" ]
```

### Example of output formatted as `json`:
```
{"child_cmd":"-bash","child_pid":"72046","child_tgid":"72046","parent_cmd":"-bash","parent_pid":"71823","parent_tgid":"71823","timestamp":"1593916602","type":"fork"}
{"cmd":"sleep 10","pid":"72046","tgid":"72046","timestamp":"1593916602","type":"exec"}
{"cmd":"sleep 10","exit_code":"0","exit_signal":"17","parent_cmd":"-bash","parent_pid":"71823","parent_tgid":"71823","pid":"72046","tgid":"72046","timestamp":"1593916612","type":"exit"}
```

## Coming soon
- Tests!
- Support for more event types (e.g., ptrace, sid)

## Licence
This project is licensed under the [MIT License](LICENSE).

## Related material
- https://bewareofgeek.livejournal.com/2945.html
- https://www.kernel.org/doc/html/latest/driver-api/connector.html
