	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @param lock pointer to a mutex_t variable
 *
 * TODO - add a comment each line describing its action
 *
 * %ebx, general register
 * %esp
 * %eax
 *
 */
test_and_set:
	pushl	%ebx		# preserve contents of the %ebx register, which is about to be modified
	movl	8(%esp), %ebx	# dereference the stack pointer "array", access the stored value 8bytes away, store in %ebx general purpose register
	movl	$1, %eax	# store 1 into the general purpose register %eax
	xchg	%eax, (%ebx)	# change the memory adderss of the pointer
	popl	%ebx		#  the previous %ebx register's value into %ebx
	ret		# return control to return address located on the stack
