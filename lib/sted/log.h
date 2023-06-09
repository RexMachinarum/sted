#ifndef EDITOR_LOG
#define EDITOR_LOG

#define stedLog(msg)        _stedLog(msg, __FILE__, __LINE__)
#define stedLogWarning(msg) _stedLogWarning(msg, __FILE__, __LINE__)
#define stedLogError(msg)   _stedLogError(msg, __FILE__, __LINE__)
extern void stedSetupLogging   (void);
extern void stedCleanupLogging (void);
extern void _stedLog           (char* msg, const char* const file_name, const int line_number);
extern void _stedLogWarning    (char* msg, const char* const file_name, const int line_number);
extern void _stedLogError      (char* msg, const char* const file_name, const int line_number);

#endif
