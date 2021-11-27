.section ".text.boot"

.globl _start
_start:
	la sp, __stack_top__
.option push
.option norelax
	la gp, __global_pointer$
.option pop
	call _cstart
halt:
	j halt
