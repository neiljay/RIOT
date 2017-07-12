#include <mips/asm.h>
#include <mips/regdef.h>
#include <mips/cpu.h>


NESTED(_vechead, 0, ra)
	.set	push
	.set	noat

	/* todo copy sp from current process to shadow set */

	/* these get patched after this code is copied to the vector location */
	lui k0 0xDEAD
	ori k0 0xBEEF

	jr k0
	.set	pop
END(_svechead)
