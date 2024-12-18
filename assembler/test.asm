.ORIG 0x3000
        LEA R0, Hi        ; R0 = &Hi
        TRAP 0x22         ; print string
Hi: .STRINGZ "Hello, World!"
.END