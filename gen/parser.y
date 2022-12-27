%{
  #include "symtable.h"
  #include "quad.h"
  #include "vec.h"

  #include "lib.h"

  extern int yylex();
  extern void yyerror(const char *s);
  extern vec_t quad_array;
%}

%union {
  char *id;
  int integer;
  char *string;
  struct quad *quad;
}

%token <id>ID
%token <integer>integer
%token declare
%token IF THEN FI ELIF ELSE
%token FOR DO IN DONE
%token WHILE UNTIL
%token CASE ESAC
%token EKKO
%token READ 
%token RETURN
%token EXIT
%token <string>word 
%token <string>string
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

%type <quad> op
%type <quad> operator1
%type <quad> operator2
%type <quad> sum_int
%type <quad> prod_int
%type <quad> op_int
%type <integer> plus_minus
%type <integer> mult_div_mod

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

// row of operation -> put vec_t
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
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new(id_arg, &$3), NULL, NULL); }
| '$' '{' ID '[' op_int ']' '}'
{ $$ = quad_new(0, assn_int_to_arraysub_op, quadarg_new(id_arg, &$3), quadarg_new(int_arg, &$5), NULL); }
| word 
{ $$ = quad_new(0, assn_string_to_var_op, quadarg_new(str_arg, &$1), NULL, NULL); }
| '$' integer
{ $$ = quad_new(0, assn_arg_to_var_op, quadarg_new(int_arg, &$2), NULL, NULL); }
| '$' '*'
{ $$ = quad_new(0, assn_all_arg_to_var_op, NULL, NULL, NULL); }
| '$' '?'
{ $$ = quad_new(0, assn_status_to_var_op, NULL, NULL, NULL); }
| '"' string '"'
{ $$ = quad_new(0, assn_string_to_var_op, quadarg_new(str_arg, &$2), NULL, NULL); }
| '\'' string '\''
{ $$ = quad_new(0, assn_string_with_quotes_to_var_op, quadarg_new(str_arg, &$2), NULL, NULL); }
| '$' '(' expr sum_int ')'
{ $$ = quad_new(0, assn_expr_value_to_var_op, quadarg_new(int_arg, &$4), NULL, NULL); }
| '$' '(' cfun ')'
// not done yet
{ $$ = quad_new(0, return_op, NULL, NULL, NULL); }
;

operator1
: nnull 
{ $$ = quad_new(0, nnull_op, NULL, NULL, NULL); }
| null
{ $$ = quad_new(0, null_op, NULL, NULL, NULL); }
; 

operator2
: eq
{ $$ = quad_new(0, eq_op, NULL, NULL, NULL); }
| neq
{ $$ = quad_new(0, neq_op, NULL, NULL, NULL); }
| gt
{ $$ = quad_new(0, gt_op, NULL, NULL, NULL); }
| ge
{ $$ = quad_new(0, ge_op, NULL, NULL, NULL); }
| lt
{ $$ = quad_new(0, lt_op, NULL, NULL, NULL); }
| le
{ $$ = quad_new(0, le_op, NULL, NULL, NULL); }
;

sum_int
: sum_int plus_minus prod_int
{ $$ = quad_new(0, $2, quadarg_new(int_arg, &$1), quadarg_new(int_arg, &$3), NULL); }
| prod_int
{ $$ = $1;}
;

prod_int
: prod_int mult_div_mod op_int
{ $$ = quad_new(0, $2, NULL, quadarg_new(int_arg, &$1), quadarg_new(int_arg, &$3)); }
| op_int
{ $$ = $1;}
;

op_int
: '$' '{' ID '}' 
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new(id_arg, &$3), NULL, NULL); }
| '$' '{' ID '[' op_int ']' '}' 
{ $$ = quad_new(0, assn_int_to_arraysub_op, quadarg_new(id_arg, &$3), quadarg_new(int_arg, &$5), NULL); }
| '$' integer 
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new(int_arg, &$2), NULL, NULL); }
| plus_minus '$' '{' ID '}' 
{ $$ = quad_new(0, $1, quadarg_new(id_arg, &$4), NULL, NULL);  }
| plus_minus '$' '{' ID '[' op_int ']' '}' 
{ $$ = quad_new(0, $1, quadarg_new(id_arg, &$4), quadarg_new(int_arg, &$6), NULL); }
| plus_minus '$' integer
{ $$ = quad_new(0, $1, quadarg_new(int_arg, &$3), NULL, NULL); }
| integer
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new(int_arg, &$1), NULL, NULL); }
| plus_minus integer 
{ $$ = quad_new(0,$1, quadarg_new(int_arg, &$2), NULL, NULL); }
| '(' sum_int ')'
{ $$ = $2; }
;

plus_minus
: '+' 
{ $$ = plus_op; }
| '-' 
{ $$ = minus_op; }
;

mult_div_mod
: '*' 
{ $$ = mult_op; }
| '/'   
{ $$ = div_op; }
| '%'
{ $$ = mod_op; }
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
