/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         machineextensionsanalyseurparse
#define yylex           machineextensionsanalyseurlex
#define yyerror         machineextensionsanalyseurerror
#define yydebug         machineextensionsanalyseurdebug
#define yynerrs         machineextensionsanalyseurnerrs


/* Copy the first part of user declarations.  */
#line 20 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:339  */

#include <src/global/global.h>
#define YYDEBUG 0
#include <src/machine/extensions/analyseur/includes.h>

namespace SVM_Extension = SVM::Machine::Extension;

extern int machineextensionsanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description, std::string mesg);
extern int machineextensionsanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
extern int machineextensionsanalyseurlex_init(void *scanner);
extern int machineextensionsanalyseurlex_destroy(void *scanner);


#line 86 "analyseur.syn.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_MACHINEEXTENSIONSANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
# define YY_MACHINEEXTENSIONSANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int machineextensionsanalyseurdebug;
#endif
/* "%code requires" blocks.  */
#line 48 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:355  */


struct ValeurExtension
{
	ValeurExtension()
	:entier(0) {}
	size_t entier;
	std::string chaine;
	std::vector<bool> booleens;
	SVM_Extension::ObjetSP objet;
	std::vector<SVM_Extension::ObjetSP> liste_objets;
	SVM_Valeur::PointEntreeExtension point_entree_extension;
	SVM_Extension::RegexParametresSP regex_parametres;
	SVM_Extension::RegexParametresSequenceSP regex_parametres_sequence;
	SVM_Extension::RegexParametresAlternativeSimpleSP regex_parametres_alternative_simple;
	SVM_Extension::RegexParametresAlternativeSP regex_parametres_alternative;
	SVM_Extension::RegexParametresElementSP regex_parametres_element;
	SVM_Memoire::Type::TypeInterne type_memoire = SVM_Memoire::Type::TypeInterne::AUTOMATIQUE;
};

#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)

#line 146 "analyseur.syn.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _INVALIDE_ = 258,
    FIN_LIGNE = 259,
    ENTIER = 260,
    IDENTIFIANT = 261,
    MOT_CLEF = 262,
    VALEUR_OPTION = 263,
    PLUGIN = 264,
    USE = 265,
    DEFINE = 266,
    TYPE = 267,
    INSTRUCTION = 268,
    STRUCT = 269,
    FUNCTION = 270,
    INTERRUPTION = 271,
    SCHEDULER = 272,
    SEQUENCER = 273,
    OPTION = 274,
    ARGUMENT = 275,
    ARGUMENTS = 276,
    SYSTEM = 277,
    WAITING = 278,
    OVERRIDE = 279,
    RETURN = 280,
    POINT = 281,
    MUTABLE = 282,
    VALUE = 283,
    INT = 284,
    STR = 285,
    BLN = 286,
    PTR = 287,
    LIB = 288,
    SYM = 289,
    PEP = 290,
    IRQ = 291,
    AUTO = 292,
    MARKER = 293,
    INFERIEUR = 294,
    SUPERIEUR = 295,
    INFERIEUR_INFERIEUR = 296,
    SUPERIEUR_SUPERIEUR = 297,
    INFERIEUR_SUPERIEUR = 298,
    DEBUT_ACCOLADE = 299,
    FIN_ACCOLADE = 300,
    VIRGULE = 301,
    POINT_VIRGULE = 302,
    EGAL = 303,
    INFERIEUR_OU_EGAL = 304,
    SUPERIEUR_OU_EGAL = 305,
    KEYWORD = 306,
    VARIABLE = 307,
    STRUCTURE = 308,
    DEBUT_CROCHET = 309,
    DEBUT_PARENTHESE = 310,
    FIN_CROCHET = 311,
    FIN_PARENTHESE = 312,
    PLUS = 313,
    FOIS = 314,
    POINT_INTERROGATION = 315,
    ALTERNATIVE = 316,
    DEUX_POINTS = 317
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  struct ValeurExtension  YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int machineextensionsanalyseurparse (void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description);

#endif /* !YY_MACHINEEXTENSIONSANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 245 "analyseur.syn.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   373

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  153

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   124,   127,   133,   139,   144,   151,   155,
     159,   163,   167,   171,   175,   179,   183,   187,   193,   199,
     205,   211,   217,   223,   229,   235,   241,   247,   253,   257,
     263,   267,   274,   277,   282,   287,   295,   298,   305,   308,
     315,   320,   325,   337,   342,   347,   352,   359,   364,   371,
     376,   384,   387,   394,   398,   402,   407,   411,   415,   421,
     426,   433,   437,   449,   455,   460,   467,   471,   477,   481,
     485,   489,   493,   497,   501,   505,   509,   513,   517,   521,
     525,   531,   537,   541,   545,   549,   553,   557,   561,   565,
     569,   573,   577,   581,   585,   591,   595,   601,   608,   611
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_INVALIDE_", "FIN_LIGNE", "ENTIER",
  "IDENTIFIANT", "MOT_CLEF", "VALEUR_OPTION", "PLUGIN", "USE", "DEFINE",
  "TYPE", "INSTRUCTION", "STRUCT", "FUNCTION", "INTERRUPTION", "SCHEDULER",
  "SEQUENCER", "OPTION", "ARGUMENT", "ARGUMENTS", "SYSTEM", "WAITING",
  "OVERRIDE", "RETURN", "POINT", "MUTABLE", "VALUE", "INT", "STR", "BLN",
  "PTR", "LIB", "SYM", "PEP", "IRQ", "AUTO", "MARKER", "INFERIEUR",
  "SUPERIEUR", "INFERIEUR_INFERIEUR", "SUPERIEUR_SUPERIEUR",
  "INFERIEUR_SUPERIEUR", "DEBUT_ACCOLADE", "FIN_ACCOLADE", "VIRGULE",
  "POINT_VIRGULE", "EGAL", "INFERIEUR_OU_EGAL", "SUPERIEUR_OU_EGAL",
  "KEYWORD", "VARIABLE", "STRUCTURE", "DEBUT_CROCHET", "DEBUT_PARENTHESE",
  "FIN_CROCHET", "FIN_PARENTHESE", "PLUS", "FOIS", "POINT_INTERROGATION",
  "ALTERNATIVE", "DEUX_POINTS", "$accept", "extension", "utilise",
  "declare", "liste_objets", "objet", "type", "instruction", "structure",
  "fonction", "interruption", "ordonnnanceur", "sequenceur", "option",
  "argument", "arguments", "type_option", "type_argument",
  "drapeaux_instructions", "retour", "regex_parametres",
  "regex_parametres_element", "regex_parametres_element_liste_simple",
  "regex_alternative", "regex_alternative_simple",
  "regex_parametres_element_simple", "regex_retour",
  "regex_retour_element", "regex_retour_element_liste_simple",
  "regex_retour_element_simple", "valeur", "point_entree_extension",
  "marqueur", "mot_clef", "variable", "identifiant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317
};
# endif

#define YYPACT_NINF -108

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-108)))

#define YYTABLE_NINF -6

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       7,    13,    22,    44,  -108,    40,    47,    43,   111,    51,
    -108,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      63,  -108,    53,    54,    56,    57,    66,    67,    69,    81,
      82,   115,     2,   111,    41,  -108,  -108,  -108,  -108,  -108,
    -108,    58,     1,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,    50,  -108,  -108,  -108,   352,
     114,   110,    -2,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
     210,  -108,   300,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,    50,   242,
    -108,  -108,     5,    59,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,   110,  -108,   290,  -108,   -13,  -108,  -108,    59,
    -108,  -108,    62,  -108,   -40,   160,    59,    59,    59,    59,
     116,  -108,  -108,     6,  -108,  -108,  -108,  -108,    59,  -108,
      59,  -108,     5,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,   160
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     3,     0,     0,    32,     0,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      32,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,     0,    18,    20,    38,    22,    23,
      24,     0,     0,    27,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    33,    34,    35,    32,
       0,    36,     0,    30,    31,    26,    38,    81,    96,    69,
       0,    53,     0,    68,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    82,    83,    86,    84,    87,    85,    88,
      89,    90,    91,    92,    93,    94,    95,    97,     0,     0,
      51,    21,    39,    98,    54,    79,    56,    57,    58,    28,
      25,    29,    36,    66,     0,    37,    98,    61,    67,    98,
      55,    80,     0,    47,     0,    49,    98,    98,    98,    98,
       0,    40,    19,     0,    64,    63,    59,    60,    98,    48,
      98,    51,    52,    46,    45,    44,    43,    99,    62,    65,
      42,    41,    50
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -108,  -108,  -108,  -108,   100,    -7,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,    72,  -108,    60,
     102,  -107,  -108,  -108,    28,   -76,  -108,  -108,  -108,  -100,
     -61,   -11,  -108,  -108,   101,   222
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     7,    10,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,   110,    65,    32,   101,
      61,   102,   122,   124,   125,   103,   115,   116,   133,   117,
     104,   105,   106,   107,   108,   131
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      35,    36,    37,    38,    39,    40,    41,    42,    43,   118,
     126,   120,    34,    44,   134,    55,     1,   140,   142,     3,
      69,   141,     4,   123,    56,    57,    58,    63,    64,   109,
      63,    64,   113,   149,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    66,   142,   139,   135,     5,   130,
       6,     8,    44,   118,     9,    33,    34,    45,    46,    98,
      47,    48,   148,   127,   128,   129,    62,    60,    34,    68,
      49,    50,   118,    51,    -4,    11,    69,    12,    13,    14,
      15,    16,    17,    18,    19,    52,    53,   121,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    34,    68,   138,    54,
      67,   130,   147,    11,    69,    12,    13,    14,    15,    16,
      17,    18,    19,    59,   111,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    34,    68,   112,   152,
       0,   119,   132,     0,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    34,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,     0,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    34,    68,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,     0,    97,    98,   114,     0,     0,     0,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    34,     0,     0,     0,
       0,     0,     0,     0,    69,     0,    34,     0,     0,     0,
       0,     0,     0,     0,    69,     0,   113,     0,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,   136,     0,
       0,   137,     0,    98,     0,     0,     0,     0,   143,   144,
     145,   146,    -5,    98,     0,     0,     0,     0,     0,     0,
     150,     0,   151,     0,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19
};

static const yytype_int16 yycheck[] =
{
      11,    12,    13,    14,    15,    16,    17,    18,    19,    70,
       5,    72,     6,    20,   114,    13,     9,    57,   125,     6,
      14,    61,     0,    99,    22,    23,    24,    29,    30,    31,
      29,    30,    26,   133,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    55,   152,   122,    60,     4,    62,
      10,     4,    59,   114,    11,     4,     6,     4,     4,    53,
       4,     4,    56,    58,    59,    60,     8,    26,     6,     7,
       4,     4,   133,     4,    11,    12,    14,    14,    15,    16,
      17,    18,    19,    20,    21,     4,     4,    98,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,     6,     7,    56,     4,
       6,    62,     6,    12,    14,    14,    15,    16,    17,    18,
      19,    20,    21,    33,    62,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,     6,     7,    66,   141,
      -1,    70,   112,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    52,    53,    54,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    -1,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,   116,    -1,
      -1,   119,    -1,    53,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,     0,    53,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,   140,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    64,     6,     0,     4,    10,    65,     4,    11,
      66,    12,    14,    15,    16,    17,    18,    19,    20,    21,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    81,     4,     6,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    68,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,    13,    22,    23,    24,    67,
      26,    83,     8,    29,    30,    80,    94,     6,     7,    14,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    82,    84,    88,    93,    94,    95,    96,    97,    31,
      79,    80,    83,    26,    54,    89,    90,    92,    93,    97,
      93,    94,    85,    88,    86,    87,     5,    58,    59,    60,
      62,    98,    82,    91,    92,    60,    98,    98,    56,    88,
      57,    61,    84,    98,    98,    98,    98,     6,    56,    92,
      98,    98,    87
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    65,    65,    66,    67,    67,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    79,
      80,    80,    81,    81,    81,    81,    82,    82,    83,    83,
      84,    84,    84,    84,    84,    84,    84,    85,    85,    86,
      86,    87,    87,    88,    88,    88,    88,    88,    88,    89,
      89,    90,    90,    90,    91,    91,    92,    92,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    94,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    96,    96,    97,    98,    98
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     0,     3,     3,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     5,
       2,     4,     2,     2,     2,     4,     3,     2,     1,     1,
       1,     1,     0,     2,     2,     2,     0,     2,     0,     2,
       2,     4,     4,     3,     3,     3,     3,     1,     2,     1,
       3,     0,     2,     1,     1,     2,     1,     1,     1,     2,
       2,     1,     3,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (&yylloc, scanner, nom, description, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner, nom, description); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (nom);
  YYUSE (description);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, nom, description);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, nom, description);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, nom, description); \
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (nom);
  YYUSE (description);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 42 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1429  */
{ 
#if YYDEBUG==1
	machineextensionsanalyseurdebug=1;
#endif
}

#line 1422 "analyseur.syn.cpp" /* yacc.c:1429  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = yylex (&yylval, &yylloc, scanner);
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 116 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	description->_nom = (yyvsp[-3].chaine);
	description->_utilise = (yyvsp[-1].liste_objets);
	description->_declare = (yyvsp[0].liste_objets);
}
#line 1615 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 124 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_objets) = std::vector<SVM_Extension::ObjetSP>();
}
#line 1623 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 128 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_objets) = (yyvsp[0].liste_objets);
}
#line 1631 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 134 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_objets) = (yyvsp[0].liste_objets);
}
#line 1639 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 140 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_objets) = std::vector<SVM_Extension::ObjetSP>();
	(yyval.liste_objets).push_back((yyvsp[0].objet));
}
#line 1648 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 145 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_objets) = std::move((yyvsp[-1].liste_objets));
	(yyval.liste_objets).push_back((yyvsp[0].objet));
}
#line 1657 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 152 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1665 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 156 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1673 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 160 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1681 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 164 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1689 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 168 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1697 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 172 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1705 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 176 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1713 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 180 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1721 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 184 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1729 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 188 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = (yyvsp[-1].objet);
}
#line 1737 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 194 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Type>((yyloc).first_line,(yyvsp[0].point_entree_extension));
}
#line 1745 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 200 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Instruction>((yyloc).first_line,(yyvsp[-2].point_entree_extension),(yyvsp[-1].regex_parametres_sequence),(yyvsp[0].regex_parametres),(yyvsp[-4].booleens)[0],(yyvsp[-4].booleens)[1],(yyvsp[-4].booleens)[2]);
}
#line 1753 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 206 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Structure>((yyloc).first_line,(yyvsp[0].point_entree_extension));
}
#line 1761 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 212 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Fonction>((yyloc).first_line,(yyvsp[-2].point_entree_extension),(yyvsp[-1].regex_parametres_sequence),(yyvsp[0].regex_parametres));
}
#line 1769 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 218 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Interruption>((yyloc).first_line,(yyvsp[0].point_entree_extension));
}
#line 1777 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 224 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Ordonnanceur>((yyloc).first_line,(yyvsp[0].point_entree_extension));
}
#line 1785 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 230 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Sequenceur>((yyloc).first_line,(yyvsp[0].point_entree_extension));
}
#line 1793 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 236 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Option>((yyloc).first_line,(yyvsp[-2].point_entree_extension),(yyvsp[-1].chaine),(yyvsp[0].type_memoire));
}
#line 1801 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 242 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Argument>((yyloc).first_line,(yyvsp[-1].point_entree_extension),description->_indice_argument++,(yyvsp[0].type_memoire));
}
#line 1809 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 248 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.objet) = std::make_shared<SVM_Extension::Arguments>((yyloc).first_line,(yyvsp[0].point_entree_extension));
}
#line 1817 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 254 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_memoire) = SVM_Memoire::Type::TypeInterne::BOOLEEN;
}
#line 1825 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 258 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_memoire) = (yyvsp[0].type_memoire);
}
#line 1833 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 264 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_memoire) = SVM_Memoire::Type::TypeInterne::ENTIER;
}
#line 1841 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 268 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_memoire) = SVM_Memoire::Type::TypeInterne::CHAINE;
}
#line 1849 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 274 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleens) = { false, false, false };
}
#line 1857 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 278 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleens) = (yyvsp[-1].booleens);
	(yyval.booleens)[0] = true;
}
#line 1866 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 283 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleens) = (yyvsp[-1].booleens);
	(yyval.booleens)[1] = true;
}
#line 1875 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 288 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleens) = (yyvsp[-1].booleens);
	(yyval.booleens)[2] = true;
}
#line 1884 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 295 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = SVM_Extension::RegexParametresSP();
}
#line 1892 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 299 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[0].regex_parametres);
}
#line 1900 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 305 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_sequence) = std::make_shared<SVM_Extension::RegexParametresSequence>();
}
#line 1908 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 309 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_sequence) = (yyvsp[-1].regex_parametres_sequence);
	(yyval.regex_parametres_sequence)->_elements.push_back((yyvsp[0].regex_parametres));
}
#line 1917 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 316 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[-1].regex_parametres_element);
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1926 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 321 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[-2].regex_parametres_alternative);
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1935 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 326 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[-2].regex_parametres_alternative_simple);
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1944 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 338 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = std::make_shared<SVM_Extension::RegexParametresOptionnel>((yyvsp[-2].regex_parametres));
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1953 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 343 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = std::make_shared<SVM_Extension::RegexParametresRepetitionOptionnel>((yyvsp[-2].regex_parametres));
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1962 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 348 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = std::make_shared<SVM_Extension::RegexParametresRepetitionObligatoire>((yyvsp[-2].regex_parametres));
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1971 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 353 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = std::make_shared<SVM_Extension::RegexParametresRepetitionFixe>((yyvsp[-2].regex_parametres),(yyvsp[-1].entier));
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 1980 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 360 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_alternative_simple) = std::make_shared<SVM_Extension::RegexParametresAlternativeSimple>();
	(yyval.regex_parametres_alternative_simple)->_elements.push_back((yyvsp[0].regex_parametres_element));
}
#line 1989 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 365 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_alternative_simple) = (yyvsp[-1].regex_parametres_alternative_simple);
	(yyval.regex_parametres_alternative_simple)->_elements.push_back((yyvsp[0].regex_parametres_element));
}
#line 1998 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 372 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_alternative) = std::make_shared<SVM_Extension::RegexParametresAlternative>();
	(yyval.regex_parametres_alternative)->_alternatives.push_back((yyvsp[0].regex_parametres_sequence));
}
#line 2007 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 377 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_alternative) = (yyvsp[-2].regex_parametres_alternative);
	(yyval.regex_parametres_alternative)->_alternatives.push_back((yyvsp[0].regex_parametres_sequence));
}
#line 2016 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 384 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_sequence) = std::make_shared<SVM_Extension::RegexParametresSequence>();
}
#line 2024 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 388 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_sequence) = (yyvsp[-1].regex_parametres_sequence);
	(yyval.regex_parametres_sequence)->_elements.push_back((yyvsp[0].regex_parametres));
}
#line 2033 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 395 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerTotal>();
}
#line 2041 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 399 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = (yyvsp[0].regex_parametres_element);
}
#line 2049 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 403 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = (yyvsp[0].regex_parametres_element);
	(yyval.regex_parametres_element)->_modifiable=true;
}
#line 2058 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 408 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = (yyvsp[0].regex_parametres_element);
}
#line 2066 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 412 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = (yyvsp[0].regex_parametres_element);
}
#line 2074 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 416 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = (yyvsp[0].regex_parametres_element);
}
#line 2082 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 422 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[-1].regex_parametres);
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 2091 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 427 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[-1].regex_parametres_element);
	(yyval.regex_parametres)->_nom = (yyvsp[0].chaine);
}
#line 2100 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 434 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[0].regex_parametres_element);
}
#line 2108 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 438 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = (yyvsp[-1].regex_parametres_alternative_simple);
}
#line 2116 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 450 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres) = std::make_shared<SVM_Extension::RegexParametresOptionnel>((yyvsp[-1].regex_parametres));
}
#line 2124 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 456 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_alternative_simple) = std::make_shared<SVM_Extension::RegexParametresAlternativeSimple>();
	(yyval.regex_parametres_alternative_simple)->_elements.push_back((yyvsp[0].regex_parametres_element));
}
#line 2133 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 461 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_alternative_simple) = (yyvsp[-1].regex_parametres_alternative_simple);
	(yyval.regex_parametres_alternative_simple)->_elements.push_back((yyvsp[0].regex_parametres_element));
}
#line 2142 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 468 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerTotal>();
}
#line 2150 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 472 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = (yyvsp[0].regex_parametres_element);
}
#line 2158 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 478 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerPartielValeur>();
}
#line 2166 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 482 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerPartielStructure>();
}
#line 2174 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 486 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::ENTIER));
}
#line 2182 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 490 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE));
}
#line 2190 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 494 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BOOLEEN));
}
#line 2198 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 498 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR));
}
#line 2206 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 502 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE));
}
#line 2214 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 506 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::SYMBOLE));
}
#line 2222 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 510 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION));
}
#line 2230 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 514 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION));
}
#line 2238 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 518 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::AUTOMATIQUE));
}
#line 2246 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 522 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresValeur>(SVM_Memoire::Type((yyvsp[0].point_entree_extension)));
}
#line 2254 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 526 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresStructure>(SVM_Memoire::Type((yyvsp[0].point_entree_extension)));
}
#line 2262 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 532 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.point_entree_extension) = SVM_Valeur::PointEntreeExtension((yyvsp[-2].chaine),(yyvsp[0].chaine));
}
#line 2270 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 538 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerPartielMarqueur>();
}
#line 2278 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 542 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR);
}
#line 2286 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 546 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR_INFERIEUR);
}
#line 2294 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 550 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR_SUPERIEUR);
}
#line 2302 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 554 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::SUPERIEUR);
}
#line 2310 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 558 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::SUPERIEUR_SUPERIEUR);
}
#line 2318 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 562 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::DEBUT_ACCOLADE);
}
#line 2326 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 566 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::FIN_ACCOLADE);
}
#line 2334 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 570 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::VIRGULE);
}
#line 2342 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 574 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::POINT_VIRGULE);
}
#line 2350 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 578 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::EGAL);
}
#line 2358 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 582 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR_OU_EGAL);
}
#line 2366 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 586 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMarqueur>(SVM_Valeur::ParametreMarqueur::Marqueur::SUPERIEUR_OU_EGAL);
}
#line 2374 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 592 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerPartielMotClef>();
}
#line 2382 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 596 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresMotClef>((yyvsp[0].chaine));
}
#line 2390 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 602 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.regex_parametres_element) = std::make_shared<SVM_Extension::RegexParametresJokerPartielVariable>();
}
#line 2398 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 608 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = std::string();
}
#line 2406 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 612 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2414 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;


#line 2418 "analyseur.syn.cpp" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
      yyerror (&yylloc, scanner, nom, description, YY_("syntax error"));
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (&yylloc, scanner, nom, description, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, scanner, nom, description);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scanner, nom, description);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, scanner, nom, description, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, nom, description);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, nom, description);
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
#line 617 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1906  */


int machineextensionsanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description, std::string mesg)
{
	std::ostringstream oss;
	oss << "Unable to parse configuration string of library " << nom << ": " << mesg; 
	description->_message = oss.str();
	description->_ligne = llocp->first_line;
	description->_erreur = true; 
	return 1;
}

