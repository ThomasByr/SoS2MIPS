%{

  #include "parser.h"
  #include "lib.h"
%}

%option nounput
%option noyywrap


%%

[[:space:]] ;

. {
  alert("Unexpected character: %s", yytext);
}

%%
