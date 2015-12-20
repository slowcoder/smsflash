#pragma once

/** @file
    This file contains prototypes for various levels of log and debug output.
    The logging-level for the file that includes this file can
    be set by defining "LOGLEVEL" before including this file.
    For the maximum amount of debug output, define LOGLEVEL to be LOGLEVEL_DEBUG.
    The levels of logging are (in falling verboseness):
    - DEBUG
    - VERB
    - INFO
    - WARN
    - ERROR

    Example usage:
    LOGE("This is the value fourtytwo: %i",42);
*/

#include <stdarg.h>
#include <assert.h>

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

enum {
	LOGLEVEL_ERROR,
	LOGLEVEL_WARN,
	LOGLEVEL_INFO,
	LOGLEVEL_VERB,
	LOGLEVEL_DEBUG,
};

#ifndef LOGLEVEL
#define LOGLEVEL LOGLEVEL_INFO
#endif

#ifdef __cplusplus
extern "C" {
#endif

void __logi(const char *pzFunc,const char *pzFile,int line,const char *pzMessage,...);

#ifndef NDEBUGx
#if defined(__linux__) || defined(__PS3_GCC_REVISION__) || defined(__APPLE__) || defined(__GNUC__)
#define LOGx(l,x,...) \
  do { if (LOGLEVEL >= l) __logi(__func__,__FILE__,__LINE__,x,##__VA_ARGS__); } while (0)
#define LOGE(x,...) LOGx(LOGLEVEL_ERROR,x,##__VA_ARGS__)
#define LOGW(x,...) LOGx(LOGLEVEL_WARN ,x,##__VA_ARGS__)
#define LOGI(x,...) LOGx(LOGLEVEL_INFO ,x,##__VA_ARGS__)
#define LOGV(x,...) LOGx(LOGLEVEL_VERB ,x,##__VA_ARGS__)
#define LOGD(x,...) LOGx(LOGLEVEL_DEBUG,x,##__VA_ARGS__)
#define ASSERT(c,x,...) do { if( !(c) ) { LOGE(x,##__VA_ARGS__); sys_dump(); assert(!x); } } while (0)
#else
#define LOGx(l,x,...) \
  do { if (LOGLEVEL >= l) __logi(__func__,__FILE__,__LINE__,x,__VA_ARGS__); } while (0)
/** Log an error-message (with formatting) to the debug console */
#define LOGE(x,...) LOGx(LOGLEVEL_ERROR,x,__VA_ARGS__)
/** Log a warning-message (with formatting) to the debug console */
#define LOGW(x,...) LOGx(LOGLEVEL_WARN ,x,__VA_ARGS__)
/** Log an informational message (with formatting) to the debug console */
#define LOGI(x,...) LOGx(LOGLEVEL_INFO ,x,__VA_ARGS__)
/** Log a verbose message (with formatting) to the debug console */
#define LOGV(x,...) LOGx(LOGLEVEL_VERB ,x,__VA_ARGS__)
/** Log a debug message (with formatting) to the debug console */
#define LOGD(x,...) LOGx(LOGLEVEL_DEBUG,x,__VA_ARGS__)
/** Evaluates a condition, and if not-true, terminate program execution with a message (includes formatting)*/
#define ASSERT(c,x,...) do { if( !(c) ) { LOGE(x,__VA_ARGS__); sys_dump(); assert(!x); } } while (0)
#endif
#else // No debug
#define LOGE(x,...)
#define LOGW(x,...)
#define LOGI(x,...)
#define LOGV(x,...)
#define LOGD(x,...)
#define ASSERT(c,x,...)
#endif

#define LOG LOGI

#ifdef __cplusplus
}
#endif

