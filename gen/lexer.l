%{

  #include "parser.h"
  #include "lib.h"
%}

%option noinput
%option nounput
%option noyywrap


%%

";" return ';';
"=" return '=';
"[" return '[';
"]" return ']';
"(" return '(';
")" return ')';
"\"" return '"';
"'" return '\'';
"|" return '|';
"*" return '*';
"$" return '$';
"?" return '?';
"{" return '{';
"}" return '}'; 
"!" return '!';
"+" return '+';
"-" return '-';
"/" return '/';
"%" return '%';


"declare" return declare;

"if" return IF;
"then" return THEN;
"fi" return FI;
"elif" return ELIF;
"else" return ELSE;

"for" return FOR;
"do" return DO;
"in" return IN;
"done" return DONE;

"while" return WHILE;
"until" return UNTIL;

"case" return CASE;
"esac" return ESAC;

"echo" return EKKO;
"read" return READ;

"return" return RETURN;
"exit" return EXIT;


"test" return test;

"-o" return or;
"-a" return and;

("=="|"-eq") return eq;
("!="|"-ne") return neq;
"-gt" return gt;
"-ge" return ge;
"-lt" return lt;
"-le" return le;

"-n" return nnull;
"-z" return null;

%{
  /* TODO: FUCKING FIX THIS */
%}

"expr" return expr;

"local" return local;

[_]?[a-zA-Z][a-zA-Z0-9_]* return ID;
([0]|[1-9]+[0-9]*) return integer;

[_]?[a-zA-Z][a-zA-Z0-9_]* return word;
([^"\\\n]|\\.|\\\n)* return string;

[[:space:]] ;

. {
  alert("Unexpected character: %s", yytext);
}

%%
