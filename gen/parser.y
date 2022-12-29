%{
  #include "symtable.h"
  #include "quad.h"
  #include "vec.h"
  #include <string.h>

  #include "lib.h"

  extern int yylex();
  extern void yyerror(const char *s);
  extern vec_t quad_array;
  extern struct symtable *id_name_table;
  struct filter_s {
    int size;
    char **array_string;
  };
%}

%union {
  char *id;
  int integer;
  char *string;
  struct quad *quad;
  struct filter_s *filter_t;
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

%type <quad> program
%type <quad> instructions
%type <quad> instruction
%type <quad> maybe_else
%type <quad> cases
%type <filter_t> filter
%type <quad> ops
%type <quad> concat
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
%type <quad> dfun
%type <quad> declarations
%type <quad> cfun

%%

program
: instructions
{ $$ = $1; }
;

instructions
: instructions ';' instruction
{ $$ = quad_new(0, instr_op, $1->arg3, $3->arg3, NULL); }
| instruction
{ $$ = $1; }
;

instruction
: ID '=' concat
{ $$ = quad_new(0, assn_instr_op, quadarg_new_id($1), $3->arg3, NULL); }
| ID '[' op_int ']' '=' concat
{ struct quad *marker = quad_new(0, array_instr_op, quadarg_new_id($1), $3->arg3, NULL);
  $$ = quad_new(0, assn_array_instr_op, marker->arg3, $6->arg3, NULL); }
| declare ID '[' integer ']'
{ $$ = quad_new(0, declare_array_instr_op, quadarg_new_id($2), quadarg_new_int($4), NULL); }
| IF testing THEN instructions maybe_else instructions FI
{ struct quad *marker1 = quad_new(0, test_instr_op, $2->arg3, $4->arg3, NULL);
  struct quad *marker2 = quad_new(0, maybe_else_instr_op, $5->arg3, $6->arg3, NULL);
  $$ = quad_new(0, if_instr_op, marker1->arg3, marker2->arg3, NULL); }
| FOR ID DO instructions DONE
{ $$ = quad_new(0, for_instr_op, quadarg_new_id($2), $4->arg3, NULL); }
| FOR ID IN ops DO instructions DONE
{ struct quad *marker = quad_new(0, in_instr_op, quadarg_new_id($2), $4->arg3, NULL);
  $$ = quad_new(0, for_instr_op, marker->arg3, $6->arg3, NULL); }
| WHILE testing DO instructions DONE
{ $$ = quad_new(0, while_instr_op, $2->arg3, $4->arg3, NULL); }
| UNTIL testing DO instructions DONE
{ $$ = quad_new(0, until_instr_op, $2->arg3, $4->arg3, NULL); }
| CASE op IN cases ESAC
{ $$ = quad_new(0, case_instr_op, $2->arg3, $4->arg3, NULL); }
| EKKO ops
{ $$ = quad_new(0, echo_instr_op, $2->arg3, NULL, NULL); }
| READ  ID 
{ $$ = quad_new(0, read_instr_op, quadarg_new_id($2), NULL, NULL); }
| READ  ID '[' op_int ']'
{ $$ = quad_new(0, read_array_instr_op, quadarg_new_id($2), $4->arg3, NULL); }
| dfun
{ $$ = $1; }
| cfun
{ $$ = $1; }
| RETURN 
{ $$ = quad_new(0, return_void_op, NULL, NULL, NULL); }
| RETURN op_int
{ $$ = quad_new(0, return_int_op, $2->arg3, NULL, NULL); }
| EXIT 
{ $$ = quad_new(0, exit_void_op, NULL, NULL, NULL); }
| EXIT op_int
{ $$ = quad_new(0, exit_int_op, $2->arg3, NULL, NULL); }
;

maybe_else
: ELIF testing THEN instructions maybe_else
{ struct quad *marker = quad_new(0, testing_op, $2->arg3, $4->arg3, NULL);
  $$ = quad_new(0, elif_op, marker->arg3, $4->arg3, NULL); }
| ELSE instructions
{ $$ = quad_new(0, else_op, $2->arg3, NULL, NULL); }
| %empty
{ $$ = quad_new(0, empty_op, NULL, NULL, NULL); }
;

cases
: cases filter 
{ $2 = calloc(1, sizeof(struct filter_s));
  $2->array_string = malloc(sizeof(char *)); 
  $2->size = 0; } 
  ')' instructions ';' ';'
{ struct quad *marker = quad_new(0, filter_instr, quadarg_new_array_str($2->array_string), $5->arg3, NULL); 
  $$ = quad_new(0, cases_op, $1->arg3, marker->arg3, NULL); }
| filter ')' instructions ';' ';'
{ $$ = quad_new(0, filter_instr, quadarg_new_array_str($1->array_string), $3->arg3, NULL); }
;

filter
: word 
{ $$->array_string[$$->size] = strdup($1);
  $$->size++; }
| '"' string '"'
{ $$->array_string[$$->size] = strdup($2);
  $$->size++; }
| '\'' string '\''
{ $$->array_string[$$->size] = strdup($2);
  $$->size++; }
| filter '|' word
{ $$ = $1;
  $$->array_string = realloc($$->array_string, sizeof(char *) * ($$->size + 1));
  $$->array_string[$$->size] = strdup($3);
  $$->size++; }
| filter '|' '"' string '"'
{ $$ = $1;
  $$->array_string = realloc($$->array_string, sizeof(char *) * ($$->size + 1));
  $$->array_string[$$->size] = strdup($4);
  $$->size++; }
| filter '|' '\'' string '\''
{ $$ = $1;
  $$->array_string = realloc($$->array_string, sizeof(char *) * ($$->size + 1));
  $$->array_string[$$->size] = strdup($4);
  $$->size++; }
| '*'
{ $$->array_string[$$->size] = "*"; 
  $$->size++; }
;

ops
: ops op
{ $$ = quad_new(0, concat_op, $1->arg3, $2->arg3, NULL); }
| op
{ $$ = $1; }
| '$' '{' ID '[' '*' ']' '}'
{ $$ = quad_new(0, assn_array_to_var_ops, quadarg_new_id($3), NULL, NULL); }
;

concat
: concat op
{ $$ = quad_new(0, concat_op, $1->arg3, $2->arg3, NULL); }
| op
{ $$ = $1; }
;

testing
: test test_expr
{ $$ = quad_new(0, test_op, $2->arg3, NULL, NULL); }
;

test_expr
: test_expr or test_expr2
{ $$ = quad_new(0, or_op, $1->arg3, $3->arg3, NULL); }
| test_expr2
{ $$ = $1; }
;

test_expr2
: test_expr2 and test_expr3
{ $$ = quad_new(0, and_op, $1->arg3, $3->arg3, NULL); }
| test_expr3
{ $$ = $1; }
;

test_expr3
: '(' test_expr ')'
{ $$ = $2; }
| '!' '(' test_expr ')'
{ $$ = quad_new(0, not_op, $3->arg3, NULL, NULL); }
| test_instr
{ $$ = $1; }
| '!' test_instr
{ $$ = quad_new(0, not_op, $2->arg3, NULL, NULL); }
;

test_instr
: concat eq concat
{ $$ = quad_new(0, eq_op, $1->arg3, $3->arg3, NULL); }
| concat neq concat
{ $$ = quad_new(0, neq_op, $1->arg3, $3->arg3, NULL); }
| operator1 concat
{ $$ = quad_new(0, $1, $2->arg3, NULL, NULL); }
| op operator2 op
{ $$ = quad_new(0, $2, $1->arg3, $3->arg3, NULL); }
;

op
: '$' '{' ID '}'
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new_id($3), NULL, NULL); }
| '$' '{' ID '[' op_int ']' '}'
{ $$ = quad_new(0, assn_elem_array_to_var_op, quadarg_new_id($3), $5->arg3, NULL); }
| word 
{ $$ = quad_new(0, assn_string_to_var_op, quadarg_new_str($1), NULL, NULL); }
| '$' integer
{ $$ = quad_new(0, assn_arg_to_var_op, quadarg_new_int($2), NULL, NULL); }
| '$' '*'
{ $$ = quad_new(0, assn_all_arg_to_var_op, NULL, NULL, NULL); }
| '$' '?'
{ $$ = quad_new(0, assn_status_to_var_op, NULL, NULL, NULL); }
| '"' string '"'
{ $$ = quad_new(0, assn_string_to_var_op, quadarg_new_str($2), NULL, NULL); }
| '\'' string '\''
{ $$ = quad_new(0, assn_string_to_var_op, quadarg_new_str($2), NULL, NULL); }
| '$' '(' expr sum_int ')'
{ $$ = quad_new(0, assn_expr_value_to_var_op, $4->arg3, NULL, NULL); }
| '$' '(' cfun ')' 
{ $$ = quad_new(0, assn_cfun_to_var_op, $3->arg3, NULL, NULL); }
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
{ $$ = quad_new(0, $2, $1->arg3, $3->arg3, NULL); }
| prod_int
{ $$ = $1;}
;

prod_int
: prod_int mult_div_mod op_int
{ $$ = quad_new(0, $2, $1->arg3, $3->arg3, NULL); }
| op_int
{ $$ = $1;}
;

op_int
: '$' '{' ID '}' 
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new_id($3), NULL, NULL); }
| '$' '{' ID '[' op_int ']' '}' 
{ $$ = quad_new(0, assn_elem_array_to_var_op, quadarg_new_id($3), $5->arg3, NULL); }
| '$' integer 
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new_int($2), NULL, NULL); }
| plus_minus '$' '{' ID '}' 
{ $$ = quad_new(0, $1, quadarg_new_id($4), NULL, NULL);  }
| plus_minus '$' '{' ID '[' op_int ']' '}' 
{ $$ = quad_new(0, $1, quadarg_new_id($4), $6->arg3, NULL); }
| plus_minus '$' integer
{ $$ = quad_new(0, $1, quadarg_new_int($3), NULL, NULL); }
| integer
{ $$ = quad_new(0, assn_int_to_var_op, quadarg_new_int($1), NULL, NULL); }
| plus_minus integer 
{ $$ = quad_new(0,$1, quadarg_new_int($2), NULL, NULL); }
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
{ struct quad *marker = quad_new(0, cont_func_op, $5->arg3, $6->arg3, NULL);
  $$ = quad_new(0, dfun_op, quadarg_new_id($1), marker->arg3, NULL); }
;

declarations
: declarations local ID '=' concat ';'
{ struct quad *marker = quad_new(0, local_decl_op, $1->arg3, quadarg_new_id($3), NULL);
  $$ = quad_new(0, decl_op, quadarg_new_id($3), marker->arg3, NULL); }
| %empty
{ $$ = quad_new(0, decl_op, NULL, NULL, NULL); }
;

cfun
: ID ops
{ $$ = quad_new(0, cfun_ops, quadarg_new_id($1), $2->arg3, NULL); }
| ID
{ $$ = quad_new(0, cfun_op, quadarg_new_id($1), NULL, NULL); }
;

%%

void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
