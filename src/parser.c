/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

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

  #define ARG (void *)0x42

  extern int yylex();
  extern void yyerror(const char *s);
  extern vec_t quad_array;
  extern struct symtable *id_name_table;
  struct filter_s {
    int size;
    char **array_string;
  };

#line 91 "src/parser.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_SRC_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    integer = 259,
    declare = 260,
    IF = 261,
    THEN = 262,
    ELIF = 263,
    ELSE = 264,
    FI = 265,
    FOR = 266,
    DO = 267,
    IN = 268,
    DONE = 269,
    WHILE = 270,
    UNTIL = 271,
    CASE = 272,
    ESAC = 273,
    EKKO = 274,
    READ = 275,
    RETURN = 276,
    EXIT = 277,
    word = 278,
    string = 279,
    test = 280,
    or = 281,
    and = 282,
    eq = 283,
    neq = 284,
    gt = 285,
    ge = 286,
    lt = 287,
    le = 288,
    null = 289,
    nnull = 290,
    expr = 291,
    local = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 22 "gen/parser.y"

  char *id;
  int integer_t;
  char *string;
  struct quad *quad;
  struct filter_s *filter_t;

#line 189 "src/parser.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_H_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   291

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  95
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  213

#define YYUNDEFTOK  2
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,    48,     2,    51,    31,     2,    49,
      54,    47,    29,    27,     2,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    44,
       2,    26,     2,    56,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,    50,    53,     2,     2,     2,     2,
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
      25,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    92,    92,    97,    99,   101,   106,   108,   110,   112,
     117,   119,   122,   124,   126,   128,   130,   132,   134,   136,
     138,   140,   142,   144,   149,   152,   152,   155,   161,   160,
     167,   172,   175,   178,   181,   186,   191,   196,   202,   204,
     206,   211,   213,   218,   223,   225,   230,   232,   237,   239,
     241,   243,   248,   250,   252,   254,   259,   261,   263,   265,
     267,   269,   271,   273,   275,   280,   282,   287,   289,   291,
     293,   295,   297,   302,   304,   309,   311,   316,   318,   320,
     322,   324,   326,   328,   330,   332,   337,   339,   344,   346,
     348,   353,   359,   362,   367,   369
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "integer", "declare", "IF", "THEN",
  "ELIF", "ELSE", "FI", "FOR", "DO", "IN", "DONE", "WHILE", "UNTIL",
  "CASE", "ESAC", "EKKO", "READ", "RETURN", "EXIT", "word", "string",
  "test", "'='", "'+'", "'-'", "'*'", "'/'", "'%'", "or", "and", "eq",
  "neq", "gt", "ge", "lt", "le", "null", "nnull", "expr", "local", "';'",
  "'['", "']'", "')'", "'\"'", "'\\''", "'|'", "'$'", "'{'", "'}'", "'('",
  "'!'", "'?'", "$accept", "program", "instructions", "instruction",
  "maybe_else", "$@1", "cases", "$@2", "filter", "ops", "concat",
  "testing", "test_expr", "test_expr2", "test_expr3", "test_instr", "op",
  "operator1", "operator2", "sum_int", "prod_int", "op_int", "plus_minus",
  "mult_div_mod", "dfun", "declarations", "cfun", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    61,    43,    45,    42,
      47,    37,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,    59,    91,    93,    41,    34,    39,
     124,    36,   123,   125,    40,    33,    63
};
# endif

#define YYPACT_NINF (-73)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     240,    19,    43,    22,   110,    22,    22,    36,    48,   116,
      47,    47,    49,    78,   -73,   -73,   -73,   -73,   -73,    36,
      47,    26,    81,    36,   -73,    84,    85,   143,    14,   132,
     145,    32,   146,    36,   117,   -73,   -73,   -73,    11,    47,
     -73,    46,   -73,   -73,   240,    36,   -73,   128,   -73,   -73,
     162,    41,   -73,   139,   -73,   171,   -73,   -73,    85,   107,
     100,   164,   168,   -73,   -73,   229,    36,   240,   240,    48,
     240,   240,   200,   137,    47,   -73,   201,   140,   177,   -73,
     -73,    15,   -73,   -73,   186,   -16,    48,    47,   172,   -73,
     178,     6,    85,   -73,    36,    36,    85,    85,   -73,   -73,
     -73,   -73,   -73,   -73,    36,    36,    12,    -3,    30,    88,
      94,   -14,   -73,   -73,   199,   205,   204,    68,   179,    61,
     -73,    47,   -73,   -73,   -73,    47,   -73,   225,    36,   126,
     -73,   155,   -73,   215,   -73,   -73,    73,    36,    36,   168,
     -73,   -73,    22,   -73,   240,   -73,   240,   -73,   -73,    47,
     191,   192,   -73,   190,   240,   123,   -73,    47,   -73,   177,
     -73,   111,    36,   196,   198,   -73,   244,   -20,   -73,   241,
     240,    18,    98,   -73,   -73,   202,   206,   -73,   230,   245,
     224,    47,   -73,   218,   219,   247,   -73,   240,    78,   -73,
     -73,   240,   173,   226,   227,   222,   231,   -73,   -73,    36,
      12,   234,   -73,   -73,   -73,   -73,   228,   119,   -73,   194,
     -73,   -73,   -73
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      20,    22,     0,     2,     4,    18,    19,    64,    61,     0,
       0,     0,     0,    94,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    15,    16,    83,    86,    87,     0,     0,
      21,     0,    23,     1,     0,     6,    42,     0,    58,    59,
       0,     0,    60,     0,    38,     0,    66,    65,     0,     0,
       0,    43,    45,    47,    50,    42,     0,     5,     5,     0,
       5,     5,     0,     0,     0,    79,     0,     0,    74,    76,
      84,     0,     3,    41,     0,     0,    95,     0,     0,    93,
       0,     0,     0,    51,     0,     0,     0,     0,    67,    68,
      69,    70,    71,    72,     0,    54,    27,     0,     0,     0,
       0,     0,    31,    37,     0,     0,     0,     0,     0,     0,
      85,     0,    88,    89,    90,     0,    82,     0,     0,     0,
      56,     0,    63,     5,     8,    48,     0,    52,    53,    44,
      46,    55,     0,    25,     5,    10,     5,    12,    13,     0,
       0,     0,    14,    28,     5,     0,    17,     0,    77,    73,
      75,     0,     7,     0,     0,    62,     0,     0,    49,     0,
       5,     0,     0,    32,    33,     0,     0,    34,     0,     0,
       0,     0,    80,     0,     0,     0,    91,     5,    26,     9,
      11,     5,     0,     0,     0,     0,     0,    40,    57,     0,
      27,     0,    30,    35,    36,    78,     0,     0,    24,     0,
      81,    92,    29
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -73,   -73,   -54,   -40,    79,   -73,   -73,   -73,   166,     2,
     -18,     3,   -23,   184,   187,   232,    -1,   -73,   -73,   203,
     165,    -8,   -72,   -73,   -73,   -73,   236
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,   144,   170,   116,   175,   117,    23,
      60,    27,    61,    62,    63,    64,    83,    66,   104,    77,
      78,    79,    41,   125,    15,   133,    16
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      24,    45,    40,    42,    82,   121,    32,    24,    29,    30,
      33,   145,    47,   106,   107,    75,   109,   110,    46,   126,
     142,   143,    54,    17,    44,    65,    68,    69,   189,   129,
      48,   149,    54,   186,    17,    91,    48,   130,    96,   130,
      17,    44,   146,    18,    86,    19,    25,    26,   105,    43,
      80,    35,    17,   135,    18,    49,    44,    65,    65,   121,
      18,    49,    44,    76,    20,    46,   118,   127,    24,   136,
      21,   108,    18,    22,    36,    37,   137,   138,    50,   167,
      51,    31,    52,    87,    72,    24,    51,    31,    52,    17,
     171,    65,   172,    46,    46,    65,    65,    81,    38,    21,
     176,    39,   147,   141,    17,    96,   157,    54,   148,    18,
     162,    17,   190,    28,   158,   154,   188,   160,   155,    34,
     168,   164,    44,    17,    18,    56,    57,    46,    53,    55,
      35,    18,    44,   200,    94,    95,    31,   201,    44,    58,
      59,   164,    44,    18,    70,   169,   177,    56,    57,   180,
      67,    31,    82,    36,    37,   163,   181,    71,    31,    73,
     112,    92,    74,   211,   182,    85,   113,    36,    37,    82,
      31,   178,   179,   196,    84,    90,     1,    38,     2,     3,
      39,   207,    36,    37,     4,   114,   115,   120,     5,     6,
       7,    89,     8,     9,    10,    11,    96,     1,    46,     2,
       3,    97,   165,   111,   119,     4,   122,   123,   124,     5,
       6,     7,   128,     8,     9,    10,    11,   202,     1,   132,
       2,     3,   152,   150,   134,   156,     4,   112,   161,   151,
       5,     6,     7,   113,     8,     9,    10,    11,   212,   173,
     155,   174,   183,     1,   184,     2,     3,   185,   187,   191,
     192,     4,   114,   115,   193,     5,     6,     7,   166,     8,
       9,    10,    11,    98,    99,   100,   101,   102,   103,   194,
     195,   197,   198,   199,   203,   205,   204,   206,   209,   208,
     139,   210,   153,     0,   140,     0,   159,    88,     0,     0,
     131,    93
};

static const yytype_int16 yycheck[] =
{
       1,    19,    10,    11,    44,    77,     7,     8,     5,     6,
       8,    14,    20,    67,    68,     4,    70,    71,    19,     4,
       8,     9,    23,     4,    44,    26,    12,    13,    10,    45,
       4,    45,    33,    53,     4,    58,     4,    53,    32,    53,
       4,    44,    12,    24,     3,    26,     3,    25,    66,     0,
       4,     4,     4,    47,    24,    29,    44,    58,    59,   131,
      24,    29,    44,    52,    45,    66,    74,    52,    69,    92,
      51,    69,    24,    54,    27,    28,    94,    95,    52,   133,
      54,    51,    56,    42,    52,    86,    54,    51,    56,     4,
     144,    92,   146,    94,    95,    96,    97,    51,    51,    51,
     154,    54,    14,   104,     4,    32,    45,   108,    14,    24,
     128,     4,    14,     3,    53,    47,   170,   125,    50,     3,
      47,   129,    44,     4,    24,    40,    41,   128,    47,    45,
       4,    24,    44,   187,    34,    35,    51,   191,    44,    54,
      55,   149,    44,    24,    12,   142,    23,    40,    41,   157,
       7,    51,   192,    27,    28,    29,    45,    12,    51,    13,
      23,    54,    45,    44,    53,     3,    29,    27,    28,   209,
      51,    48,    49,   181,    46,     4,     3,    51,     5,     6,
      54,   199,    27,    28,    11,    48,    49,    47,    15,    16,
      17,    52,    19,    20,    21,    22,    32,     3,   199,     5,
       6,    33,    47,     3,     3,    11,    29,    30,    31,    15,
      16,    17,    26,    19,    20,    21,    22,    44,     3,    47,
       5,     6,    18,    24,    46,    46,    11,    23,     3,    24,
      15,    16,    17,    29,    19,    20,    21,    22,    44,    48,
      50,    49,    46,     3,    46,     5,     6,     3,     7,    47,
      44,    11,    48,    49,    24,    15,    16,    17,    43,    19,
      20,    21,    22,    34,    35,    36,    37,    38,    39,    24,
      46,    53,    53,    26,    48,    53,    49,    46,    44,   200,
      96,    53,   116,    -1,    97,    -1,   121,    51,    -1,    -1,
      87,    59
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     6,    11,    15,    16,    17,    19,    20,
      21,    22,    58,    59,    60,    81,    83,     4,    24,    26,
      45,    51,    54,    66,    73,     3,    25,    68,     3,    68,
      68,    51,    73,    66,     3,     4,    27,    28,    51,    54,
      78,    79,    78,     0,    44,    67,    73,    78,     4,    29,
      52,    54,    56,    47,    73,    45,    40,    41,    54,    55,
      67,    69,    70,    71,    72,    73,    74,     7,    12,    13,
      12,    12,    52,    13,    45,     4,    52,    76,    77,    78,
       4,    51,    60,    73,    46,     3,     3,    42,    83,    52,
       4,    69,    54,    72,    34,    35,    32,    33,    34,    35,
      36,    37,    38,    39,    75,    67,    59,    59,    66,    59,
      59,     3,    23,    29,    48,    49,    63,    65,    78,     3,
      47,    79,    29,    30,    31,    80,     4,    52,    26,    45,
      53,    76,    47,    82,    46,    47,    69,    67,    67,    70,
      71,    73,     8,     9,    61,    14,    12,    14,    14,    45,
      24,    24,    18,    65,    47,    50,    46,    45,    53,    77,
      78,     3,    67,    29,    78,    47,    43,    59,    47,    68,
      62,    59,    59,    48,    49,    64,    59,    23,    48,    49,
      78,    45,    53,    46,    46,     3,    53,     7,    59,    10,
      14,    47,    44,    24,    24,    46,    78,    53,    53,    26,
      59,    59,    44,    48,    49,    53,    46,    67,    61,    44,
      53,    44,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    62,    61,    61,    64,    63,
      63,    65,    65,    65,    65,    65,    65,    65,    66,    66,
      66,    67,    67,    68,    69,    69,    70,    70,    71,    71,
      71,    71,    72,    72,    72,    72,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    74,    74,    75,    75,    75,
      75,    75,    75,    76,    76,    77,    77,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    80,    80,
      80,    81,    82,    82,    83,    83
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     1,     0,     3,     6,     5,     7,
       5,     7,     5,     5,     5,     2,     2,     5,     1,     1,
       1,     2,     1,     2,     5,     0,     3,     0,     0,     7,
       5,     1,     3,     3,     3,     5,     5,     1,     2,     1,
       7,     2,     1,     2,     3,     1,     3,     1,     3,     4,
       1,     2,     3,     3,     2,     3,     4,     7,     2,     2,
       2,     1,     5,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     4,     7,     2,
       5,     8,     3,     1,     2,     3,     1,     1,     1,     1,
       1,     7,     6,     0,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 2:
#line 93 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad);}
#line 1521 "src/parser.c"
    break;

  case 3:
#line 98 "gen/parser.y"
{ (yyval.quad) = quad_new(0, instr_op, NULL, NULL, NULL); }
#line 1527 "src/parser.c"
    break;

  case 4:
#line 100 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1533 "src/parser.c"
    break;

  case 5:
#line 102 "gen/parser.y"
{ (yyval.quad) = NULL; }
#line 1539 "src/parser.c"
    break;

  case 6:
#line 107 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_instr_op, quadarg_new_id((yyvsp[-2].id)), (yyvsp[0].quad)->arg3, NULL); }
#line 1545 "src/parser.c"
    break;

  case 7:
#line 109 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_array_instr_op, quadarg_new_id((yyvsp[-5].id)), (yyvsp[-3].quad)->arg3, (yyvsp[0].quad)->arg3); }
#line 1551 "src/parser.c"
    break;

  case 8:
#line 111 "gen/parser.y"
{ (yyval.quad) = quad_new(0, declare_array_instr_op, quadarg_new_id((yyvsp[-3].id)), quadarg_new_int((yyvsp[-1].integer_t)), NULL); }
#line 1557 "src/parser.c"
    break;

  case 9:
#line 113 "gen/parser.y"
{ if ((yyvsp[-2].quad) == NULL)
    (yyval.quad) = quad_new(0, if_instr_op, (yyvsp[-5].quad)->arg3, NULL, NULL); 
  else 
    (yyval.quad) = quad_new(0, if_instr_op, (yyvsp[-5].quad)->arg3, (yyvsp[-2].quad)->arg3, NULL); }
#line 1566 "src/parser.c"
    break;

  case 10:
#line 118 "gen/parser.y"
{ (yyval.quad) = quad_new(0, for_instr_op, quadarg_new_id((yyvsp[-3].id)), (yyvsp[-1].quad)->arg3, NULL); }
#line 1572 "src/parser.c"
    break;

  case 11:
#line 120 "gen/parser.y"
{ struct quad *marker = quad_new(0, in_instr_op, quadarg_new_id((yyvsp[-5].id)), (yyvsp[-3].quad)->arg3, NULL);
  (yyval.quad) = quad_new(0, for_instr_op, marker->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1579 "src/parser.c"
    break;

  case 12:
#line 123 "gen/parser.y"
{ (yyval.quad) = quad_new(0, while_instr_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1585 "src/parser.c"
    break;

  case 13:
#line 125 "gen/parser.y"
{ (yyval.quad) = quad_new(0, until_instr_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1591 "src/parser.c"
    break;

  case 14:
#line 127 "gen/parser.y"
{ (yyval.quad) = quad_new(0, case_instr_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, NULL); }
#line 1597 "src/parser.c"
    break;

  case 15:
#line 129 "gen/parser.y"
{ (yyval.quad) = quad_new(0, echo_instr_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1603 "src/parser.c"
    break;

  case 16:
#line 131 "gen/parser.y"
{ (yyval.quad) = quad_new(0, read_instr_op, quadarg_new_id((yyvsp[0].id)), NULL, NULL); }
#line 1609 "src/parser.c"
    break;

  case 17:
#line 133 "gen/parser.y"
{ (yyval.quad) = quad_new(0, read_array_instr_op, quadarg_new_id((yyvsp[-3].id)), (yyvsp[-1].quad)->arg3, NULL); }
#line 1615 "src/parser.c"
    break;

  case 18:
#line 135 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1621 "src/parser.c"
    break;

  case 19:
#line 137 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1627 "src/parser.c"
    break;

  case 20:
#line 139 "gen/parser.y"
{ (yyval.quad) = quad_new(0, return_void_op, NULL, NULL, NULL); }
#line 1633 "src/parser.c"
    break;

  case 21:
#line 141 "gen/parser.y"
{ (yyval.quad) = quad_new(0, return_int_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1639 "src/parser.c"
    break;

  case 22:
#line 143 "gen/parser.y"
{ (yyval.quad) = quad_new(0, exit_void_op, NULL, NULL, NULL); }
#line 1645 "src/parser.c"
    break;

  case 23:
#line 145 "gen/parser.y"
{ (yyval.quad) = quad_new(0, exit_int_op, (yyvsp[0].quad)->arg3, NULL, NULL); }
#line 1651 "src/parser.c"
    break;

  case 24:
#line 150 "gen/parser.y"
{ struct quad *marker = quad_new(0, testing_op, (yyvsp[-3].quad)->arg3, (yyvsp[-1].quad)->arg3, quadarg_new_reg());
  (yyval.quad) = quad_new(0, elif_op, marker->arg3, (yyvsp[-1].quad)->arg3, quadarg_new_reg()); }
#line 1658 "src/parser.c"
    break;

  case 25:
#line 152 "gen/parser.y"
       { quad_new(0, else_op, NULL, NULL, quadarg_new_reg()); }
#line 1664 "src/parser.c"
    break;

  case 26:
#line 154 "gen/parser.y"
{ (yyval.quad) = quad_new(0, else_end_op, NULL, NULL, quadarg_new_reg()); }
#line 1670 "src/parser.c"
    break;

  case 27:
#line 156 "gen/parser.y"
{ (yyval.quad) = NULL; }
#line 1676 "src/parser.c"
    break;

  case 28:
#line 161 "gen/parser.y"
{ (yyvsp[0].filter_t) = calloc(1, sizeof(struct filter_s));
  (yyvsp[0].filter_t)->array_string = malloc(sizeof(char *)); 
  (yyvsp[0].filter_t)->size = 0; }
#line 1684 "src/parser.c"
    break;

  case 29:
#line 165 "gen/parser.y"
{ struct quad *marker = quad_new(0, filter_instr, quadarg_new_array_str((yyvsp[-5].filter_t)->array_string), (yyvsp[-2].quad)->arg3, quadarg_new_reg()); 
  (yyval.quad) = quad_new(0, cases_op, (yyvsp[-6].quad)->arg3, marker->arg3, quadarg_new_reg()); }
#line 1691 "src/parser.c"
    break;

  case 30:
#line 168 "gen/parser.y"
{ (yyval.quad) = quad_new(0, filter_instr, quadarg_new_array_str((yyvsp[-4].filter_t)->array_string), (yyvsp[-2].quad)->arg3, quadarg_new_reg()); }
#line 1697 "src/parser.c"
    break;

  case 31:
#line 173 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[0].string));
  (yyval.filter_t)->size++; }
#line 1704 "src/parser.c"
    break;

  case 32:
#line 176 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1711 "src/parser.c"
    break;

  case 33:
#line 179 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1718 "src/parser.c"
    break;

  case 34:
#line 182 "gen/parser.y"
{ (yyval.filter_t) = (yyvsp[-2].filter_t);
  (yyval.filter_t)->array_string = realloc((yyval.filter_t)->array_string, sizeof(char *) * ((yyval.filter_t)->size + 1));
  (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[0].string));
  (yyval.filter_t)->size++; }
#line 1727 "src/parser.c"
    break;

  case 35:
#line 187 "gen/parser.y"
{ (yyval.filter_t) = (yyvsp[-4].filter_t);
  (yyval.filter_t)->array_string = realloc((yyval.filter_t)->array_string, sizeof(char *) * ((yyval.filter_t)->size + 1));
  (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1736 "src/parser.c"
    break;

  case 36:
#line 192 "gen/parser.y"
{ (yyval.filter_t) = (yyvsp[-4].filter_t);
  (yyval.filter_t)->array_string = realloc((yyval.filter_t)->array_string, sizeof(char *) * ((yyval.filter_t)->size + 1));
  (yyval.filter_t)->array_string[(yyval.filter_t)->size] = strdup((yyvsp[-1].string));
  (yyval.filter_t)->size++; }
#line 1745 "src/parser.c"
    break;

  case 37:
#line 197 "gen/parser.y"
{ (yyval.filter_t)->array_string[(yyval.filter_t)->size] = "*"; 
  (yyval.filter_t)->size++; }
#line 1752 "src/parser.c"
    break;

  case 38:
#line 203 "gen/parser.y"
{ (yyval.quad) = quad_new(0, concat_op, (yyvsp[-1].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1758 "src/parser.c"
    break;

  case 39:
#line 205 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1764 "src/parser.c"
    break;

  case 40:
#line 207 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_array_to_var_ops, quadarg_new_id((yyvsp[-4].id)), NULL, quadarg_new_reg()); }
#line 1770 "src/parser.c"
    break;

  case 41:
#line 212 "gen/parser.y"
{ (yyval.quad) = quad_new(0, concat_op, (yyvsp[-1].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1776 "src/parser.c"
    break;

  case 42:
#line 214 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1782 "src/parser.c"
    break;

  case 43:
#line 219 "gen/parser.y"
{ (yyval.quad) = quad_new(0, test_op, (yyvsp[0].quad)->arg3, NULL, quadarg_new_reg()); }
#line 1788 "src/parser.c"
    break;

  case 44:
#line 224 "gen/parser.y"
{ (yyval.quad) = quad_new(0, or_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1794 "src/parser.c"
    break;

  case 45:
#line 226 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1800 "src/parser.c"
    break;

  case 46:
#line 231 "gen/parser.y"
{ (yyval.quad) = quad_new(0, and_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1806 "src/parser.c"
    break;

  case 47:
#line 233 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1812 "src/parser.c"
    break;

  case 48:
#line 238 "gen/parser.y"
{ (yyval.quad) = (yyvsp[-1].quad); }
#line 1818 "src/parser.c"
    break;

  case 49:
#line 240 "gen/parser.y"
{ (yyval.quad) = quad_new(0, not_op, (yyvsp[-1].quad)->arg3, NULL, quadarg_new_reg()); }
#line 1824 "src/parser.c"
    break;

  case 50:
#line 242 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1830 "src/parser.c"
    break;

  case 51:
#line 244 "gen/parser.y"
{ (yyval.quad) = quad_new(0, not_op, (yyvsp[0].quad)->arg3, NULL, quadarg_new_reg()); }
#line 1836 "src/parser.c"
    break;

  case 52:
#line 249 "gen/parser.y"
{ (yyval.quad) = quad_new(0, eq_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1842 "src/parser.c"
    break;

  case 53:
#line 251 "gen/parser.y"
{ (yyval.quad) = quad_new(0, neq_op, (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1848 "src/parser.c"
    break;

  case 54:
#line 253 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer_t), (yyvsp[0].quad)->arg3, NULL, quadarg_new_reg()); }
#line 1854 "src/parser.c"
    break;

  case 55:
#line 255 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer_t), (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1860 "src/parser.c"
    break;

  case 56:
#line 260 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_id_to_var_op, quadarg_new_id((yyvsp[-1].id)), NULL, quadarg_new_reg()); }
#line 1866 "src/parser.c"
    break;

  case 57:
#line 262 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_id_to_var_op, quadarg_new_id((yyvsp[-4].id)), (yyvsp[-2].quad)->arg3, quadarg_new_reg()); }
#line 1872 "src/parser.c"
    break;

  case 58:
#line 264 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_arg_to_var_op, quadarg_new_int((yyvsp[0].integer_t)), NULL, quadarg_new_reg()); }
#line 1878 "src/parser.c"
    break;

  case 59:
#line 266 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_all_arg_to_var_op, NULL, NULL, quadarg_new_reg()); }
#line 1884 "src/parser.c"
    break;

  case 60:
#line 268 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_status_to_var_op, NULL, NULL, quadarg_new_reg()); }
#line 1890 "src/parser.c"
    break;

  case 61:
#line 270 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_string_to_var_op, quadarg_new_str((yyvsp[0].string)), NULL, quadarg_new_reg()); }
#line 1896 "src/parser.c"
    break;

  case 62:
#line 272 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_expr_value_to_var_op, (yyvsp[-1].quad)->arg3, NULL, quadarg_new_reg()); }
#line 1902 "src/parser.c"
    break;

  case 63:
#line 274 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_cfun_to_var_op, (yyvsp[-1].quad)->arg3, NULL, quadarg_new_reg()); }
#line 1908 "src/parser.c"
    break;

  case 64:
#line 276 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_int_to_var_op, quadarg_new_int((yyvsp[0].integer_t)), NULL, quadarg_new_reg()); }
#line 1914 "src/parser.c"
    break;

  case 65:
#line 281 "gen/parser.y"
{ (yyval.integer_t) = nnull_op; }
#line 1920 "src/parser.c"
    break;

  case 66:
#line 283 "gen/parser.y"
{ (yyval.integer_t) = null_op; }
#line 1926 "src/parser.c"
    break;

  case 67:
#line 288 "gen/parser.y"
{ (yyval.integer_t) = eq_op; }
#line 1932 "src/parser.c"
    break;

  case 68:
#line 290 "gen/parser.y"
{ (yyval.integer_t) = neq_op; }
#line 1938 "src/parser.c"
    break;

  case 69:
#line 292 "gen/parser.y"
{ (yyval.integer_t) = gt_op; }
#line 1944 "src/parser.c"
    break;

  case 70:
#line 294 "gen/parser.y"
{ (yyval.integer_t) = ge_op; }
#line 1950 "src/parser.c"
    break;

  case 71:
#line 296 "gen/parser.y"
{ (yyval.integer_t) = lt_op; }
#line 1956 "src/parser.c"
    break;

  case 72:
#line 298 "gen/parser.y"
{ (yyval.integer_t) = le_op; }
#line 1962 "src/parser.c"
    break;

  case 73:
#line 303 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer_t), (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1968 "src/parser.c"
    break;

  case 74:
#line 305 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1974 "src/parser.c"
    break;

  case 75:
#line 310 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-1].integer_t), (yyvsp[-2].quad)->arg3, (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 1980 "src/parser.c"
    break;

  case 76:
#line 312 "gen/parser.y"
{ (yyval.quad) = (yyvsp[0].quad); }
#line 1986 "src/parser.c"
    break;

  case 77:
#line 317 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_id_to_var_op, quadarg_new_id((yyvsp[-1].id)), NULL, quadarg_new_reg()); }
#line 1992 "src/parser.c"
    break;

  case 78:
#line 319 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_id_to_var_op, quadarg_new_id((yyvsp[-4].id)), (yyvsp[-2].quad)->arg3, quadarg_new_reg()); }
#line 1998 "src/parser.c"
    break;

  case 79:
#line 321 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_arg_to_var_op, quadarg_new_int((yyvsp[0].integer_t)), NULL, quadarg_new_reg()); }
#line 2004 "src/parser.c"
    break;

  case 80:
#line 323 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-4].integer_t), quadarg_new_id((yyvsp[-1].id)), NULL, quadarg_new_reg());  }
#line 2010 "src/parser.c"
    break;

  case 81:
#line 325 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-7].integer_t), quadarg_new_id((yyvsp[-4].id)), (yyvsp[-2].quad)->arg3, quadarg_new_reg()); }
#line 2016 "src/parser.c"
    break;

  case 82:
#line 327 "gen/parser.y"
{ (yyval.quad) = quad_new(0, (yyvsp[-2].integer_t), quadarg_new_int((yyvsp[0].integer_t)), ARG, quadarg_new_reg()); }
#line 2022 "src/parser.c"
    break;

  case 83:
#line 329 "gen/parser.y"
{ (yyval.quad) = quad_new(0, assn_int_to_var_op, quadarg_new_int((yyvsp[0].integer_t)), NULL, quadarg_new_reg()); }
#line 2028 "src/parser.c"
    break;

  case 84:
#line 331 "gen/parser.y"
{ (yyval.quad) = quad_new(0,(yyvsp[-1].integer_t), quadarg_new_int((yyvsp[0].integer_t)), NULL, quadarg_new_reg()); }
#line 2034 "src/parser.c"
    break;

  case 85:
#line 333 "gen/parser.y"
{ (yyval.quad) = (yyvsp[-1].quad); }
#line 2040 "src/parser.c"
    break;

  case 86:
#line 338 "gen/parser.y"
{ (yyval.integer_t) = plus_op;}
#line 2046 "src/parser.c"
    break;

  case 87:
#line 340 "gen/parser.y"
{ (yyval.integer_t) = minus_op; }
#line 2052 "src/parser.c"
    break;

  case 88:
#line 345 "gen/parser.y"
{ (yyval.integer_t) = mult_op; }
#line 2058 "src/parser.c"
    break;

  case 89:
#line 347 "gen/parser.y"
{ (yyval.integer_t) = div_op; }
#line 2064 "src/parser.c"
    break;

  case 90:
#line 349 "gen/parser.y"
{ (yyval.integer_t) = mod_op; }
#line 2070 "src/parser.c"
    break;

  case 91:
#line 354 "gen/parser.y"
{ struct quad *marker = quad_new(0, cont_func_op, (yyvsp[-2].quad)->arg3, (yyvsp[-1].quad)->arg3, quadarg_new_reg());
  (yyval.quad) = quad_new(0, dfun_op, quadarg_new_id((yyvsp[-6].id)), marker->arg3, quadarg_new_reg()); }
#line 2077 "src/parser.c"
    break;

  case 92:
#line 360 "gen/parser.y"
{ struct quad *marker = quad_new(0, local_decl_op, (yyvsp[-5].quad)->arg3, quadarg_new_id((yyvsp[-3].id)), quadarg_new_reg());
  (yyval.quad) = quad_new(0, decl_op, quadarg_new_id((yyvsp[-3].id)), marker->arg3, quadarg_new_reg()); }
#line 2084 "src/parser.c"
    break;

  case 93:
#line 363 "gen/parser.y"
{ (yyval.quad) = quad_new(0, decl_op, NULL, NULL, quadarg_new_reg()); }
#line 2090 "src/parser.c"
    break;

  case 94:
#line 368 "gen/parser.y"
{ (yyval.quad) = quad_new(0, cfun_ops, quadarg_new_id((yyvsp[-1].id)), (yyvsp[0].quad)->arg3, quadarg_new_reg()); }
#line 2096 "src/parser.c"
    break;

  case 95:
#line 370 "gen/parser.y"
{ (yyval.quad) = quad_new(0, cfun_op, quadarg_new_id((yyvsp[0].id)), NULL, quadarg_new_reg()); }
#line 2102 "src/parser.c"
    break;


#line 2106 "src/parser.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 373 "gen/parser.y"


void yyerror(const char *s) {
  alert("Syntax error: %s", s);
}
