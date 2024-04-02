#include <stddef.h>

#include <klib/common.h>

#include <sbi.h>
#include <dtb.h>

typedef long hartid_t;

long legacy_print(const char *s) {
	long err = 0;
	for (; *(s) != '\0'; s++) {
		err = sbi_legacy_putchar(*s);
		if (err < 0) break;
	}
	return err;
}

void start(hartid_t boot_hartid, dtb_t* dtb_location) {
	legacy_print("Hello Kernel World!\r\n");

	(void)sbicall0_legacy(0x08);

	while (1);
}
