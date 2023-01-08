# <img src="assets/imgs/icon.png" alt="icon" width="4%"/> SoS2MIPS - A Subscript of Shell to MIPS32 Assembly Compiler

[![Linux](https://svgshare.com/i/Zhy.svg)](https://docs.microsoft.com/en-us/windows/wsl/tutorials/gui-apps)
[![GitHub license](https://img.shields.io/github/license/ThomasByr/SoS2MIPS)](https://github.com/ThomasByr/SoS2MIPS/blob/master/LICENSE)
[![GitHub commits](https://badgen.net/github/commits/ThomasByr/SoS2MIPS)](https://GitHub.com/ThomasByr/SoS2MIPS/commit/)
[![GitHub latest commit](https://badgen.net/github/last-commit/ThomasByr/SoS2MIPS)](https://gitHub.com/ThomasByr/SoS2MIPS/commit/)
[![Maintenance](https://img.shields.io/badge/maintained%3F-yes-green.svg)](https://GitHub.com/ThomasByr/SoS2MIPS/graphs/commit-activity)

[![C/C++ CI](https://github.com/ThomasByr/SoS2MIPS/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ThomasByr/SoS2MIPS/actions/workflows/c-cpp.yml)
[![CodeQL](https://github.com/ThomasByr/SoS2MIPS/actions/workflows/codeql.yml/badge.svg)](https://github.com/ThomasByr/SoS2MIPS/actions/workflows/codeql.yml)
[![Publish](https://github.com/ThomasByr/SoS2MIPS/actions/workflows/publish.yml/badge.svg)](https://github.com/ThomasByr/SoS2MIPS/actions/workflows/publish.yml)

[![GitHub version](https://badge.fury.io/gh/ThomasByr%2FSoS2MIPS.svg)](https://github.com/ThomasByr/SoS2MIPS)
[![Author](https://img.shields.io/badge/author-@ThomasByr-blue)](https://github.com/ThomasByr)
[![Author](https://img.shields.io/badge/author-@ThomasD-blue)](https://github.com/LosKeeper)
[![Author](https://img.shields.io/badge/author-@EthanH-blue)](https://github.com/EthanAndreas)
[![Author](https://img.shields.io/badge/author-@MathieuM-blue)](https://github.com/MMARTIN77178)

1. [✏️ Setup](#️-setup)
2. [💁 More infos and Usage](#-more-infos-and-usage)
3. [🧪 Testing](#-testing)
4. [🧑‍🏫 Contributing](#-contributing)
5. [⚖️ License](#️-license)
6. [🔄 Changelog](#-changelog)
7. [🐛 Bugs \& TODO](#-bugs--todo)

## ✏️ Setup

> **Note**
> This project is part of a M1 study done at the University of Strasbourg, France.

To install qtspim, please ensure you have the right minimal dependencies installed :

```bash
cd vm && sudo dpkg -i qtspim.deb
sudo apt-get install libgl1 libxkbcommon-x11-0
```

Then compile a release version of the program with :

```bash
make release
```

The produced executable binary is to be found inside of the `bin` folder.

## 💁 More infos and Usage

The program takes command line arguments from (`..` indicating no short option, `<>` that an argument is required and `*` mutual incompatibility) :

| command             | hint                                     | required ? | default |
| ------------------- | ---------------------------------------- | ---------- | ------- |
| `-h, --help`        | display help and **exit**                | ❔         |         |
| `-v, --version`     | display version and **exit**             | ❔         |         |
| `-l, --license`     | display license and **exit**             | ❔         |         |
| `-i, --in` `<>`     | path to input file                       | ✔️         |         |
| `-o, --out` `<>`    | path to output file                      | ❌         | `a.s`   |
| `.., --tos`         | display the Symbol Table on running time | ❌         |         |
| `.., --verbose`     | be very noisy                            | ❌         |         |
| `.., --no-exe`      | do not execute output file               | ❌         |         |
| `-O, --optlvl` `<>` | set optimization level (from `0` or `1`) | ❌         | `0`     |

Note that at the time of writing, the only optimization level implemented is `0`. Furthermore, `-i` and `-o` can be omitter, provided that both (if both) are typed in order. If only one option is used without the hint, you can specify it wherever you want in the command line.

A legal launching instruction could be :

```bash
./bin/sos -o out.s examples/hello_world.sos --tos --verbose
```

<!--

**What's with the optimization level ?**

- constant expression evaluation
- not bothering to evaluation everything related to unused objects
- first order instructions group

**What is working ?**

- standard console i/o
- primitive types
- variable declarations
- first order keywords

-->

## 🧪 Testing

Oh god... please don't.

Still, make sure you have `valgrind` up and ready and then run :

```bash
cd tests && make check_quiet
```

## 🧑‍🏫 Contributing

If you ever want to contribute, please begin by reading our [Contributing Guidelines](.github/CONTRIBUTING.md).

> The standard procedure is :
>
> ```txt
> fork -> git branch -> push -> pull request
> ```
>
> Note that we won't accept any PR :
>
> - that does not follow our Contributing Guidelines
> - that is not sufficiently commented or isn't well formated
> - without any proper test suite
> - with a failing or incomplete test suite

Happy coding ! 🙂

## ⚖️ License

This project is licensed under the GPL-3.0 new or revised license. Please read the [LICENSE](LICENSE) file.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
- Neither the name of the SoS2MIPS authors nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## 🔄 Changelog

<details>
  <summary>  V1 First Release Version (click here to expand) </summary>

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

</details>

## 🐛 Bugs & TODO

**known bugs** (final correction patch version) [see Issues](https://github.com/ThomasByr/SoS2MIPS/issues)

- ~~`qtspim -file ./vm/qtspim/helloworld.s` is failing to parse the assembly file~~
  (not a bug, please just do not use `-bare`)

**todo** (first implementation version)

- [x] add lex/yacc to makefiles
      (throw `.h` to [inc](inc/) folder and `.c` to [src](src/) folder ; then add files to wildcard) [v0.2.1]
- [x] make sure qtspim is properly working
      (dump in some [example files](vm/qtspim/helloworld.s)) [v0.1.1]
- [x] create grammar
      (that's a big fish, please send help) [v1.1]
- [ ] improve error messages on `yylex` and `yyparse`
      (point the current line and make a suggestion)
- [x] standard console i/o
      (`echo` and `read`) [v1.4]
- [ ] primitive types
      (`int`, `float`, `char` -> dynamic types)
- [x] variable declaration
      (support scope : all variables are global by default [?](https://www.gnu.org/software/emacs/manual/html_node/elisp/Local-Variables.html), unless `local` is used) [v1.3]
- [x] function declaration
      (no keywords, no arguments : `funname() { ... }`) [v1.6, no `local`]
- [ ] first order keywords
      (`if` `then` `for` `do` `done` `in` `while` `until` `case` `esac` `echo` `read` `return` `exit` `local` `elif` `else` `fi` `declare` `test` `expr`) [v1.5 for some of them]
- [x] comments
      (with `/**/` from `/*` until `*/`) [v1.2]
- [ ] optimization level 1.0
      (evaluate constant expression at compile time)
- [ ] optimization level 1.1
      (do not evaluate anything related to unused objects)
- [ ] optimization level 1.2
      (group first order lazy instructions : eval + test -> test)
- [ ] support multiple files
      (multi-core compiling)
