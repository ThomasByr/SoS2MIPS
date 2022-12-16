%{

  #include "lib.h"

  extern int yylex();
  extern void yyerror(const char *s);
%}

%token ID
%token integer
%token declare
%token IF THEN FI ELIF ELSE
%token FOR DO IN DONE
%token WHILE UNTIL
%token CASE ESAC
%token EKKO
%token READ 
%token RETURN
%token EXIT
%token word string
%token test

%left or
%left and

%left eq
%left neq
%left gt
%left ge
%left lt
%left le

%token null nnull
%token expr
%token local

%%

program
: instructions
;

instructions
: instructions ';' instruction
| instruction
;

instruction
: ID '=' concat
| ID '[' op_int ']' '=' concat
| declare ID '[' integer ']'
| IF testing THEN instructions maybe_else instructions FI
| FOR ID DO instructions DONE
| FOR ID IN ops DO instructions DONE
| WHILE testing DO instructions DONE
| UNTIL testing DO instructions DONE
| CASE op IN cases ESAC
| EKKO ops
| READ  ID 
| READ  ID '[' op_int ']'
| dfun
| cfun
| RETURN 
| RETURN op_int
| EXIT 
| EXIT op_int
;

maybe_else
: ELIF testing THEN instructions maybe_else
| ELSE instructions
| %empty
;

cases
: cases filter ')' instructions ';' ';'
| filter ')' instructions ';' ';'
;

filter
: word 
| '"' string '"'
| '\'' string '\''
| filter '|' word
| filter '|' '"' string '"'
| filter '|' '\'' string '\''
| '*'
;

ops
: ops op
| op
| '$' '{' ID '[' '*' ']' '}'
;

concat
: concat op
| op
;

testing
: test test_expr
;

test_expr
: test_expr or test_expr2
| test_expr2
;

test_expr2
: test_expr2 and test_expr3
| test_expr3
;

test_expr3
: '(' test_expr ')'
| '!' '(' test_expr ')'
| test_instr
| '!' test_instr
;

test_instr
: concat eq concat
| concat neq concat
| operator1 concat
| op operator2 op
;

op
: '$' '{' ID '}'
| '$' '{' ID '[' op_int ']' '}'
| word 
| '$' integer
| '$' '*'
| '$' '?'
| '"' string '"'
| '\'' string '\''
| '$' '(' expr sum_int ')'
| '$' '(' cfun ')'
;

operator1
: nnull
| null
; 

operator2
: eq
| neq
| gt
| ge
| lt
| le
;

sum_int
: sum_int plus_minus prod_int
| prod_int
;

prod_int
: prod_int mult_div_mod op_int
| op_int
;

op_int
: '$' '{' ID '}'
| '$' '{' ID '[' op_int ']' '}'
| '$' integer
| plus_minus '$' '{' ID '}'
| plus_minus '$' '{' ID '[' op_int ']' '}'
| plus_minus '$' integer
| integer
| plus_minus integer
| '(' sum_int ')'
;

plus_minus
: '+'
| '-'
;

mult_div_mod
: '*'
| '/'
| '%'
;

dfun
: ID '(' ')' '{' declarations instructions '}'
;

declarations
: declarations local ID '=' concat ';'
| %empty
;

cfun
: ID ops
| ID
;

%%

void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
