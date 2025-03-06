# pastel-base

Some goodies to use as base for pastel projects.

## mk

`config.mk` - base Makefile configuration for pastel projects, as some useful functions.
`embed.mk` - allow embedding files into a binary

## pl-log

A simple single-header C logging library.

Usage:

```c
void pl_init();
void pl_close();

void pl_set_level(pl_Level level);    // PL_TRACE_LEVEL, PL_DEBUG_LEVEL, PL_INFO_AND_UP (default)

PL_TRACE("Shows only if env PL_LEVEL=trace is set. Prints in faded gray.");
PL_DEBUG("Shows only if env PL_LEVEL=debug or above is set. Prints in brown.");
PL_INFO("Always shows. Prints in terminal color. All messages can use printf %s style tags.");
PL_WARNING("Always shows. Prints in magenta.");
PL_ERROR("Always shows. Prints in red. Returns from current function.");
PL_ERROR_RET(127, "Always shows. Prints in red. Returns from current function with an error code.");
PL_ABORT();  // Used for programming errors. Always abort.

const char* pl_last_error();    // returns the last error message (warning, error or abort message)

void pl_set_warning_callback(void (*cb)());
void pl_set_error_callback(void (*cb)());
void pl_set_abort_callback(void (*cb)());
```

The following env variables define the output style:

```bash
PL_LEVEL=trace|debug       # set default debug level (empty by default)
PL_COLOR=0                 # disable colors (default 1)
PL_LEVELNAME=0             # don't start the message with the message level (default 1)
PL_FILE=stdout|stderr|myfile.log  # send log to file (default stderr)
```
