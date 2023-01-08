# Changelog

<summary>The full history, or so was I told...</summary>

## Beta first minor release

**v0.1.0** first release

- helper functions for C app
- ah yes... threadpools in C, but why you may ask ? you know what ? I do not know either
- reworked my old [dict.h](inc/dict.h) hash table
- did some testing (so long valgrind)

**v0.1.1** qtspim

- ditched java for c++ for speed concerns
- please do not use `-bare`

**v0.2.1** dumping lex and yacc

- it kind of works ! yo do I finally understand how to make makefiles ?
- support arbitrary number of lex/yacc files
- renamed `sos.l` and `sos.y` to `lexer.l` and `parser.y`
- the makefile creates files in [src](src/) dir, please do not push them
- changed the makefile for tests also

## V1 First Release Version

**v1.1** boy do i hate makefiles

- properly linked lex/yacc objects file this time
- `dispose_on_exit` was being changed on `-i` instead of `-o`
- removed `-Werror` compile flag until `static int input(void)` is either used or no longer static
- added `-Werror` back (with `%option noinput` in `./gen/lexer.l`)
- created grammar (shoutout to ThomasD !)

**v1.2** but what is a quad ?

- `quad`, `symtable` and `vec` header files
- `vec` should now be thread safe (still need unit tests for that)
- updated doc
- implemented `symtable` (not based on `dict` - might change that later)
- `quad` is a struct with a `type` and a `value`
- do I go along the AST route? send help

**v1.3** memory

- implemented `.data` segment
- assembly instructions stack
- now we compile
- `--no-exe` flag to not run the compiled assembly file

**v1.4** the end is near

- more to assembly instructions stack for `j` and `jal`
- wrote control flow structures and some more logical expressions
- now we use `sbrk`
- default `exit`, can be omitted

**v1.5** looooooooooops

- added `while` and `for` loops
- rewrote `if` and `else`
- some `echo` fixes
  
**v1.6** functions

- added `function` and `return` keywords
- we can use loops inside functions now
- some more `echo` fixes
- runtime errors are now handled by `_error` block
