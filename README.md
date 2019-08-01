# CPU-Emulator

A simple CPU ISA and emulator. The ISA is 


OPCODES list

NOP     00
MTR     01 RR XXXX
RTM     02 RR XXXX
ITM     03 RR XXXX
ADDM    04 RR XXXX
ADDI    05 RR XXXX
SUBM    06 RR XXXX
SUBI    07 RR XXXX
SUBIND  08 RR XXXX
AND     09 RR XXXX
OR      0A RR XXXX
XOR     0B RR XXXX
NOT     0C RR XXXX
SHL     0D RR XXXX
SHR     0E RR XXXX
JMP     0F RR XXXX
JZ      10 RR XXXX
JNZ     11 RR XXXX

DRAW    FE
HALT    FF



RR table
A  00
AH 01
AL 02
B  03
BH 04
BL 05
...
G  12
GH 13
GL 14
H  15
HH 16
HL 17
