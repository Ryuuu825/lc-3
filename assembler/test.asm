ADD R0, R0, #8
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 
ADD R0, R0, #8 ; R0 = 64
ADD R0, R0, #1 ; R0 = 65 (ASCII 'A')
ADD R1, R1, #0
AND R0, R0, R1 ; R0 = 0
TRAP 0x1 ; print R0
TRAP 0x3 ; halt
