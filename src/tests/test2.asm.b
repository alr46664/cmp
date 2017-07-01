.data  # data variables comes after this (syntax - data_name:)

.text  # this means we are writing instructions/
       # subroutines/macros down below

main:
 li $v0, 1  # system call for print_int
 li $a0, 5  # integer to print
 syscall    # execute syscall
 li $v0, 17 # system call for terminate program
 li $a0, 0  # exit code
 syscall    # execute syscall

# creates a macro that is replaced
# when called (this is not a subroutine!
# has no function return and stack creation)

#.macro print_int($arg)
# li $v0, 1     # system call for print_int
# li $a0, $arg  # print $arg (integer)
# syscall       # execute syscall
#.end_macro

# acessing memory data space ($gp = 0x10000000)
# lw $v0, 0($gp)      # 0x10000000 + 0x0 = 0x10000000






