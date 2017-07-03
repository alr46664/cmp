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

Registers $a0–$a3 (4–7) are used to pass the first four arguments to rou-
tines (remaining arguments are passed on the stack). Registers $v0 and
$v1 (2, 3) are used to return values from functions.

Registers $t0–$t9 (8–15, 24, 25) are caller-saved registers that are used
to hold temporary quantities that need not be preserved across calls (see
section 3.6 in Chapter 3).

Registers $s0–$s7 (16–23) are callee-saved registers that hold long-
lived values that should be preserved across calls.

Register $gp (28) is a global pointer that points to the middle of a 64K
block of memory in the static data segment.

Register $sp (29) is the stack pointer, which points to the first free loca-
tion on the stack. Register $fp (30) is the frame pointer. The jal instruc-
tion writes register $ra (31), the return address from a procedure call.
These two registers are explained in the next section.

# how to do function calls
    I) CALLER (before calling the function):

        1. Pass arguments. By convention, the first four arguments are passed in
        registers $a0–$a3. Any remaining arguments are pushed on the stack
        and appear at the beginning of the called procedure’s stack frame.

        2. Save caller-saved registers. The called procedure can use these registers
        ($a0–$a3 and $t0–$t9) without first saving their value. If the caller
        expects to use one of these registers after a call, it must save its value
        before the call.

        3. Execute a jal instruction (see section 3.6 of Chapter 3), which jumps to
        the callee’s first instruction and saves the return address in register $ra.

    II) CALLEE (before execution of any code):

        1. Allocate memory for the frame by subtracting the frame’s size from the
        stack pointer.

        2. Save callee-saved registers in the frame. A callee must save the values
        in these registers ($s0–$s7, $fp, and $ra) before altering them since the
        caller expects to find these registers unchanged after the call. Register
        $fp is saved by every procedure that allocates a new stack frame. How-
        ever, register $ra only needs to be saved if the callee itself makes a call.
        The other callee-saved registers that are used also must be saved.

        3. Establish the frame pointer by adding the stack frame’s size minus four
        to $sp and storing the sum in register $fp.

   III) CALLEE (after execution - return to the caller):

        1. If the callee is a function that returns a value, place the returned value
        in register $v0.

        2. Restore all callee-saved registers that were saved upon procedure entry.

        3. Pop the stack frame by subtracting the frame size from $sp.

        4. Return by jumping to the address in register $ra.

#########################  INSTRUCTIONS  #############################


    # ARITHMETIC

        # rdest = |rsrc| (absolute value)
        abs rdest, rsrc

        # rd = rs + rt
        add rd, rs, rt

        # rd = rs + (*imm)
        addi rt, rs, imm

        # rd = rs - rt
        sub rd, rs, rt

        # lo = quotient(rs / rt)
        # hi = remainder(rs / rt)
        div rs, rt

        # rdest = quotient(rsrc1 / src2)
        div rdest, rsrc1, src2

        # rdest = remainder(rsrc1 / src2)
        rem rdest, rsrc1, rsrc2

        # rdest = rsrc1 * src2
        mul rdest, rsrc1, src2


    # LOGICAL

        # rdest = !rsrc (bitwise not)
        not rdest, rsrc

        # rd = rs && rt
        and rd, rs, rt

        # rd = rs && (*imm)
        andi rt, rs, imm

        # rd = rs || rt
        or rd, rs, rt

        # rd = rs || (*imm)
        ori rt, rs, imm

        # rdest = -rsrc
        neg rdest, rsrc


    # COMPARISON

        # rd = (rs < rt ? 1 : 0)
        slt rd, rs, rt

        # rd = (rs < *imm ? 1 : 0)
        slti rd, rs, imm

        # rdest = (rsrc1 <= rsrc2 ? 1 : 0)
        sle rdest, rsrc1, rsrc2

        # rdest = (rsrc1 > rsrc2 ? 1 : 0)
        sgt rdest, rsrc1, rsrc2

        # rdest = (rsrc1 >= rsrc2 ? 1 : 0)
        sge rdest, rsrc1, rsrc2

        # rdest = (rsrc1 == rsrc2 ? 1 : 0)
        seq rdest, rsrc1, rsrc2

        # rdest = (rsrc1 != rsrc2 ? 1 : 0)
        sne rdest, rsrc1, rsrc2


    # MEMORY

        # rdest = (*imm)
        li rdest, imm

        # rdest = address (thats the address pointer)
        la rdest, address

        # rdest = *address (load 32 bit word content)
        lw rt, address

        # *address = rt (store 32 bit word content)
        sw rt, address


    # BRANCH

        # goto label
        b label


        # (rs != rt ? goto label : )
        bne rs, rt, label

        # (rs == rt ? goto label : )
        beq rs, rt, label

        # (rsrc1 >= rsrc2 ? goto label : )
        bge rsrc1, rsrc2, label

        # (rsrc1 <= rsrc2 ? goto label : )
        ble rsrc1, rsrc2, label

        # (rsrc1 > rsrc2 ? goto label : )
        bgt rsrc1, rsrc2, label

        # (rsrc1 < rsrc2 ? goto label : )
        blt rsrc1, rsrc2, label


        # (rs != 0 ? goto label : )
        bnez rsrc, label

        # (rs == 0 ? goto label : )
        beqz rsrc, label

        # (rs <= 0 ? goto label : )
        blez rs, label

        # (rs >= 0 ? goto label : )
        bgez rs, label

        # (rs <= 0 ? goto label : )
        # and save next instruction addr into reg 31
        bltzal rs, label

        # (rs >= 0 ? goto label : )
        # and save next instruction addr into reg 31
        bgezal rs, label

        # (rs < 0 ? goto label : )
        bltz rs, label

        # (rs > 0 ? goto label : )
        bgtz rs, label


    # JUMPS

        # jump to instr target
        j target

        # jump to instr inside register rs
        jr rs

        # jump to instr target
        # and save next addr into $rd
        jal target

        # jump to instr inside register rs
        # and save next addr into $rd
        jalr rs, rd



















