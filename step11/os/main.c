#include "defines.h"
#include "kozos.h"
#include "interrupt.h"
#include "lib.h"

static int
start_threads(int argc, char *argv[])
{
	kz_run(test11_1_main, "test11_1", 1, 0x100, 0, NULL);
	kz_run(test11_2_main, "test11_2", 2, 0x100, 0, NULL);

	kz_chpri(15);
	INTR_ENABLE;
	while (1) {
		asm volatile ("sleep");
	}

	return 0;
}

int
main(void)
{
	INTR_DISABLE;

	puts("Hello World! My name is Kozos.\n");

	kz_start(start_threads, "idle", 0, 0x100, 0, NULL);

	return 0;
}
