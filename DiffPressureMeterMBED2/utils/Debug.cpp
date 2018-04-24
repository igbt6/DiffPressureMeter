#include <cstdarg>
#include "Debug.h"

#ifdef __DEBUG

void DebugHelper::Debug(const char* p_format, ...) {
  va_list argp;
  
  va_start(argp, p_format);
  vprintf(p_format, argp);
  va_end(argp);
}

void DebugHelper::BreakPoint(const char* p_file, int p_line) {
  printf("Stop in %s at line %d\r\n", p_file, p_line);
  fflush(stdout);
  getchar();
  fflush(stdin);
}

#endif // __DEBUG
