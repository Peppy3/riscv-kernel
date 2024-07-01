#include <stdarg.h>

#include <sbi.h>
#include <cpu.h>

#include <spinlock.h>

typedef int (*putchar_fn)(int);

struct {
	// Both are blocking
	// (Stupid backwards compatibility)
	putchar_fn putchar;

	struct Spinlock spinlock;
} dbg;

int debug_con_putchar(int ch) {
	return sbi_debug_console_write_byte(ch & 0xff).error;
}

int legacy_con_putchar(int ch) {
	return sbi_legacy_putchar(ch & 0xff);
}

void debug_init(void) {
	spinlock_init(&dbg.spinlock);

	if (sbi_probe_extension(SBI_EXT_DEBUG)) {
		dbg.putchar = debug_con_putchar;
	}
	else {
		dbg.putchar = legacy_con_putchar;
	}
}


const char digits[] = "0123456789abcdef";

void print_int(int x, putchar_fn putchar, int dec, int sign) {
	char buff[16] = { 0 };
	unsigned int num;
	
	if (sign && (sign = x < 0)) {
		num = -x;
	}
	else {
		num = x;
	}

	int i = 0;
	do {
		buff[i++] = digits[num % dec];
	} while ((num /= dec) != 0);
	
	if (sign) buff[i++] = '-';

	while (--i >= 0) {
		putchar(buff[i]);
	}
}

void print_ptr(unsigned long p, putchar_fn putchar) {
	putchar('0'); putchar('x');
	int i = 0;
	while (i < (int)(sizeof(unsigned long) * 2)) {
		char ch = digits[p >> (sizeof(unsigned long) * 8 - 4)];
		putchar(ch);

		i++;
		p <<= 4;
	}
}

// supports %c, %s, %d, %u, %x and %p
__attribute__(( format(printf, 1, 2) ))
void debug_printf(const char* fmt, ...)  {
	putchar_fn putchar = dbg.putchar;

	spinlock_aquire(&dbg.spinlock);

	va_list vargs;
	va_start(vargs, fmt);
	
	unsigned long i = 0;
	while (fmt[i] != '\0') {
		char ch = fmt[i++];
		if (ch != '%') {
			putchar(ch);
			continue;
		}

		ch = fmt[i++];
		if (ch == '\0') {
			putchar('%');
			break;
		}
		switch (ch) {
			case 'c': 
				{
					int c = va_arg(vargs, int);
					putchar(c);
				} break;
			case 's':
				{
					char *s = va_arg(vargs, char *);
					if (s == NULL) s = "(null)";

					while (*s != '\0') {
						putchar(*s++);
					}
				} break;
			case 'd':
				print_int(va_arg(vargs, int), putchar, 10, 1);
				break;
			case 'u':
				print_int(va_arg(vargs, int), putchar, 10, 0);
				break;
			case 'x':
				print_int(va_arg(vargs, unsigned int), putchar, 16, 0);
				break;
			case 'p':
				print_ptr(va_arg(vargs, unsigned long), putchar);
				break;
			case '%':
				putchar('%'); 
				break;
			default:
				putchar('%');
				putchar(ch);
		}

	}
	va_end(vargs);

	spinlock_release(&dbg.spinlock);
}

void debug_puts(const char* str) {
	putchar_fn putchar = dbg.putchar;
	spinlock_aquire(&dbg.spinlock);
	
	long i = 0;
	while (str[i] != '\0') {
		putchar(str[i++]);
	}
	putchar('\n');

	spinlock_release(&dbg.spinlock);
}

void debug_what_console(void) {
	char *con = "legacy console";
	if (dbg.putchar == debug_con_putchar) {
		con = "debug console";
	}
	debug_printf("Debug console: %s\n", con);
}
