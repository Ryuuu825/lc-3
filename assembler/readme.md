# Assembler for LC-3
translates assembly code to machine code for the LC-3 architecture.

```x86asm
.ORIG 0x3000
    LEA R0, Hi        ; R0 = &Hi
    TRAP 0x22         ; print string
Hi: .STRINGZ "Hello, World!"
.END
```


### Trap vector table
```c++
std::map<int, trap_func> trap_table = {
    {0x20, trap_getc},
    {0x21, trap_out},
    {0x22, trap_puts},
    {0x24, trap_out_int},
    {0x25, trap_halt},
};
```