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
  struct vec_s *vec;
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

%type <vec> ops
%type <vec> concat
%type <quad> testing
%type <quad> test_expr
%type <quad> test_expr2
%type <quad> test_expr3
%type <quad> test_instr
%type <quad> op
%type <integer> operator1
%type <integer> operator2
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

ops
: ops op
{ $$ = quadarray_add($1, $2); }
| op
{ $$ = quadarray_new(); 
  $$ = quadarray_add($$, $1); }
| '$' '{' ID '[' '*' ']' '}'
{ $$ = quadarray_new(); 
  $$ = quadarray_add($$, quad_new(0, assn_array_to_var_op, quadarg_new(id_arg, &$3), NULL, NULL)); }
;

concat
: concat op
{ $$ = quadarray_add($1, $2); }
| op
{ $$ = quadarray_new(); 
  $$ = quadarray_add($$, $1); }
;

testing
: test test_expr
{ $$ = quad_new(0, test_op, quadarg_new(quad_arg, &$2), NULL, NULL); }
;

test_expr
: test_expr or test_expr2
{ $$ = quad_new(0, or_op, quadarg_new(quad_arg, &$1), quadarg_new(quad_arg, &$3), NULL); }
| test_expr2
{ $$ = $1; }
;

test_expr2
: test_expr2 and test_expr3
{ $$ = quad_new(0, and_op, quadarg_new(quad_arg, &$1), quadarg_new(quad_arg, &$3), NULL); }
| test_expr3
{ $$ = $1; }
;

test_expr3
: '(' test_expr ')'
{ $$ = $2; }
| '!' '(' test_expr ')'
{ $$ = quad_new(0, not_op, quadarg_new(quad_arg, &$3), NULL, NULL); }
| test_instr
{ $$ = $1; }
| '!' test_instr
{ $$ = quad_new(0, not_op, quadarg_new(quad_arg, &$2), NULL, NULL); }
;

test_instr
: concat eq concat
{ $$ = quad_new(0, eq_vec_op, quadarg_new(vec_arg, &$1), quadarg_new(vec_arg, &$3), NULL); }
| concat neq concat
{ $$ = quad_new(0, neq_vec_op, quadarg_new(vec_arg, &$1), quadarg_new(vec_arg, &$3), NULL); }
| operator1 concat
{ $$ = quad_new(0, $1, quadarg_new(vec_arg, &$2), NULL, NULL); }
| op operator2 op
{ $$ = quad_new(0, $2, quadarg_new(vec_arg, &$1), quadarg_new(vec_arg, &$3), NULL); }
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
{ $$ = quad_new(0, assn_string_to_var_op, quadarg_new(str_arg, &$2), NULL, NULL); }
| '$' '(' expr sum_int ')'
{ $$ = quad_new(0, assn_expr_value_to_var_op, quadarg_new(int_arg, &$4), NULL, NULL); }
| '$' '(' cfun ')'
// not done yet
{ $$ = quad_new(0, return_op, NULL, NULL, NULL); }
;

operator1
: nnull 
{ $$ = nnull_op; }
| null
{ $$ = null_op; }
; 

operator2
: eq
{ $$ = eq_op; }
| neq
{ $$ = neq_op; }
| gt
{ $$ = gt_op; }
| ge
{ $$ = ge_op; }
| lt
{ $$ = lt_op; }
| le
{ $$ = le_op; }
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
