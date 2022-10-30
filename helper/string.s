
.data

str: .asciiz "Hello, World!"

.text

str_len_main:
  la  $t0, str  # la means load address (so we load the address of str into $t0)
  li  $t1, 0    # $t1 is the counter. set it to 0

str_len:
  lb    $t2 0($t0)
  lb    $t2, 0($t0)       # Load the first byte from address in $t0
  beqz  $t2, str_len_end  # if $t2 == 0 then go to label end
  add   $t0, $t0, 1       # else increment the address
  add   $t1, $t1, 1       # and increment the counter of course
  j     countChr          # finally loop

str_len_end:
  # Do whatever you want here.
  # Just remeber that the length of the string is stored in $t1
