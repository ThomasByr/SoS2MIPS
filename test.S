
.data
msg0: .asciiz "true\n"
msg1: .asciiz "false\n"

.text
.globl main

main:
li $t0, 1
li $t1, 2
seq $t2, $t0, $t1
beq $t2, 1, instr0
j instr2

instr0:
la $a0, msg0
li $v0, 4
syscall

instr3:
la $a0, msg1
li $v0, 4
syscall

instr2:
li $t3, 3
li $v0, 10
move $a0, $t3
syscall
