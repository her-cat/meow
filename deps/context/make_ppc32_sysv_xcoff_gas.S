	.globl	meow_asm_context_make[DS]
	.globl .meow_asm_context_make[PR]
	.align 2 
	.csect  meow_asm_context_make[DS]
meow_asm_context_make:
	.long .meow_asm_context_make[PR]
	.csect .meow_asm_context_make[PR], 3
#.meow_asm_context_make:
    # save return address into R6
    mflr  6

    # first arg of meow_asm_context_make() == top address of context-function
    # shift address in R3 to lower 16 byte boundary
    clrrwi  3, 3, 4

    # reserve space for context-data on context-stack
    # including 64 byte of linkage + parameter area (R1 % 16 == 0)
    subi  3, 3, 304

    # third arg of meow_asm_context_make() == address of context-function
    stw  5, 236(3)

    # load LR
    mflr  0
    # jump to label 1
    bl  .Label
.Label:
    # load LR into R4
    mflr  4
    # compute abs address of label .L_finish
    addi  4, 4, .L_finish - .Label 
    # restore LR
    mtlr  0
    # save address of finish as return-address for context-function
    # will be entered after context-function returns
    stw  4, 232(3)

    # restore return address from R6
    mtlr  6

    blr  # return pointer to context-data

.L_finish:
    # save return address into R0
    mflr  0
    # save return address on stack, set up stack frame
    stw  0, 4(1)
    # allocate stack space, R1 % 16 == 0
    stwu  1, -16(1)

    # exit code is zero
    li  3, 0
    # exit application
    bl  ._exit
    nop
