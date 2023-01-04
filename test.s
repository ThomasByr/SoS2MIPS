
.data
array: .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
msg0: .asciiz "Array[5] : "
msg1: .asciiz "\nArray[1] : "

.text
.globl main

main:
li $t0, 5
li $t1, 42
mul $t0, $t0, 4
sw $t1, array($t0)
li $t0, 1
li $t1, 1
mul $t0, $t0, 4
sw $t1, array($t0)
la $a0, msg0
li $v0, 4
syscall
li $t1, 5
mul $t1, $t1, 4
lw $t2, array($t1)
move $a0, $t2
li $v0, 1
syscall
la $a0, msg1
li $v0, 4
syscall
li $t3, 1
mul $t3, $t3, 4
lw $t4, array($t3)
move $a0, $t4
li $v0, 1
syscall
li $v0, 10
syscall
