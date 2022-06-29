#ifndef Debug_h
#define Debug_h

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat="

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define ANSI(COLOR, v) ANSI_COLOR_##COLOR v ANSI_COLOR_RESET
#define ENDL "\n"

#pragma GCC diagnostic pop

#endif	// Debug_h