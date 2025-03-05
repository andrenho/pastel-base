#ifndef PL_LOG_H
#define PL_LOG_H

typedef enum pl_Level { PL_TRACE_LEVEL, PL_DEBUG_LEVEL, PL_INFO_AND_UP } pl_Level;

void        pl_init();
void        pl_set_level(pl_Level level);
const char* pl_last_error();
void        pl_set_warning_callback(void (*cb)(void* data), void* data);
void        pl_set_error_callback(void (*cb)(void* data), void* data);
void        pl_set_abort_callback(void (*cb)(void* data), void* data);

#ifdef PL_LOG_IMPLEMENTATION

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef PL_ERROR_SZ
#  define PL_ERROR_SZ 4096
#endif

pl_Level    pl_level_ = PL_INFO_AND_UP;
bool        pl_color_ = true;
bool        pl_levelname_ = true;
const char  pl_last_error_[PL_ERROR_SZ] = "";
void        (*pl_warning_callback_)(void* data);
void*       pl_warning_callback_data_;
void        (*pl_error_callback_)(void* data);
void*       pl_error_callback_data_;
void        (*pl_abort_callback_)(void* data);
void*       pl_abort_callback_data_;

void pl_init()
{
    char* level = getenv("PL_LEVEL");
    if (level && strcmp(level, "trace") == 0)
        pl_level_ = PL_TRACE_LEVEL;
    else if (level && strcmp(level, "debug") == 0)
        pl_level_ = PL_DEBUG_LEVEL;

    char* color = getenv("PL_COLOR");
    if (color && strcmp(color, "0"))
        pl_color_ = false;

    char* levelname = getenv("PL_LEVELNAME");
    if (levelname && strcmp(levelname, "0"))
        pl_levelname_ = false;
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

#endif

#endif //PL_LOG_H
