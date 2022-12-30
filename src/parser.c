/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "gen/parser.y"

  #include "symtable.h"
  #include "quad.h"
  #include "vec.h"
  #include <string.h>
  #include <stdio.h>

  #include "lib.h"

  extern int yylex();
  extern void yyerror(const char *s);
  extern vec_t quad_array;
  extern struct symtable *id_name_table;
  struct filter_s {
    int size;
    char **array_string;
  };

#line 90 "src/parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_integer = 4,                    /* integer  */
  YYSYMBOL_declare = 5,                    /* declare  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_THEN = 7,                       /* THEN  */
  YYSYMBOL_FI = 8,                         /* FI  */
  YYSYMBOL_ELIF = 9,                       /* ELIF  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_FOR = 11,                       /* FOR  */
  YYSYMBOL_DO = 12,                        /* DO  */
  YYSYMBOL_IN = 13,                        /* IN  */
  YYSYMBOL_DONE = 14,                      /* DONE  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_UNTIL = 16,                     /* UNTIL  */
  YYSYMBOL_CASE = 17,                      /* CASE  */
  YYSYMBOL_ESAC = 18,                      /* ESAC  */
  YYSYMBOL_EKKO = 19,                      /* EKKO  */
  YYSYMBOL_READ = 20,                      /* READ  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_EXIT = 22,                      /* EXIT  */
  YYSYMBOL_word = 23,                      /* word  */
  YYSYMBOL_string = 24,                    /* string  */
  YYSYMBOL_test = 25,                      /* test  */
  YYSYMBOL_or = 26,                        /* or  */
  YYSYMBOL_and = 27,                       /* and  */
  YYSYMBOL_eq = 28,                        /* eq  */
  YYSYMBOL_neq = 29,                       /* neq  */
  YYSYMBOL_gt = 30,                        /* gt  */
  YYSYMBOL_ge = 31,                        /* ge  */
  YYSYMBOL_lt = 32,                        /* lt  */
  YYSYMBOL_le = 33,                        /* le  */
  YYSYMBOL_null = 34,                      /* null  */
  YYSYMBOL_nnull = 35,                     /* nnull  */
  YYSYMBOL_expr = 36,                      /* expr  */
  YYSYMBOL_local = 37,                     /* local  */
  YYSYMBOL_38_ = 38,                       /* ';'  */
  YYSYMBOL_39_ = 39,                       /* '='  */
  YYSYMBOL_40_ = 40,                       /* '['  */
  YYSYMBOL_41_ = 41,                       /* ']'  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_43_ = 43,                       /* '"'  */
  YYSYMBOL_44_ = 44,                       /* '\''  */
  YYSYMBOL_45_ = 45,                       /* '|'  */
  YYSYMBOL_46_ = 46,                       /* '*'  */
  YYSYMBOL_47_ = 47,                       /* '$'  */
  YYSYMBOL_48_ = 48,                       /* '{'  */
  YYSYMBOL_49_ = 49,                       /* '}'  */
  YYSYMBOL_50_ = 50,                       /* '('  */
  YYSYMBOL_51_ = 51,                       /* '!'  */
  YYSYMBOL_52_ = 52,                       /* '?'  */
  YYSYMBOL_53_ = 53,                       /* '+'  */
  YYSYMBOL_54_ = 54,                       /* '-'  */
  YYSYMBOL_55_ = 55,                       /* '/'  */
  YYSYMBOL_56_ = 56,                       /* '%'  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_program = 58,                   /* program  */
  YYSYMBOL_instructions = 59,              /* instructions  */
  YYSYMBOL_instruction = 60,               /* instruction  */
  YYSYMBOL_maybe_else = 61,                /* maybe_else  */
  YYSYMBOL_cases = 62,                     /* cases  */
  YYSYMBOL_63_1 = 63,                      /* $@1  */
  YYSYMBOL_filter = 64,                    /* filter  */
  YYSYMBOL_ops = 65,                       /* ops  */
  YYSYMBOL_concat = 66,                    /* concat  */
  YYSYMBOL_testing = 67,                   /* testing  */
  YYSYMBOL_test_expr = 68,                 /* test_expr  */
  YYSYMBOL_test_expr2 = 69,                /* test_expr2  */
  YYSYMBOL_test_expr3 = 70,                /* test_expr3  */
  YYSYMBOL_test_instr = 71,                /* test_instr  */
  YYSYMBOL_op = 72,                        /* op  */
  YYSYMBOL_operator1 = 73,                 /* operator1  */
  YYSYMBOL_operator2 = 74,                 /* operator2  */
  YYSYMBOL_sum_int = 75,                   /* sum_int  */
  YYSYMBOL_prod_int = 76,                  /* prod_int  */
  YYSYMBOL_op_int = 77,                    /* op_int  */
  YYSYMBOL_plus_minus = 78,                /* plus_minus  */
  YYSYMBOL_mult_div_mod = 79,              /* mult_div_mod  */
  YYSYMBOL_dfun = 80,                      /* dfun  */
  YYSYMBOL_declarations = 81,              /* declarations  */
  YYSYMBOL_cfun = 82                       /* cfun  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  44
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   304

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  217

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,    43,     2,    47,    56,     2,    44,
      50,    42,    46,    53,     2,    54,     2,    55,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
       2,    39,     2,    52,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,    45,    49,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    85,    85,    90,    92,    97,    99,   102,   104,   108,
     110,   113,   115,   117,   119,   121,   123,   125,   127,   129,
     131,   133,   135,   140,   143,   145,   151,   150,   157,   162,
     165,   168,   171,   176,   181,   186,   192,   194,   196,   201,
     203,   208,   213,   215,   220,   222,   227,   229,   231,   233,
     238,   240,   242,   244,   249,   251,   253,   255,   257,   259,
     261,   263,   265,   267,   272,   274,   279,   281,   283,   285,
     287,   289,   294,   296,   301,   303,   308,   310,   312,   314,
     316,   318,   320,   322,   324,   329,   331,   336,   338,   340,
     345,   351,   354,   359,   361
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "integer",
  "declare", "IF", "THEN", "FI", "ELIF", "ELSE", "FOR", "DO", "IN", "DONE",
  "WHILE", "UNTIL", "CASE", "ESAC", "EKKO", "READ", "RETURN", "EXIT",
  "word", "string", "test", "or", "and", "eq", "neq", "gt", "ge", "lt",
  "le", "null", "nnull", "expr", "local", "';'", "'='", "'['", "']'",
  "')'", "'\"'", "'\\''", "'|'", "'*'", "'$'", "'{'", "'}'", "'('", "'!'",
  "'?'", "'+'", "'-'", "'/'", "'%'", "$accept", "program", "instructions",
  "instruction", "maybe_else", "cases", "$@1", "filter", "ops", "concat",
  "testing", "test_expr", "test_expr2", "test_expr3", "test_instr", "op",
  "operator1", "operator2", "sum_int", "prod_int", "op_int", "plus_minus",
  "mult_div_mod", "dfun", "declarations", "cfun", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-67)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     238,    63,    25,    51,    38,    51,    51,   178,   191,    75,
       9,     9,    84,    67,   -67,   -67,   -67,   -67,   178,     9,
     104,   117,    27,    69,   178,   -67,    90,   165,   138,    32,
     137,   141,    39,   155,   178,   142,   -67,    16,     9,   -67,
     -67,   -67,    11,   -67,   -67,   238,   178,   -67,   140,   144,
     153,   -67,   -67,   181,    21,   -67,   157,   -67,   206,   -67,
     -67,   165,   183,   119,   187,   197,   -67,   -67,   239,   178,
     238,   238,   191,   238,   238,   220,   196,     9,   -67,   225,
     194,   210,   -67,   -67,    17,   -67,   -67,   192,   -67,   -67,
     -15,   191,     9,   190,   -67,   204,   -10,   165,   -67,   178,
     178,   165,   165,   -67,   -67,   -67,   -67,   -67,   -67,   178,
     178,    71,     8,     6,    80,   105,    48,   -67,   213,   222,
     -67,    49,   122,   211,    87,   -67,     9,   -67,   -67,   -67,
       9,   -67,   248,   178,     1,   -67,   208,   -67,   174,   -67,
     -67,    -7,   178,   178,   197,   -67,   -67,    51,   238,   238,
     -67,   238,   -67,   -67,     9,   221,   219,   -67,   228,   238,
     108,   -67,     9,   -67,   210,   -67,   180,   178,   233,   234,
     -67,   273,   -11,   -67,   270,    67,     4,   106,   -67,   -67,
     236,   241,   -67,   256,   257,   242,     9,   -67,   235,   237,
     243,   -67,   238,   -67,   -67,   238,   118,   244,   245,   246,
     247,   -67,   -67,   178,    71,   252,   -67,   -67,   -67,   -67,
     249,   160,   -67,   154,   -67,   -67,   -67
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      19,    21,     0,     2,     4,    17,    18,    56,     0,     0,
       0,     0,     0,     0,    93,    37,     0,     0,     0,     0,
       0,     0,     0,     0,    14,    15,    82,     0,     0,    85,
      86,    20,     0,    22,     1,     0,     5,    40,     0,     0,
       0,    57,    58,     0,     0,    59,     0,    36,     0,    65,
      64,     0,     0,     0,    41,    43,    45,    48,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,    73,    75,    83,     0,     3,    39,     0,    60,    61,
       0,    94,     0,     0,    92,     0,     0,     0,    49,     0,
       0,     0,     0,    66,    67,    68,    69,    70,    71,     0,
      52,    25,     0,     0,     0,     0,     0,    29,     0,     0,
      35,     0,     0,     0,     0,    84,     0,    87,    88,    89,
       0,    81,     0,     0,     0,    54,     0,    63,     0,     7,
      46,     0,    50,    51,    42,    44,    53,     0,     0,     0,
       9,     0,    11,    12,     0,     0,     0,    13,    26,     0,
       0,    16,     0,    76,    72,    74,     0,     6,     0,     0,
      62,     0,     0,    47,     0,    24,     0,     0,    30,    31,
       0,     0,    32,     0,     0,     0,     0,    79,     0,     0,
       0,    90,     0,     8,    10,     0,     0,     0,     0,     0,
       0,    38,    55,     0,    25,     0,    28,    33,    34,    77,
       0,     0,    23,     0,    80,    91,    27
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   -34,   -41,    81,   -67,   -67,   170,     2,   -17,
       3,   -31,   193,   195,   230,    -1,   -67,   -67,   201,   173,
      -8,   -66,   -67,   -67,   -67,   250
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    12,    13,    14,   149,   121,   180,   122,    24,    63,
      28,    64,    65,    66,    67,    86,    69,   109,    80,    81,
      82,    42,   130,    15,   138,    16
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      25,    46,    41,    43,    85,    36,    33,    25,    30,    31,
      34,    48,   193,    36,   126,    83,   101,    47,   151,   101,
      78,   131,   150,    57,    91,   134,    68,    45,    26,    17,
      96,    51,   140,    57,   135,   173,   111,   112,   191,   114,
     115,    29,    45,    51,    71,    72,    45,   168,    37,    20,
      21,    38,   110,    32,    39,    40,    37,    92,    84,    38,
      68,    68,    39,    40,    79,   132,   141,   157,    47,   123,
     126,    25,   117,    52,   113,    53,    27,    54,    35,    55,
     147,   148,   142,   143,    44,    52,    17,    75,   154,    54,
      25,    55,   118,   119,   152,   120,    68,   135,    47,    47,
      68,    68,    18,    19,   172,    45,    20,    21,   146,    45,
      22,    56,    57,    23,   175,   176,   167,   177,    45,   153,
     194,     1,   165,     2,     3,   181,   169,   162,    49,     4,
      58,   182,    47,     5,     6,     7,   163,     8,     9,    10,
      11,    50,    17,    45,    45,    70,   169,    99,   100,    73,
     174,   183,   184,    74,   185,    85,   206,     1,   204,     2,
       3,   205,    20,    21,   159,     4,    32,   160,    76,     5,
       6,     7,    85,     8,     9,    10,    11,     1,   200,     2,
       3,    87,    77,    17,    90,     4,   211,    88,    17,     5,
       6,     7,   216,     8,     9,    10,    11,    89,   215,    59,
      60,    17,    47,    20,    21,    94,    17,    32,    20,    21,
      95,   171,    32,   101,    17,    61,    62,    59,    60,   117,
     186,    20,    21,   116,   102,    32,    20,    21,   124,   187,
      32,   133,   137,    97,    20,    21,   125,   155,    22,   118,
     119,     1,   120,     2,     3,   139,   156,    39,    40,     4,
     170,   166,   161,     5,     6,     7,   127,     8,     9,    10,
      11,    39,    40,   179,   178,   128,   129,   103,   104,   105,
     106,   107,   108,   160,   188,   189,   190,   192,   195,   196,
     197,   198,   203,   199,   201,   212,   202,   207,   210,   208,
     213,   158,    98,   136,   144,   209,     0,   145,   214,   164,
       0,     0,     0,     0,    93
};

static const yytype_int16 yycheck[] =
{
       1,    18,    10,    11,    45,     4,     7,     8,     5,     6,
       8,    19,     8,     4,    80,     4,    26,    18,    12,    26,
       4,     4,    14,    24,     3,    40,    27,    38,     3,    23,
      61,     4,    42,    34,    49,    42,    70,    71,    49,    73,
      74,     3,    38,     4,    12,    13,    38,    46,    47,    43,
      44,    50,    69,    47,    53,    54,    47,    36,    47,    50,
      61,    62,    53,    54,    48,    48,    97,    18,    69,    77,
     136,    72,    23,    46,    72,    48,    25,    50,     3,    52,
       9,    10,    99,   100,     0,    46,    23,    48,    40,    50,
      91,    52,    43,    44,    14,    46,    97,    49,    99,   100,
     101,   102,    39,    40,   138,    38,    43,    44,   109,    38,
      47,    42,   113,    50,   148,   149,   133,   151,    38,    14,
      14,     3,   130,     5,     6,   159,   134,    40,    24,    11,
      40,    23,   133,    15,    16,    17,    49,    19,    20,    21,
      22,    24,    23,    38,    38,     7,   154,    28,    29,    12,
     147,    43,    44,    12,   162,   196,    38,     3,   192,     5,
       6,   195,    43,    44,    42,    11,    47,    45,    13,    15,
      16,    17,   213,    19,    20,    21,    22,     3,   186,     5,
       6,    41,    40,    23,     3,    11,   203,    43,    23,    15,
      16,    17,    38,    19,    20,    21,    22,    44,    38,    34,
      35,    23,   203,    43,    44,    48,    23,    47,    43,    44,
       4,    37,    47,    26,    23,    50,    51,    34,    35,    23,
      40,    43,    44,     3,    27,    47,    43,    44,     3,    49,
      47,    39,    42,    50,    43,    44,    42,    24,    47,    43,
      44,     3,    46,     5,     6,    41,    24,    53,    54,    11,
      42,     3,    41,    15,    16,    17,    46,    19,    20,    21,
      22,    53,    54,    44,    43,    55,    56,    28,    29,    30,
      31,    32,    33,    45,    41,    41,     3,     7,    42,    38,
      24,    24,    39,    41,    49,   204,    49,    43,    41,    44,
      38,   121,    62,    92,   101,    49,    -1,   102,    49,   126,
      -1,    -1,    -1,    -1,    54
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     6,    11,    15,    16,    17,    19,    20,
      21,    22,    58,    59,    60,    80,    82,    23,    39,    40,
      43,    44,    47,    50,    65,    72,     3,    25,    67,     3,
      67,    67,    47,    72,    65,     3,     4,    47,    50,    53,
      54,    77,    78,    77,     0,    38,    66,    72,    77,    24,
      24,     4,    46,    48,    50,    52,    42,    72,    40,    34,
      35,    50,    51,    66,    68,    69,    70,    71,    72,    73,
       7,    12,    13,    12,    12,    48,    13,    40,     4,    48,
      75,    76,    77,     4,    47,    60,    72,    41,    43,    44,
       3,     3,    36,    82,    48,     4,    68,    50,    71,    28,
      29,    26,    27,    28,    29,    30,    31,    32,    33,    74,
      66,    59,    59,    65,    59,    59,     3,    23,    43,    44,
      46,    62,    64,    77,     3,    42,    78,    46,    55,    56,
      79,     4,    48,    39,    40,    49,    75,    42,    81,    41,
      42,    68,    66,    66,    69,    70,    72,     9,    10,    61,
      14,    12,    14,    14,    40,    24,    24,    18,    64,    42,
      45,    41,    40,    49,    76,    77,     3,    66,    46,    77,
      42,    37,    59,    42,    67,    59,    59,    59,    43,    44,
      63,    59,    23,    43,    44,    77,    40,    49,    41,    41,
       3,    49,     7,     8,    14,    42,    38,    24,    24,    41,
      77,    49,    49,    39,    59,    59,    38,    43,    44,    49,
      41,    66,    61,    38,    49,    38,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    61,    61,    61,    63,    62,    62,    64,
      64,    64,    64,    64,    64,    64,    65,    65,    65,    66,
      66,    67,    68,    68,    69,    69,    70,    70,    70,    70,
      71,    71,    71,    71,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    73,    73,    74,    74,    74,    74,
      74,    74,    75,    75,    76,    76,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    78,    78,    79,    79,    79,
      80,    81,    81,    82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     1,     3,     6,     5,     7,     5,
       7,     5,     5,     5,     2,     2,     5,     1,     1,     1,
       2,     1,     2,     5,     2,     0,     0,     7,     5,     1,
       3,     3,     3,     5,     5,     1,     2,     1,     7,     2,
       1,     2,     3,     1,     3,     1,     3,     4,     1,     2,
       3,     3,     2,     3,     4,     7,     1,     2,     2,     2,
       3,     3,     5,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     4,     7,     2,     5,
       8,     3,     1,     2,     3,     1,     1,     1,     1,     1,
       7,     6,     0,     2,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: instructions  */
#line 86 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); printf("BIG\n"); }
#line 1312 "src/parser.c"
    break;

  case 3: /* instructions: instructions ';' instruction  */
#line 91 "gen/parser.y"
{ (yyval.quad) = quad_new(0, instr_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1318 "src/parser.c"
    break;

  case 4: /* instructions: instruction  */
#line 93 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1324 "src/parser.c"
    break;

  case 5: /* instruction: ID '=' concat  */
#line 98 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_instr_op, quadarg_new_id((yyvsp[-2].id)), (yyvsp[0].quad)->arg3, NULL); }
#line 1330 "src/parser.c"
    break;

  case 6: /* instruction: ID '[' op_int ']' '=' concat  */
#line 100 "gen/parser.y"
{ struct quad *marker = quad_new(0, array_instr_op, quadarg_new_id((yyvsp[-5].id)), (yyvsp[-3].quad)->arg3, NULL);
  (yyval.quad) = quad_new(0, assn_array_instr_op, marker->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1337 "src/parser.c"
    break;

  case 7: /* instruction: declare ID '[' integer ']'  */
#line 103 "gen/parser.y"
{ (yyval.quad) = quad_new(0, declare_array_instr_op, quadarg_new_id((yyvsp[-3].id)), quadarg_new_int((yyvsp[-1].integer)), NULL); }
#line 1343 "src/parser.c"
    break;

  case 8: /* instruction: IF testing THEN instructions maybe_else instructions FI  */
#line 105 "gen/parser.y"
{ struct quad *marker1 = quad_new(0, test_instr_op, (yyvsp[-5].quad)->arg3, (yyvsp[-3].quad)->arg3, NULL);
  struct quad *marker2 = quad_new(0, maybe_else_instr_op, (yyvsp[-2].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL);
  (yyval.quad) = quad_new(0, if_instr_op, marker1->arg3, marker2->arg3, NULL); }
#line 1351 "src/parser.c"
    break;

  case 9: /* instruction: FOR ID DO instructions DONE  */
#line 109 "gen/parser.y"
{ (yyval.quad) = quad_new(0, for_instr_op, quadarg_new_id((yyvsp[-3].id)), (yyvsp[-1].quad)->arg3, NULL); }
#line 1357 "src/parser.c"
    break;

  case 10: /* instruction: FOR ID IN ops DO instructions DONE  */
#line 111 "gen/parser.y"
{ struct quad *marker = quad_new(0, in_instr_op, quadarg_new_id((yyvsp[-5].id)), (yyvsp[-3].quad)->arg3, NULL);
  (yyval.quad) = quad_new(0, for_instr_op, marker->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1364 "src/parser.c"
    break;

  case 11: /* instruction: WHILE testing DO instructions DONE  */
#line 114 "gen/parser.y"
{ (yyval.quad) = quad_new(0, while_instr_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1370 "src/parser.c"
    break;

  case 12: /* instruction: UNTIL testing DO instructions DONE  */
#line 116 "gen/parser.y"
{ (yyval.quad) = quad_new(0, until_instr_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1376 "src/parser.c"
    break;

  case 13: /* instruction: CASE op IN cases ESAC  */
#line 118 "gen/parser.y"
{ (yyval.quad) = quad_new(0, case_instr_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1382 "src/parser.c"
    break;

  case 14: /* instruction: EKKO ops  */
#line 120 "gen/parser.y"
{ (yyval.quad) = quad_new(0, echo_instr_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1388 "src/parser.c"
    break;

  case 15: /* instruction: READ ID  */
#line 122 "gen/parser.y"
{ (yyval.quad) = quad_new(0, read_instr_op, quadarg_new_id((yyvsp[0].id)), NULL, NULL); }
#line 1394 "src/parser.c"
    break;

  case 16: /* instruction: READ ID '[' op_int ']'  */
#line 124 "gen/parser.y"
{ (yyval.quad) = quad_new(0, read_array_instr_op, quadarg_new_id((yyvsp[-3].id)), (yyvsp[-1].quad)->arg3, NULL); }
#line 1400 "src/parser.c"
    break;

  case 17: /* instruction: dfun  */
#line 126 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1406 "src/parser.c"
    break;

  case 18: /* instruction: cfun  */
#line 128 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1412 "src/parser.c"
    break;

  case 19: /* instruction: RETURN  */
#line 130 "gen/parser.y"
{ (yyval.quad) = quad_new(0, return_void_op, NULL, NULL, NULL); }
#line 1418 "src/parser.c"
    break;

  case 20: /* instruction: RETURN op_int  */
#line 132 "gen/parser.y"
{ (yyval.quad) = quad_new(0, return_int_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1424 "src/parser.c"
    break;

  case 21: /* instruction: EXIT  */
#line 134 "gen/parser.y"
{ (yyval.quad) = quad_new(0, exit_void_op, NULL, NULL, NULL); }
#line 1430 "src/parser.c"
    break;

  case 22: /* instruction: EXIT op_int  */
#line 136 "gen/parser.y"
{ (yyval.quad) = quad_new(0, exit_int_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1436 "src/parser.c"
    break;

  case 23: /* maybe_else: ELIF testing THEN instructions maybe_else  */
#line 141 "gen/parser.y"
{ struct quad *marker = quad_new(0, testing_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL);
  (yyval.quad) = quad_new(0, elif_op, marker->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1443 "src/parser.c"
    break;

  case 24: /* maybe_else: ELSE instructions  */
#line 144 "gen/parser.y"
{ (yyval.quad) = quad_new(0, else_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1449 "src/parser.c"
    break;

  case 25: /* maybe_else: %empty  */
#line 146 "gen/parser.y"
{ (yyval.quad) = quad_new(0, empty_op, NULL, NULL, NULL); }
#line 1455 "src/parser.c"
    break;

  case 26: /* $@1: %empty  */
#line 151 "gen/parser.y"
{ (yyvsp[0].filter_t) = calloc(1, sizeof(struct filter_s));
  (yyvsp[0].filter_t)->array_string = malloc(sizeof(char *)); 
  (yyvsp[0].filter_t)->size = 0; }
#line 1463 "src/parser.c"
    break;

  case 27: /* cases: cases filter $@1 ')' instructions ';' ';'  */
#line 155 "gen/parser.y"
{ struct quad *marker = quad_new(0, filter_instr, quadarg_new_array_str((yyvsp[-5].filter_t)->array_string), (yyvsp[-2].quad)->arg3, NULL); 
  (yyval.quad) = quad_new(0, cases_op, (yyvsp[-6].quad)->arg3, marker->arg3, NULL); }
#line 1470 "src/parser.c"
    break;

  case 28: /* cases: filter ')' instructions ';' ';'  */
#line 158 "gen/parser.y"
{ (yyval.quad) = quad_new(0, filter_instr, quadarg_new_array_str((yyvsp[-4].filter_t)->array_string), (yyvsp[-2].quad)->arg3, NULL); }
#line 1476 "src/parser.c"
    break;

  case 29: /* filter: word  */
#line 163 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[0].string));
  (yyval.filter_t)->size++; }
#line 1483 "src/parser.c"
    break;

  case 30: /* filter: '"' string '"'  */
#line 166 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1490 "src/parser.c"
    break;

  case 31: /* filter: '\'' string '\''  */
#line 169 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1497 "src/parser.c"
    break;

  case 32: /* filter: filter '|' word  */
#line 172 "gen/parser.y"
{ (yyval.filter_t) = (yyvsp[-2].filter_t);
  (yyval.filter_t)->array_string = realloc((yyval.filter_t)->array_string, sizeof(char *) * ((yyval.filter_t)->size + 1));
  (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[0].string));
  (yyval.filter_t)->size++; }
#line 1506 "src/parser.c"
    break;

  case 33: /* filter: filter '|' '"' string '"'  */
#line 177 "gen/parser.y"
{ (yyval.filter_t) = (yyvsp[-4].filter_t);
  (yyval.filter_t)->array_string = realloc((yyval.filter_t)->array_string, sizeof(char *) * ((yyval.filter_t)->size + 1));
  (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1515 "src/parser.c"
    break;

  case 34: /* filter: filter '|' '\'' string '\''  */
#line 182 "gen/parser.y"
{ (yyval.filter_t) = (yyvsp[-4].filter_t);
  (yyval.filter_t)->array_string = realloc((yyval.filter_t)->array_string, sizeof(char *) * ((yyval.filter_t)->size + 1));
  (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1524 "src/parser.c"
    break;

  case 35: /* filter: '*'  */
#line 187 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = "*"; 
  (yyval.filter_t)->size++; }
#line 1531 "src/parser.c"
    break;

  case 36: /* ops: ops op  */
#line 193 "gen/parser.y"
{ (yyval.quad) = quad_new(0, concat_op, (yyvsp[-1].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1537 "src/parser.c"
    break;

  case 37: /* ops: op  */
#line 195 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1543 "src/parser.c"
    break;

  case 38: /* ops: '$' '{' ID '[' '*' ']' '}'  */
#line 197 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_array_to_var_ops, quadarg_new_id((yyvsp[-4].id)), NULL, NULL); }
#line 1549 "src/parser.c"
    break;

  case 39: /* concat: concat op  */
#line 202 "gen/parser.y"
{ (yyval.quad) = quad_new(0, concat_op, (yyvsp[-1].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1555 "src/parser.c"
    break;

  case 40: /* concat: op  */
#line 204 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1561 "src/parser.c"
    break;

  case 41: /* testing: test test_expr  */
#line 209 "gen/parser.y"
{ (yyval.quad) = quad_new(0, test_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1567 "src/parser.c"
    break;

  case 42: /* test_expr: test_expr or test_expr2  */
#line 214 "gen/parser.y"
{ (yyval.quad) = quad_new(0, or_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1573 "src/parser.c"
    break;

  case 43: /* test_expr: test_expr2  */
#line 216 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1579 "src/parser.c"
    break;

  case 44: /* test_expr2: test_expr2 and test_expr3  */
#line 221 "gen/parser.y"
{ (yyval.quad) = quad_new(0, and_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1585 "src/parser.c"
    break;

  case 45: /* test_expr2: test_expr3  */
#line 223 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1591 "src/parser.c"
    break;

  case 46: /* test_expr3: '(' test_expr ')'  */
#line 228 "gen/parser.y"
{ (yyval.quad) = (yyvsp[-1].quad); }
#line 1597 "src/parser.c"
    break;

  case 47: /* test_expr3: '!' '(' test_expr ')'  */
#line 230 "gen/parser.y"
{ (yyval.quad) = quad_new(0, not_op, (yyvsp[-1].quad)->arg3, NULL, NULL); }
#line 1603 "src/parser.c"
    break;

  case 48: /* test_expr3: test_instr  */
#line 232 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1609 "src/parser.c"
    break;

  case 49: /* test_expr3: '!' test_instr  */
#line 234 "gen/parser.y"
{ (yyval.quad) = quad_new(0, not_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1615 "src/parser.c"
    break;

  case 50: /* test_instr: concat eq concat  */
#line 239 "gen/parser.y"
{ (yyval.quad) = quad_new(0, eq_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1621 "src/parser.c"
    break;

  case 51: /* test_instr: concat neq concat  */
#line 241 "gen/parser.y"
{ (yyval.quad) = quad_new(0, neq_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1627 "src/parser.c"
    break;

  case 52: /* test_instr: operator1 concat  */
#line 243 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer), (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1633 "src/parser.c"
    break;

  case 53: /* test_instr: op operator2 op  */
#line 245 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer), (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1639 "src/parser.c"
    break;

  case 54: /* op: '$' '{' ID '}'  */
#line 250 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_int_to_var_op, quadarg_new_id((yyvsp[-1].id)), NULL, NULL); }
#line 1645 "src/parser.c"
    break;

  case 55: /* op: '$' '{' ID '[' op_int ']' '}'  */
#line 252 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_elem_array_to_var_op, quadarg_new_id((yyvsp[-4].id)), (yyvsp[-2].quad)->arg3, NULL); }
#line 1651 "src/parser.c"
    break;

  case 56: /* op: word  */
#line 254 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_string_to_var_op, quadarg_new_str((yyvsp[0].string)), NULL, NULL); }
#line 1657 "src/parser.c"
    break;

  case 57: /* op: '$' integer  */
#line 256 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_arg_to_var_op, quadarg_new_int((yyvsp[0].integer)), NULL, NULL); }
#line 1663 "src/parser.c"
    break;

  case 58: /* op: '$' '*'  */
#line 258 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_all_arg_to_var_op, NULL, NULL, NULL); }
#line 1669 "src/parser.c"
    break;

  case 59: /* op: '$' '?'  */
#line 260 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_status_to_var_op, NULL, NULL, NULL); }
#line 1675 "src/parser.c"
    break;

  case 60: /* op: '"' string '"'  */
#line 262 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_string_to_var_op, quadarg_new_str((yyvsp[-1].string)), NULL, NULL); }
#line 1681 "src/parser.c"
    break;

  case 61: /* op: '\'' string '\''  */
#line 264 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_string_to_var_op, quadarg_new_str((yyvsp[-1].string)), NULL, NULL); }
#line 1687 "src/parser.c"
    break;

  case 62: /* op: '$' '(' expr sum_int ')'  */
#line 266 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_expr_value_to_var_op, (yyvsp[-1].quad)->arg3, NULL, NULL); }
#line 1693 "src/parser.c"
    break;

  case 63: /* op: '$' '(' cfun ')'  */
#line 268 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_cfun_to_var_op, (yyvsp[-1].quad)->arg3, NULL, NULL); }
#line 1699 "src/parser.c"
    break;

  case 64: /* operator1: nnull  */
#line 273 "gen/parser.y"
{ (yyval.integer) = nnull_op; }
#line 1705 "src/parser.c"
    break;

  case 65: /* operator1: null  */
#line 275 "gen/parser.y"
{ (yyval.integer) = null_op; }
#line 1711 "src/parser.c"
    break;

  case 66: /* operator2: eq  */
#line 280 "gen/parser.y"
{ (yyval.integer) = eq_op; }
#line 1717 "src/parser.c"
    break;

  case 67: /* operator2: neq  */
#line 282 "gen/parser.y"
{ (yyval.integer) = neq_op; }
#line 1723 "src/parser.c"
    break;

  case 68: /* operator2: gt  */
#line 284 "gen/parser.y"
{ (yyval.integer) = gt_op; }
#line 1729 "src/parser.c"
    break;

  case 69: /* operator2: ge  */
#line 286 "gen/parser.y"
{ (yyval.integer) = ge_op; }
#line 1735 "src/parser.c"
    break;

  case 70: /* operator2: lt  */
#line 288 "gen/parser.y"
{ (yyval.integer) = lt_op; }
#line 1741 "src/parser.c"
    break;

  case 71: /* operator2: le  */
#line 290 "gen/parser.y"
{ (yyval.integer) = le_op; }
#line 1747 "src/parser.c"
    break;

  case 72: /* sum_int: sum_int plus_minus prod_int  */
#line 295 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer), (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1753 "src/parser.c"
    break;

  case 73: /* sum_int: prod_int  */
#line 297 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad);}
#line 1759 "src/parser.c"
    break;

  case 74: /* prod_int: prod_int mult_div_mod op_int  */
#line 302 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer), (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, NULL); }
#line 1765 "src/parser.c"
    break;

  case 75: /* prod_int: op_int  */
#line 304 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad);}
#line 1771 "src/parser.c"
    break;

  case 76: /* op_int: '$' '{' ID '}'  */
#line 309 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_int_to_var_op, quadarg_new_id((yyvsp[-1].id)), NULL, NULL); }
#line 1777 "src/parser.c"
    break;

  case 77: /* op_int: '$' '{' ID '[' op_int ']' '}'  */
#line 311 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_elem_array_to_var_op, quadarg_new_id((yyvsp[-4].id)), (yyvsp[-2].quad)->arg3, NULL); }
#line 1783 "src/parser.c"
    break;

  case 78: /* op_int: '$' integer  */
#line 313 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_int_to_var_op, quadarg_new_int((yyvsp[0].integer)), NULL, NULL); }
#line 1789 "src/parser.c"
    break;

  case 79: /* op_int: plus_minus '$' '{' ID '}'  */
#line 315 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-4].integer), quadarg_new_id((yyvsp[-1].id)), NULL, NULL);  }
#line 1795 "src/parser.c"
    break;

  case 80: /* op_int: plus_minus '$' '{' ID '[' op_int ']' '}'  */
#line 317 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-7].integer), quadarg_new_id((yyvsp[-4].id)), (yyvsp[-2].quad)->arg3, NULL); }
#line 1801 "src/parser.c"
    break;

  case 81: /* op_int: plus_minus '$' integer  */
#line 319 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-2].integer), quadarg_new_int((yyvsp[0].integer)), NULL, NULL); }
#line 1807 "src/parser.c"
    break;

  case 82: /* op_int: integer  */
#line 321 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_int_to_var_op, quadarg_new_int((yyvsp[0].integer)), NULL, NULL); }
#line 1813 "src/parser.c"
    break;

  case 83: /* op_int: plus_minus integer  */
#line 323 "gen/parser.y"
{ (yyval.quad) = quad_new(0,(yyvsp[-1].integer), quadarg_new_int((yyvsp[0].integer)), NULL, NULL); }
#line 1819 "src/parser.c"
    break;

  case 84: /* op_int: '(' sum_int ')'  */
#line 325 "gen/parser.y"
{ (yyval.quad) = (yyvsp[-1].quad); }
#line 1825 "src/parser.c"
    break;

  case 85: /* plus_minus: '+'  */
#line 330 "gen/parser.y"
{ (yyval.integer) = plus_op; printf("test\n"); }
#line 1831 "src/parser.c"
    break;

  case 86: /* plus_minus: '-'  */
#line 332 "gen/parser.y"
{ (yyval.integer) = minus_op; }
#line 1837 "src/parser.c"
    break;

  case 87: /* mult_div_mod: '*'  */
#line 337 "gen/parser.y"
{ (yyval.integer) = mult_op; }
#line 1843 "src/parser.c"
    break;

  case 88: /* mult_div_mod: '/'  */
#line 339 "gen/parser.y"
{ (yyval.integer) = div_op; }
#line 1849 "src/parser.c"
    break;

  case 89: /* mult_div_mod: '%'  */
#line 341 "gen/parser.y"
{ (yyval.integer) = mod_op; }
#line 1855 "src/parser.c"
    break;

  case 90: /* dfun: ID '(' ')' '{' declarations instructions '}'  */
#line 346 "gen/parser.y"
{ struct quad *marker = quad_new(0, cont_func_op, (yyvsp[-2].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL);
  (yyval.quad) = quad_new(0, dfun_op, quadarg_new_id((yyvsp[-6].id)), marker->arg3, NULL); }
#line 1862 "src/parser.c"
    break;

  case 91: /* declarations: declarations local ID '=' concat ';'  */
#line 352 "gen/parser.y"
{ struct quad *marker = quad_new(0, local_decl_op, (yyvsp[-5].quad)->arg3, quadarg_new_id((yyvsp[-3].id)), NULL);
  (yyval.quad) = quad_new(0, decl_op, quadarg_new_id((yyvsp[-3].id)), marker->arg3, NULL); }
#line 1869 "src/parser.c"
    break;

  case 92: /* declarations: %empty  */
#line 355 "gen/parser.y"
{ (yyval.quad) = quad_new(0, decl_op, NULL, NULL, NULL); }
#line 1875 "src/parser.c"
    break;

  case 93: /* cfun: ID ops  */
#line 360 "gen/parser.y"
{ (yyval.quad) = quad_new(0, cfun_ops, quadarg_new_id((yyvsp[-1].id)), (yyvsp[0].quad)->arg3, NULL); }
#line 1881 "src/parser.c"
    break;

  case 94: /* cfun: ID  */
#line 362 "gen/parser.y"
{ (yyval.quad) = quad_new(0, cfun_op, quadarg_new_id((yyvsp[0].id)), NULL, NULL); }
#line 1887 "src/parser.c"
    break;


#line 1891 "src/parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 365 "gen/parser.y"


void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
