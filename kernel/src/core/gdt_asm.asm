gdtr DW 0
     DD 0

setGdt:
    MOV   [gdtr], DI 
    MOV   [gdtr+2], RSI
    LGDT  [gdtr]
    RET
    
reloadSegments:
   PUSH 0x08
   LEA RAX, [rel .reload_CS]
   PUSH RAX
   RETFQ
.reload_CS:
   MOV   AX, 0x10
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET