%{
  #include <string.h>
  #include <stdio.h>

  #include "symtable.h"
  #include "quad.h"
  #include "vec.h"
  #include "lib.h"
  #include "protocol.h"

  extern int yylex(), lineNumber;
  extern void yyerror(const char *s);
  extern vec_t quad_array;
  extern struct symtable *id_name_table;

  char *buf;
%}

%union {
  char *id;
  int integer_t;
  char *string;
  struct quad *quad;
  struct vec_s *quadarg_array;
  struct cases_s *cases_t;
  struct filter_s *filter_t;
}

%token <id>ID
%token <integer_t>integer
%token declare
%token IF THEN ELIF ELSE FI
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

%right '='
%left '+' '-'
%left '*' '/' '%' '?'

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

%type <quad> program
%type <quad> instructions
%type <quad> instruction
%type <quad> maybe_else
%type <filter_t> filter
%type <quadarg_array> ops
%type <quad> concat
%type <quad> testing
%type <quad> test_expr
%type <quad> test_expr2
%type <quad> test_expr3
%type <quad> test_instr
%type <quad> op
%type <integer_t> operator1
%type <integer_t> operator2
%type <quad> sum_int
%type <quad> prod_int
%type <quad> op_int
%type <integer_t> plus_minus
%type <integer_t> mult_div_mod
%type <quad> dfun
%type <quad> declarations
%type <quad> cfun


%%

program
: instructions
{ $$ = $1;}
;

instructions
: instructions ';' instruction
{ $$ = quad_new_from_quadarg(lineNumber, instr_op, NULL, NULL, NULL); }
| instruction
{ $$ = $1; }
| %empty
{ $$ = NULL; }
;

instruction
: ID '=' concat
{ $$ = quad_new_from_quadarg(lineNumber, assn_instr_op, quadarg_new_id($1), $3->arg3, NULL); }
| ID '[' op_int ']' '=' concat
{ $$ = quad_new_from_quadarg(lineNumber, assn_array_instr_op, quadarg_new_id($1), $3->arg3, $6->arg3); }
| declare ID '[' integer ']'
{ $$ = quad_new_from_quadarg(lineNumber, declare_array_instr_op, quadarg_new_id($2), quadarg_new_int($4), NULL); }
| IF { quad_new_from_quadarg(lineNumber, if_init_op, NULL, NULL, NULL); } testing THEN 
{ quad_new_from_quadarg(lineNumber, if_op, NULL, NULL, NULL); } instructions maybe_else instructions FI
{ if ($7 == NULL)
    $$ = quad_new_from_quadarg(lineNumber, if_instr_op, NULL, NULL, NULL); 
  else if ($7 == ELSE_OP)
    $$ = quad_new_from_quadarg(lineNumber, if_instr_op, ELSE_OP, NULL, NULL); }
| FOR ID { quad_new_from_quadarg(lineNumber, for_init_op, quadarg_new_id($2), NULL, NULL); } IN 
{ quad_new_from_quadarg(lineNumber, ops_init_op, NULL, NULL, NULL); } ops { quadarg_array_add($6, quadarg_new_id($2));
  quad_new_from_vec(lineNumber, for_assn_op, $6); } DO instructions DONE
{ $$ = quad_new_from_vec(lineNumber, for_instr_op, $6);}
| WHILE {quad_new_from_quadarg(lineNumber, while_init_op, NULL, NULL, NULL); } testing DO instructions DONE
{ $$ = quad_new_from_quadarg(lineNumber, while_instr_op, NULL, NULL, NULL); }
| UNTIL {quad_new_from_quadarg(lineNumber, until_init_op, NULL, NULL, NULL); } testing DO instructions DONE
{ $$ = quad_new_from_quadarg(lineNumber, until_instr_op, $3->arg3, NULL, NULL); }
| CASE op { quad_new_from_quadarg(lineNumber, case_init_op, $2->arg3, NULL, NULL); } IN cases ESAC
{ $$ = quad_new_from_quadarg(lineNumber, case_instr_op, NULL, NULL, NULL); }
| EKKO { quad_new_from_quadarg(lineNumber, ops_init_op, NULL, NULL, NULL); } ops
{ $$ = quad_new_from_vec(lineNumber, echo_instr_op, $3); }
| READ  ID 
{ $$ = quad_new_from_quadarg(lineNumber, read_instr_op, quadarg_new_id($2), NULL, NULL); }
| READ  ID '[' op_int ']'
{ $$ = quad_new_from_quadarg(lineNumber, read_array_instr_op, quadarg_new_id($2), $4->arg3, NULL); }
| dfun
{ $$ = $1; }
| cfun
{ $$ = $1; }
| RETURN 
{ $$ = quad_new_from_quadarg(lineNumber, return_void_op, NULL, NULL, NULL); }
| RETURN op_int
{ $$ = quad_new_from_quadarg(lineNumber, return_int_op, $2->arg3, NULL, NULL); }
| EXIT 
{ $$ = quad_new_from_quadarg(lineNumber, exit_void_op, NULL, NULL, NULL); }
| EXIT op_int
{ $$ = quad_new_from_quadarg(lineNumber, exit_int_op, $2->arg3, NULL, NULL); }
;

maybe_else
: ELIF  { quad_new_from_quadarg(lineNumber, elif_init_op, NULL, NULL, NULL); }  testing THEN 
{ quad_new_from_quadarg(lineNumber, elif_op, NULL, NULL, NULL); } instructions maybe_else
{ if ($7 == NULL)
    $$ = quad_new_from_quadarg(lineNumber, elif_instr_op, NULL, NULL, NULL); 
  else if ($7 == ELSE_OP)
    $$ = quad_new_from_quadarg(lineNumber, elif_instr_op, ELSE_OP, NULL, NULL); }
| ELSE { quad_new_from_quadarg(lineNumber, else_op, NULL, NULL, quadarg_new_reg()); } instructions
{ $$ = (struct quad *)ELSE_OP; }
| %empty
{ $$ = NULL; }
;

cases
: cases filter { quad_new_from_quadarg(lineNumber, case_set_name, quadarg_new_filters($2), NULL, NULL); }
')' instructions ';' ';'
| filter ')' { quad_new_from_quadarg(lineNumber, case_set_name, quadarg_new_filters($1), NULL, NULL); } instructions ';' ';'
;

filter
: string 
{ $$ = calloc(1, sizeof(struct filter_s));
  $$->array_string = malloc(sizeof(char *)); 
  $$->size = 0; 
  $$->array_string[$$->size] = strdup($1);
  $$->size++; }
| integer 
{ $$ = calloc(1, sizeof(struct filter_s));
  $$->array_string = malloc(sizeof(char *)); 
  $$->size = 0; 
  buf = malloc(9);
  sprintf(buf, "%d", $1);
  $$->array_string[$$->size] = strdup(buf);
  $$->size++; }
| filter '|' string 
{ $$ = $1;
  $$->array_string = realloc($$->array_string, sizeof(char *) * ($$->size + 1));
  $$->array_string[$$->size] = strdup($3);
  $$->size++; }
| filter '|' integer 
{ $$ = $1;
  $$->array_string = realloc($$->array_string, sizeof(char *) * ($$->size + 1));
  buf = malloc(9);
  sprintf(buf, "%d", $3);
  $$->array_string[$$->size] = strdup(buf);
  $$->size++; }
| '*'
{ $$ = calloc(1, sizeof(struct filter_s));
  $$->array_string = malloc(sizeof(char *)); 
  $$->size = 0; 
  $$->array_string[$$->size] = "*";
  $$->size++; }
;

ops
: ops op 
{ if ($2->arg1 != ALL_ARG) {
    quadarg_array_add($$, $2->arg3);
    quad_new_from_quadarg(lineNumber, ops_add_op, $2->arg3, NULL, NULL);
  } else {
    quadarg_array_add($$, ALL_ARG);
    quad_new_from_quadarg(lineNumber, ops_add_op, ALL_ARG, NULL, NULL);
  }
}
| ops '$' '{' ID '[' '*' ']' '}'
{ quadarg_array_add($$, quadarg_new_id($4));
  quadarg_array_add($$, ALL); 
  quad_new_from_quadarg(lineNumber, ops_array_op, quadarg_new_id($4), ALL, NULL); }
| op
{ $$ = quadarg_array_new();
  if ($1->arg1 != ALL_ARG) {
    quadarg_array_add($$, $1->arg3);
    quad_new_from_quadarg(lineNumber, ops_add_op, $1->arg3, NULL, NULL);
  } else {
    quadarg_array_add($$, ALL_ARG);
    quad_new_from_quadarg(lineNumber, ops_add_op, ALL_ARG, NULL, NULL);
  }
}
| '$' '{' ID '[' '*' ']' '}'
{ $$ = quadarg_array_new();
  quadarg_array_add($$, quadarg_new_id($3));
  quadarg_array_add($$, ALL); 
  quad_new_from_quadarg(lineNumber, ops_array_op, quadarg_new_id($3), ALL, NULL); }
;

concat
: concat op
{ $$ = quad_new_from_quadarg(lineNumber, concat_op, $1->arg3, $2->arg3, quadarg_new_reg()); }
| op
{ $$ = $1; }
;

testing
: test test_expr
{ $$ = quad_new_from_quadarg(lineNumber, test_op, $2->arg3, NULL, quadarg_new_reg()); }
;

test_expr
: test_expr or test_expr2
{ $$ = quad_new_from_quadarg(lineNumber, or_op, $1->arg3, $3->arg3, quadarg_new_reg()); }
| test_expr2
{ $$ = $1; }
;

test_expr2
: test_expr2 and test_expr3
{ $$ = quad_new_from_quadarg(lineNumber, and_op, $1->arg3, $3->arg3, quadarg_new_reg()); }
| test_expr3
{ $$ = $1; }
;

test_expr3
: '(' test_expr ')'
{ $$ = $2; }
| '!' '(' test_expr ')'
{ $$ = quad_new_from_quadarg(lineNumber, not_op, $3->arg3, NULL, quadarg_new_reg()); }
| test_instr
{ $$ = $1; }
| '!' test_instr
{ $$ = quad_new_from_quadarg(lineNumber, not_op, $2->arg3, NULL, quadarg_new_reg()); }
;

test_instr
: concat eq concat
{ $$ = quad_new_from_quadarg(lineNumber, eq_op, $1->arg3, $3->arg3, quadarg_new_reg()); }
| concat neq concat
{ $$ = quad_new_from_quadarg(lineNumber, neq_op, $1->arg3, $3->arg3, quadarg_new_reg()); }
| operator1 concat
{ $$ = quad_new_from_quadarg(lineNumber, $1, $2->arg3, NULL, quadarg_new_reg()); }
| op operator2 op
{ $$ = quad_new_from_quadarg(lineNumber, $2, $1->arg3, $3->arg3, quadarg_new_reg()); }
;

op
: '$' '{' ID '}'
{ $$ = quad_new_from_quadarg(lineNumber, assn_id_to_var_op, quadarg_new_id($3), NULL, quadarg_new_reg()); }
| '$' '{' ID '[' op_int ']' '}'
{ $$ = quad_new_from_quadarg(lineNumber, assn_id_to_var_op, quadarg_new_id($3), $5->arg3, quadarg_new_reg()); }
| '$' integer
{ $$ = quad_new_from_quadarg(lineNumber, assn_arg_to_var_op, quadarg_new_int($2), NULL, quadarg_new_reg()); }
| '$' '*'
{ $$ = quad_new_from_quadarg(lineNumber, assn_all_arg_to_var_op, ALL_ARG, NULL ,quadarg_new_reg()); }
| '$' '?'
{ $$ = quad_new_from_quadarg(lineNumber, assn_status_to_var_op, NULL, NULL, quadarg_new_reg()); }
| string
{ $$ = quad_new_from_quadarg(lineNumber, assn_string_to_var_op, quadarg_new_str($1), NULL, quadarg_new_reg()); }
| '$' '(' expr sum_int ')'
{ $$ = quad_new_from_quadarg(lineNumber, assn_expr_value_to_var_op, $4->arg3, NULL, quadarg_new_reg()); }
| '$' '(' cfun ')' 
{ $$ = quad_new_from_quadarg(lineNumber, assn_cfun_to_var_op, $3->arg3, NULL, quadarg_new_reg()); }
| integer 
{ $$ = quad_new_from_quadarg(lineNumber, assn_int_to_var_op, quadarg_new_int($1), NULL, quadarg_new_reg()); }
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
{ $$ = quad_new_from_quadarg(lineNumber, $2, $1->arg3, $3->arg3, quadarg_new_reg()); }
| prod_int
{ $$ = $1; }
;

prod_int
: prod_int mult_div_mod op_int
{ $$ = quad_new_from_quadarg(lineNumber, $2, $1->arg3, $3->arg3, quadarg_new_reg()); }
| op_int
{ $$ = $1; }
;

op_int
: '$' '{' ID '}' 
{ $$ = quad_new_from_quadarg(lineNumber, assn_id_to_var_op, quadarg_new_id($3), NULL, quadarg_new_reg()); }
| '$' '{' ID '[' op_int ']' '}' 
{ $$ = quad_new_from_quadarg(lineNumber, assn_id_to_var_op, quadarg_new_id($3), $5->arg3, quadarg_new_reg()); }
| '$' integer 
{ $$ = quad_new_from_quadarg(lineNumber, assn_arg_to_var_op, quadarg_new_int($2), NULL, quadarg_new_reg()); }
| plus_minus '$' '{' ID '}' 
{ $$ = quad_new_from_quadarg(lineNumber, $1, quadarg_new_id($4), NULL, quadarg_new_reg());  }
| plus_minus '$' '{' ID '[' op_int ']' '}' 
{ $$ = quad_new_from_quadarg(lineNumber, $1, quadarg_new_id($4), $6->arg3, quadarg_new_reg()); }
| plus_minus '$' integer
{ $$ = quad_new_from_quadarg(lineNumber, $1, quadarg_new_int($3), ARG, quadarg_new_reg()); }
| integer
{ $$ = quad_new_from_quadarg(lineNumber, assn_int_to_var_op, quadarg_new_int($1), NULL, quadarg_new_reg()); }
| plus_minus integer 
{ $$ = quad_new_from_quadarg(lineNumber,$1, quadarg_new_int($2), NULL, quadarg_new_reg()); }
| '(' sum_int ')'
{ $$ = $2; }
;

plus_minus
: '+' 
{ $$ = plus_op;}
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
: ID '(' ')' '{' declarations 
{ quad_new_from_quadarg(lineNumber, dfun_init_op, quadarg_new_id($1), NULL, quadarg_new_reg()); } instructions '}'
{ $$ = quad_new_from_quadarg(lineNumber, dfun_op, quadarg_new_id($1), NULL, quadarg_new_reg()); }
;

declarations
: declarations local ID '=' concat ';'
{ $$ = quad_new_from_quadarg(lineNumber, assn_instr_op, quadarg_new_id($3), $5->arg3, NULL); }
| %empty
{ $$ = NULL; }
;

cfun
: ID  { quad_new_from_quadarg(lineNumber, ops_init_op, NULL, NULL, NULL); } ops
{ quadarg_array_add($3,quadarg_new_id($1));
  $$ = quad_new_from_vec(lineNumber, cfun_ops, $3); }
| ID
{ $$ = quad_new_from_quadarg(lineNumber, cfun_op, quadarg_new_id($1), NULL, quadarg_new_reg()); }
;

%%

void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
