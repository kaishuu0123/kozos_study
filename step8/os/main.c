#include "defines.h"
#include "kozos.h"
#include "interrupt.h"
#include "lib.h"

static int
start_threads(int argc, char *argv[])
{
	kz_run(test08_1_main, "command", 0x100, 0, NULL);

	return 0;
}

int
main(void)
{
	INTR_DISABLE;

	puts("Hello World! My name is Kozos.\n");

	kz_start(start_threads, "start", 0x100, 0, NULL);

	return 0;
}
