#ifndef EDITOR_LOG
#define EDITOR_LOG

#include <stdarg.h>

#define stedLog(msg)        _stedLog(msg, __FILE__, __LINE__)
#define stedLogWarning(msg) _stedLogWarning(msg, __FILE__, __LINE__)
#define stedLogError(msg)   _stedLogError(msg, __FILE__, __LINE__)
extern char* stedCreateLogString (const int num_parts, ...); /* takes multiple string values and returns a single string. This return value MUST be freed. */
extern void  stedSetupLogging    (void);
extern void  stedCleanupLogging  (void);
extern void  _stedLog            (char* msg, const char* const file_name, const int line_number);
extern void  _stedLogWarning     (char* msg, const char* const file_name, const int line_number);
extern void  _stedLogError       (char* msg, const char* const file_name, const int line_number);

#endif
