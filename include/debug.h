#ifndef DEBUG_H
#define DEBUG_H

void debug_init(void);

// supports %c, %s, %d, %u, %x and %p
__attribute__(( format(printf, 1, 2) ))
void debug_printf(const char* fmt, ...);

void debug_puts(const char* str);

// prints out what console it's outputting to
void debug_what_console(void);

#endif /* DEBUG_H */
