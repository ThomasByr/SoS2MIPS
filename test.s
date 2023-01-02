.data

.text
msg0: .asciiz "Hello, World!\n"
la $a0, msg0
li $v0, 4
syscall
li $t1, 42
li $v0, 10
move $a0, $t1
syscall
