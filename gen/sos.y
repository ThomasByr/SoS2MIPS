%{

  #include "lib.h"

  extern int yylex();
  extern void yyerror(const char *s);
%}

%%

%%

void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
