.ORIG 0x3000
LOOP:   LEA R0, A
        LDR R0, R0, #0
        JSR print
        TRAP 0x3 ; halt
print:  TRAP 0x01 ; print char
        RET         

A: .FILL #65
TEST6: .BLKW #5

.END
