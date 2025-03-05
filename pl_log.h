#ifndef PL_LOG_H
#define PL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>

typedef enum pl_Level { PL_TRACE_LEVEL = 2, PL_DEBUG_LEVEL = 1, PL_INFO_AND_UP = 0 } pl_Level;

extern void        pl_init();
extern void        pl_close();
extern pl_Level    pl_level();
extern void        pl_set_level(pl_Level level);
extern const char* pl_last_error();
extern void        pl_set_warning_callback(void (*cb)(void* data), void* data);
extern void        pl_set_error_callback(void (*cb)(void* data), void* data);
extern void        pl_set_abort_callback(void (*cb)(void* data), void* data);

extern void pl_log(pl_Level level, const char* color, const char* lvl, bool copy_error, const char* msg, ...) __attribute__((format(printf, 5, 6)));

extern void call_warning_callback();
extern void call_error_callback();
extern void call_abort_callback();

#define PL_TRACE(msg, ...) { pl_log(PL_TRACE_LEVEL, "\e[90m", "TRACE", false, msg, ##__VA_ARGS__); }
#define PL_DEBUG(msg, ...) { pl_log(PL_DEBUG_LEVEL, "\e[33m", "DEBUG", false, msg, ##__VA_ARGS__); }
#define PL_INFO(msg, ...) { pl_log(PL_INFO_AND_UP, "\e[0m", "INFO ", false, msg, ##__VA_ARGS__); }
#define PL_WARNING(msg, ...) { pl_log(PL_INFO_AND_UP, "\e[95m", "WARN ", false, msg, ##__VA_ARGS__); call_warning_callbacl(); }
#define PL_ERROR(msg, ...) { pl_log(PL_INFO_AND_UP, "\e[91m", "ERROR", true, msg, ##__VA_ARGS__); call_error_callback(); return; }
#define PL_ERROR_RET(code, msg, ...) { pl_log(PL_INFO_AND_UP, "\e[91m", "ERROR", true, msg, ##__VA_ARGS__); call_error_callback(); return code; }
#define PL_ABORT() { pl_log(PL_INFO_AND_UP, "\e[31m", "CRITICAL", true, "Aborting due to programming error"); call_abort_callback(); abort(); }

#ifdef PL_LOG_IMPLEMENTATION

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef PL_ERROR_SZ
#  define PL_ERROR_SZ 4096
#endif

pl_Level    pl_level_ = PL_INFO_AND_UP;
bool        pl_color_ = true;
bool        pl_levelname_ = true;
char        pl_last_error_[PL_ERROR_SZ] = "";
void        (*pl_warning_callback_)(void* data);
void*       pl_warning_callback_data_;
void        (*pl_error_callback_)(void* data);
void*       pl_error_callback_data_;
void        (*pl_abort_callback_)(void* data);
void*       pl_abort_callback_data_;
FILE*       pl_file_ = NULL;

void pl_init()
{
    char* level = getenv("PL_LEVEL");
    if (level && strcmp(level, "trace") == 0)
        pl_level_ = PL_TRACE_LEVEL;
    else if (level && strcmp(level, "debug") == 0)
        pl_level_ = PL_DEBUG_LEVEL;

    char* color = getenv("PL_COLOR");
    if (color && strcmp(color, "0") == 0)
        pl_color_ = false;

    char* levelname = getenv("PL_LEVELNAME");
    if (levelname && strcmp(levelname, "0") == 0)
        pl_levelname_ = false;

    pl_file_ = stderr;

    char* file = getenv("PL_FILE");
    if (file && strcmp(file, "stdout") == 0) {
        pl_file_ = stdout;
    } else if (file && strcmp(file, "stderr") != 0) {
        FILE* f = fopen(file, "w");
        if (f) {
            pl_file_ = f;
            pl_color_ = false;
        } else {
            fprintf(stderr, "Could not open file '%s' for logging: %s", file, strerror(errno));
        }
    }
}

void pl_close()
{
    if (pl_file_ != stderr && pl_file_ != stdout)
        fclose(pl_file_);
}

pl_Level pl_level()
{
    return pl_level_;
}

void pl_set_level(pl_Level level)
{
    pl_level_ = level;
}

const char* pl_last_error()
{
    return pl_last_error_;
}

void pl_set_warning_callback(void (*cb)(void* data), void* data)
{
    pl_warning_callback_ = cb;
    pl_warning_callback_data_ = data;
}

void pl_set_error_callback(void (*cb)(void* data), void* data)
{
    pl_error_callback_ = cb;
    pl_error_callback_data_ = data;
}

void pl_set_abort_callback(void (*cb)(void* data), void* data)
{
    pl_abort_callback_ = cb;
    pl_abort_callback_data_ = data;
}

void pl_log(pl_Level level, const char* color, const char* lvl, bool copy_error, const char* msg, ...)
{
    if (!pl_file_)
        pl_file_ = stderr;

    if (level > pl_level_)
        return;
    if (pl_color_)
        fprintf(pl_file_, "%s", color);
    if (pl_levelname_)
        fprintf(pl_file_, "%s: ", lvl);

    va_list ap;
    va_start(ap, msg);
    vfprintf(pl_file_, msg, ap);
    if (copy_error)
        vsnprintf(pl_last_error_, sizeof pl_last_error_, msg, ap);
    va_end(ap);

    if (pl_color_)
        fprintf(pl_file_, "\e[0m");
    fprintf(pl_file_, "\n");
}

void call_warning_callback()
{
    if (pl_warning_callback_)
        pl_warning_callback_(pl_warning_callback_data_);
}

void call_error_callback()
{
    if (pl_error_callback_)
        pl_error_callback_(pl_error_callback_data_);
}

void call_abort_callback()
{
    if (pl_abort_callback_)
        pl_abort_callback_(pl_abort_callback_data_);
}

#endif

#ifdef __cplusplus
}
#endif

#endif //PL_LOG_H
