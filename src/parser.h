/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    integer = 259,                 /* integer  */
    declare = 260,                 /* declare  */
    IF = 261,                      /* IF  */
    THEN = 262,                    /* THEN  */
    FI = 263,                      /* FI  */
    ELIF = 264,                    /* ELIF  */
    ELSE = 265,                    /* ELSE  */
    FOR = 266,                     /* FOR  */
    DO = 267,                      /* DO  */
    IN = 268,                      /* IN  */
    DONE = 269,                    /* DONE  */
    WHILE = 270,                   /* WHILE  */
    UNTIL = 271,                   /* UNTIL  */
    CASE = 272,                    /* CASE  */
    ESAC = 273,                    /* ESAC  */
    EKKO = 274,                    /* EKKO  */
    READ = 275,                    /* READ  */
    RETURN = 276,                  /* RETURN  */
    EXIT = 277,                    /* EXIT  */
    word = 278,                    /* word  */
    string = 279,                  /* string  */
    test = 280,                    /* test  */
    or = 281,                      /* or  */
    and = 282,                     /* and  */
    eq = 283,                      /* eq  */
    neq = 284,                     /* neq  */
    gt = 285,                      /* gt  */
    ge = 286,                      /* ge  */
    lt = 287,                      /* lt  */
    le = 288,                      /* le  */
    null = 289,                    /* null  */
    nnull = 290,                   /* nnull  */
    expr = 291,                    /* expr  */
    local = 292                    /* local  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "gen/parser.y"

  char *id;
  int integer;
  char *string;
  struct quad *quad;
  struct filter_s *filter_t;

#line 109 "src/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_H_INCLUDED  */