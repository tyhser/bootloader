.global _start
_start:
	b reset

reset:
	/* Set cpu to svc32 mode and disable interrupt */

	/* Disable watchdog */


	/* Initial system clock, dram and uart */


	/* Initialize stacks */


	/* Clear bss section */


	/* Call _main */

_main:
	b main
	b _main

