ADD R0, R0, #8
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 ; R0 = 64
ADD R0, R0, #1 ; R0 = 65 (ASCII 'A')
JSR 0xb ; call print R0 function
ADD R1, R1, #8
ADD R1, R1, #8 
ADD R1, R1, #8 
ADD R1, R1, #8 
ADD R1, R1, #8 
ADD R1, R1, #8 
ADD R1, R1, #8 
ADD R1, R1, #8 ; R0 = 64
ADD R1, R1, #1 ; R0 = 65 (ASCII 'A')
TRAP 0x3 ; halt
TRAP 0x1 ; print R0
RET