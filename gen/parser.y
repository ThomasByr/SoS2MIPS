%{

  #include "lib.h"

  extern int yylex();
  extern void yyerror(const char *s);
%}

%token instructions

%%

program
: instructions
;

%%

void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
