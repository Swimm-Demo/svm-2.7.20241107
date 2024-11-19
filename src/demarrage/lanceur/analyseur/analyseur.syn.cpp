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
#define yyparse         demarragelanceuranalyseurparse
#define yylex           demarragelanceuranalyseurlex
#define yyerror         demarragelanceuranalyseurerror
#define yydebug         demarragelanceuranalyseurdebug
#define yynerrs         demarragelanceuranalyseurnerrs


/* Copy the first part of user declarations.  */
#line 20 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:339  */

#include <src/global/global.h>
#define YYDEBUG 0
#include <src/demarrage/lanceur/analyseur/includes.h>

extern int demarragelanceuranalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur, std::string mesg);
extern int demarragelanceuranalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
extern int demarragelanceuranalyseurlex_init(void *scanner);
extern int demarragelanceuranalyseurlex_destroy(void *scanner);


#line 84 "analyseur.syn.cpp" /* yacc.c:339  */

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
#ifndef YY_DEMARRAGELANCEURANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
# define YY_DEMARRAGELANCEURANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int demarragelanceuranalyseurdebug;
#endif
/* "%code requires" blocks.  */
#line 45 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:355  */


struct ValeurLanceur
{
	size_t entier;
	std::string chaine;
	bool booleen;
	SHARED_PTR(std::string) chaine_sp;
	SVM::Demarrage::Lanceur::TypeOptions type_options;
	SVM::Demarrage::Lanceur::HistoriqueSP historique;
	SVM::Demarrage::Lanceur::SelecteurSP selecteur;
	SVM::Machine::CodeSP code;
	SVM::Machine::Element::Valeur::PointEntreeExtensionSP nom_extension;
	std::vector<SVM::Demarrage::Lanceur::SelecteurSP> memoire;
	SVM::Demarrage::Lanceur::DebugueurSP debugueur;
	SVM::Demarrage::Lanceur::PerformanceSP performance;
	SVM::Demarrage::Lanceur::ParametreExtensionSP parametre_extension;
	std::vector<SVM::Demarrage::Lanceur::ParametreExtensionOption> liste_option_extension;
	std::vector<SVM::Demarrage::Lanceur::ParametreExtensionSP> liste_parametre_extension;
	std::pair<std::vector<SVM::Demarrage::Lanceur::ParametreExtensionOption>, std::vector<SVM::Demarrage::Lanceur::ParametreExtensionSP> > options_arguments_extension;
};

#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)



#line 148 "analyseur.syn.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _INVALIDE_ = 258,
    IDENTIFIANT = 259,
    CHAINE_MONOLIGNE = 260,
    VALEUR_OPTION = 261,
    LIGNE = 262,
    ENTIER = 263,
    DEBUGUEUR = 264,
    PERFORMANCE = 265,
    LOG = 266,
    PLUGIN_FILE = 267,
    LOCAL_PLUGIN = 268,
    OPTION = 269,
    FLAG = 270,
    MULTIPLE = 271,
    STR = 272,
    INT = 273,
    HELP = 274,
    ARGUMENT = 275,
    ARGUMENTS = 276,
    PROCESS = 277,
    CODE = 278,
    INCLUDE_FILE = 279,
    SERVER = 280,
    QUIET = 281,
    END = 282,
    TOKEN_MEMORY = 283,
    SCHEDULER = 284,
    SEQUENCER = 285,
    AUTOTERMINATED = 286,
    POINT = 287,
    DESCRIPTION = 288,
    INLINE = 289,
    NETWORK = 290,
    STYLE = 291,
    CLIENTS = 292,
    SECURITE = 293,
    SEPARATEUR = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  struct ValeurLanceur  YYSTYPE;
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



int demarragelanceuranalyseurparse (void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur);

#endif /* !YY_DEMARRAGELANCEURANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 224 "analyseur.syn.cpp" /* yacc.c:358  */

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
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   83

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  118

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

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
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   110,   110,   113,   114,   117,   124,   127,   134,   137,
     143,   151,   154,   165,   176,   187,   198,   212,   215,   221,
     227,   228,   234,   238,   242,   248,   253,   264,   280,   281,
     284,   288,   294,   298,   305,   308,   313,   321,   324,   331,
     335,   341,   342,   345,   352,   355,   360,   364,   370,   374,
     381,   384,   389,   390,   393,   399,   400,   403,   409,   410,
     413,   419,   426,   430,   437,   440,   446,   452,   457,   464,
     468,   474,   477,   483,   490,   493,   499,   506,   509,   514,
     521,   524
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_INVALIDE_", "IDENTIFIANT",
  "CHAINE_MONOLIGNE", "VALEUR_OPTION", "LIGNE", "ENTIER", "DEBUGUEUR",
  "PERFORMANCE", "LOG", "PLUGIN_FILE", "LOCAL_PLUGIN", "OPTION", "FLAG",
  "MULTIPLE", "STR", "INT", "HELP", "ARGUMENT", "ARGUMENTS", "PROCESS",
  "CODE", "INCLUDE_FILE", "SERVER", "QUIET", "END", "TOKEN_MEMORY",
  "SCHEDULER", "SEQUENCER", "AUTOTERMINATED", "POINT", "DESCRIPTION",
  "INLINE", "NETWORK", "STYLE", "CLIENTS", "SECURITE", "SEPARATEUR",
  "$accept", "lanceur", "description_optionel", "description", "lignes",
  "debugueur_optionel", "debugueur", "debugueur_liste_options",
  "performance_optionel", "performance", "historique_optionel",
  "historique", "historique_terminal", "historique_fichier",
  "historique_serveur", "liste_extensions", "extension",
  "parametres_extension", "liste_options_extension",
  "liste_arguments_extension", "argument_extension", "liste_options",
  "option", "multiple_optionel", "type_option", "type_argument",
  "aide_optionelle", "liste_arguments", "argument",
  "arguments_supplementaires_optionel", "arguments_supplementaires",
  "liste_processus", "processus", "code", "source_code",
  "memoire_optionelle", "memoire", "liste_selecteurs", "selecteur",
  "ordonanceur_optionel", "ordonanceur", "sequenceur_optionel",
  "sequenceur", "terminaison_automatique_optionnelle", "nom_extension",
  "calme_optionel", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

#define YYPACT_NINF -40

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-40)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -3,   -40,    24,    20,   -40,    22,   -40,    -9,    23,   -40,
     -40,   -40,   -40,   -40,    29,    30,   -40,   -40,    31,    27,
     -40,    12,    34,   -40,    20,   -40,   -40,   -40,    12,   -10,
     -40,     6,   -40,    35,    36,    37,    38,   -40,    39,    40,
     -40,    28,   -40,   -40,   -40,   -40,   -40,    42,    43,   -40,
     -14,   -40,    -6,   -40,    32,     3,    46,   -40,    21,   -40,
       0,   -40,   -40,   -40,    -4,   -40,   -40,    47,   -40,    48,
      21,   -40,   -40,   -40,   -40,     0,   -40,    50,   -40,    41,
      33,   -40,   -40,    41,    52,   -40,    53,    44,   -40,   -40,
     -22,    18,    26,   -40,    54,   -40,   -40,   -40,   -40,    18,
     -40,    57,    45,   -40,   -40,    22,   -40,    49,   -40,    57,
      51,   -40,    58,   -40,   -40,    56,   -40,   -40
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     6,     0,    20,     4,     5,     1,    80,     8,    21,
      22,    23,    24,     7,     0,     0,    81,    25,     0,    17,
       9,    80,     0,    11,     0,    28,    18,    26,    80,    10,
      19,    41,    27,     0,     0,     0,     0,    16,     0,     0,
      29,    52,    12,    13,    14,    15,    34,     0,     0,    42,
      55,    30,    33,    34,    44,     0,     0,    53,     0,    56,
      35,    37,    31,    45,     0,    48,    49,     0,    57,     0,
       2,    58,    40,    39,    36,    32,    46,     0,    47,    50,
       0,    59,    38,    50,     0,    54,     0,    64,    43,    51,
       0,     0,    71,    65,     0,     6,    61,    69,    70,    66,
      67,     0,    74,    72,    62,    63,    68,     0,    73,     0,
      77,    75,     0,    76,    78,     0,    79,    60
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -40,   -40,   -40,   -40,   -32,   -40,   -40,   -40,   -40,   -40,
     -40,   -20,   -40,   -40,   -40,   -40,   -40,    11,   -40,   -40,
      -8,   -40,   -40,   -40,   -40,     1,   -17,   -40,   -40,   -40,
     -40,   -40,    -2,   -40,   -40,   -40,   -40,   -40,   -30,   -40,
     -40,   -40,   -40,   -40,   -39,   -18
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     4,     5,    19,    20,    29,    25,    26,
       8,     9,    10,    11,    12,    31,    40,    51,    52,    75,
      74,    41,    49,    64,    77,    67,    85,    50,    57,    58,
      59,    70,    71,    87,    96,    92,    93,    99,   100,   102,
     103,   110,   111,   115,   108,    17
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      60,     7,    94,    27,    30,    72,    55,    56,    73,    37,
      32,    76,    95,    65,    66,    14,    15,    16,    38,    39,
      65,    66,    97,    98,     6,    33,    34,    35,    36,    13,
       1,     7,    18,    61,    21,    22,    23,    24,    16,    28,
      42,    43,    48,    69,    46,    44,    45,    53,    63,    54,
      68,    79,    47,    80,    83,   101,    86,    89,    90,   104,
      84,   107,   116,   105,    62,    78,    88,    82,    81,   106,
     113,     0,    91,     0,     0,   109,     0,     0,     0,     0,
       0,   112,   114,   117
};

static const yytype_int8 yycheck[] =
{
       6,    11,    24,    21,    24,     5,    20,    21,     8,    29,
      28,    15,    34,    17,    18,    24,    25,    26,    12,    13,
      17,    18,     4,     5,     0,    35,    36,    37,    38,     7,
      33,    11,     9,    39,     5,     5,     5,    10,    26,     5,
       5,     5,    14,    22,     5,     8,     8,     5,    16,     6,
       4,     4,    12,     5,     4,    29,    23,     5,     5,     5,
      19,     4,     4,    95,    53,    64,    83,    75,    70,    99,
     109,    -1,    28,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    32,    31,    27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    33,    41,    42,    43,    44,     0,    11,    50,    51,
      52,    53,    54,     7,    24,    25,    26,    85,     9,    45,
      46,     5,     5,     5,    10,    48,    49,    85,     5,    47,
      51,    55,    85,    35,    36,    37,    38,    51,    12,    13,
      56,    61,     5,     5,     8,     8,     5,    12,    14,    62,
      67,    57,    58,     5,     6,    20,    21,    68,    69,    70,
       6,    39,    57,    16,    63,    17,    18,    65,     4,    22,
      71,    72,     5,     8,    60,    59,    15,    64,    65,     4,
       5,    72,    60,     4,    19,    66,    23,    73,    66,     5,
       5,    28,    75,    76,    24,    34,    74,     4,     5,    77,
      78,    29,    79,    80,     5,    44,    78,     4,    84,    30,
      81,    82,    32,    84,    31,    83,     4,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    44,    44,    45,    45,
      46,    47,    47,    47,    47,    47,    47,    48,    48,    49,
      50,    50,    51,    51,    51,    52,    53,    54,    55,    55,
      56,    56,    57,    57,    58,    58,    58,    59,    59,    60,
      60,    61,    61,    62,    63,    63,    64,    64,    65,    65,
      66,    66,    67,    67,    68,    69,    69,    70,    71,    71,
      72,    73,    74,    74,    75,    75,    76,    77,    77,    78,
      78,    79,    79,    80,    81,    81,    82,    83,    83,    84,
      85,    85
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     9,     0,     1,     2,     0,     2,     0,     1,
       3,     0,     3,     3,     3,     3,     2,     0,     1,     2,
       0,     1,     1,     1,     1,     2,     4,     5,     0,     2,
       3,     4,     3,     1,     0,     2,     3,     0,     2,     1,
       1,     0,     2,     6,     0,     1,     1,     1,     1,     1,
       0,     2,     0,     2,     4,     0,     1,     2,     1,     2,
       8,     3,     2,     2,     0,     1,     2,     1,     2,     1,
       1,     0,     1,     2,     0,     1,     2,     0,     1,     3,
       0,     1
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
      yyerror (&yylloc, scanner, lanceur, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner, lanceur); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (lanceur);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, lanceur);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, lanceur);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, lanceur); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (lanceur);
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
yyparse (void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur)
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
#line 39 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1429  */
{ 
#if YYDEBUG==1
	demarragelanceuranalyseurdebug=1;
#endif
}

#line 1323 "analyseur.syn.cpp" /* yacc.c:1429  */
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
        case 5:
#line 118 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->description((yyvsp[0].chaine));
}
#line 1514 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 124 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = std::string();
}
#line 1522 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 128 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[-1].chaine) + (yyvsp[0].chaine);
}
#line 1530 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 134 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->debugueur(std::make_shared<SVM::Demarrage::Lanceur::Debugueur>(lanceur->_source));
}
#line 1538 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 138 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->debugueur((yyvsp[0].debugueur));
}
#line 1546 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 144 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = (yyvsp[0].debugueur);
	(yyval.debugueur)->_titre = (yyvsp[-1].chaine);
}
#line 1555 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 151 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = std::make_shared<SVM::Demarrage::Lanceur::Debugueur>(lanceur->_source);
}
#line 1563 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 155 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = (yyvsp[-2].debugueur);
	if((yyval.debugueur)->_aip)
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Duplicate network option");
		return 1;
	}
	(yyval.debugueur)->_aip = true;
	(yyval.debugueur)->_ip = (yyvsp[0].chaine);
}
#line 1578 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 166 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = (yyvsp[-2].debugueur);
	if((yyval.debugueur)->_astyle)
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Duplicate style option");
		return 1;
	}
	(yyval.debugueur)->_astyle = true;
	(yyval.debugueur)->_style = (yyvsp[0].chaine);
}
#line 1593 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 177 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = (yyvsp[-2].debugueur);
	if((yyval.debugueur)->_astyle)
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Duplicate style option");
		return 1;
	}
	(yyval.debugueur)->_astyle = true;
	(yyval.debugueur)->_clients = (yyvsp[0].entier);
}
#line 1608 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 188 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = (yyvsp[-2].debugueur);
	if((yyval.debugueur)->_asecurite)
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Duplicate security option");
		return 1;
	}
	(yyval.debugueur)->_asecurite = true;
	(yyval.debugueur)->_securite = (yyvsp[0].entier);
}
#line 1623 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 199 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.debugueur) = (yyvsp[-1].debugueur);
	if((yyval.debugueur)->_ahistorique)
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Duplicate log option");
		return 1;
	}
	(yyval.debugueur)->_ahistorique = true;
	(yyval.debugueur)->_historique = (yyvsp[0].historique);
}
#line 1638 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 212 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->performance(std::make_shared<SVM::Demarrage::Lanceur::Performance>());
}
#line 1646 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 216 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->performance((yyvsp[0].performance));
}
#line 1654 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 222 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.performance) = std::make_shared<SVM::Demarrage::Lanceur::Performance>((yyvsp[0].historique));
}
#line 1662 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 229 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->historique((yyvsp[0].historique));
}
#line 1670 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 235 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.historique) = (yyvsp[0].historique);
}
#line 1678 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 239 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.historique) = (yyvsp[0].historique);
}
#line 1686 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 243 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.historique) = (yyvsp[0].historique);
}
#line 1694 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 249 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.historique) = std::make_shared<SVM::Demarrage::Lanceur::HistoriqueTerminal>((yyvsp[0].booleen));
}
#line 1702 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 254 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	if((yyvsp[-1].chaine).empty())
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Invalid log file name");
		return 1;
	}
	(yyval.historique) = std::make_shared<SVM::Demarrage::Lanceur::HistoriqueFichier>((yyvsp[-1].chaine),(yyvsp[0].booleen));
}
#line 1715 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 265 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	if((yyvsp[-2].chaine).empty())
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Invalid server IP address");
		return 1;
	}
	if((yyvsp[-1].chaine).empty())
	{
		::demarragelanceuranalyseurerror(&(yyloc),scanner,lanceur,"Invalid server port");
		return 1;
	}
	(yyval.historique) = std::make_shared<SVM::Demarrage::Lanceur::HistoriqueServeur>((yyvsp[-2].chaine),(yyvsp[-1].chaine),(yyvsp[0].booleen));
}
#line 1733 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 285 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->extension((yyvsp[-1].chaine),SVM::Demarrage::Lanceur::Lanceur::TypeExtension::SYSTEME,(yyvsp[0].options_arguments_extension).first,(yyvsp[0].options_arguments_extension).second);
}
#line 1741 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 289 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->extension((yyvsp[-1].chaine),SVM::Demarrage::Lanceur::Lanceur::TypeExtension::LOCAL,(yyvsp[0].options_arguments_extension).first,(yyvsp[0].options_arguments_extension).second);
}
#line 1749 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 295 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.options_arguments_extension) = std::make_pair((yyvsp[-2].liste_option_extension),(yyvsp[0].liste_parametre_extension));
}
#line 1757 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 299 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.options_arguments_extension) = std::make_pair((yyvsp[0].liste_option_extension),std::vector<SVM::Demarrage::Lanceur::ParametreExtensionSP>());
}
#line 1765 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 305 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_option_extension) = {};
}
#line 1773 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 309 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_option_extension) = (yyvsp[-1].liste_option_extension);
	(yyval.liste_option_extension).emplace_back((yyvsp[0].chaine));
}
#line 1782 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 314 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_option_extension) = (yyvsp[-2].liste_option_extension);
	(yyval.liste_option_extension).emplace_back((yyvsp[-1].chaine),(yyvsp[0].parametre_extension));
}
#line 1791 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 321 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_parametre_extension) = {};
}
#line 1799 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 325 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_parametre_extension) = (yyvsp[-1].liste_parametre_extension);
	(yyval.liste_parametre_extension).push_back((yyvsp[0].parametre_extension));
}
#line 1808 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 332 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.parametre_extension) = std::make_shared<SVM::Demarrage::Lanceur::ParametreExtensionEntier>((yyvsp[0].entier));
}
#line 1816 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 336 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.parametre_extension) = std::make_shared<SVM::Demarrage::Lanceur::ParametreExtensionChaine>((yyvsp[0].chaine));
}
#line 1824 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 346 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->option(SVM::Demarrage::Lanceur::Option((yyvsp[-4].chaine),(yyvsp[-3].booleen),(yyvsp[-2].type_options),(yyvsp[-1].chaine),(yyvsp[0].chaine_sp)));
}
#line 1832 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 352 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = false;
}
#line 1840 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 356 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = true;
}
#line 1848 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 361 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_options) = SVM::Demarrage::Lanceur::TypeOptions::DRAPEAU;
}
#line 1856 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 365 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_options) = (yyvsp[0].type_options);
}
#line 1864 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 371 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_options) = SVM::Demarrage::Lanceur::TypeOptions::CHAINE;
}
#line 1872 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 375 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_options) = SVM::Demarrage::Lanceur::TypeOptions::ENTIER;
}
#line 1880 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 381 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine_sp) = SHARED_PTR(std::string)();
}
#line 1888 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 385 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine_sp) = std::make_shared<std::string>((yyvsp[0].chaine));
}
#line 1896 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 394 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->argument(SVM::Demarrage::Lanceur::Argument((yyvsp[-2].type_options),(yyvsp[-1].chaine),(yyvsp[0].chaine_sp)));
}
#line 1904 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 404 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->arguments_supplementaires((yyvsp[0].chaine));
}
#line 1912 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 414 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	lanceur->processus(SVM::Demarrage::Lanceur::Processus((yyvsp[-6].chaine),(yyvsp[-5].code),(yyvsp[-4].memoire),(yyvsp[-3].nom_extension),(yyvsp[-2].nom_extension),(yyvsp[-1].booleen)));
}
#line 1920 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 420 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.code) = (yyvsp[0].code);
	(yyval.code)->nom((yyvsp[-1].chaine));
}
#line 1929 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 427 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.code) = std::make_shared<SVM::Machine::Fichier>((yyvsp[0].chaine));
}
#line 1937 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 431 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.code) = std::make_shared<SVM::Machine::Texte>((yyvsp[0].chaine));
}
#line 1945 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 437 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.memoire) = {};
}
#line 1953 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 441 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.memoire) = (yyvsp[0].memoire);
}
#line 1961 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 447 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.memoire) = (yyvsp[0].memoire);
}
#line 1969 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 453 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.memoire) = std::vector<SVM::Demarrage::Lanceur::SelecteurSP>();
	(yyval.memoire).push_back((yyvsp[0].selecteur));
}
#line 1978 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 458 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.memoire) = (yyvsp[-1].memoire);
	(yyval.memoire).push_back((yyvsp[0].selecteur));
}
#line 1987 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 465 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.selecteur) = std::make_shared<SVM::Demarrage::Lanceur::SelecteurAlias>((yyvsp[0].chaine));
}
#line 1995 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 469 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.selecteur) = std::make_shared<SVM::Demarrage::Lanceur::SelecteurGroupe>((yyvsp[0].chaine));
}
#line 2003 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 474 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = SVM::Machine::Element::Valeur::PointEntreeExtensionSP();
}
#line 2011 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 478 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = (yyvsp[0].nom_extension);
}
#line 2019 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 484 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = (yyvsp[0].nom_extension);
}
#line 2027 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 490 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = SVM::Machine::Element::Valeur::PointEntreeExtensionSP();
}
#line 2035 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 494 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = (yyvsp[0].nom_extension);
}
#line 2043 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 500 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = (yyvsp[0].nom_extension);
}
#line 2051 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 506 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = false;
}
#line 2059 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 510 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = true;
}
#line 2067 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 515 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.nom_extension) = std::make_shared<SVM::Machine::Element::Valeur::PointEntreeExtension>((yyvsp[-2].chaine),(yyvsp[0].chaine));
}
#line 2075 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 521 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = false;
}
#line 2083 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 525 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = true;
}
#line 2091 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;


#line 2095 "analyseur.syn.cpp" /* yacc.c:1646  */
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
      yyerror (&yylloc, scanner, lanceur, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, lanceur, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, lanceur);
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
                  yystos[yystate], yyvsp, yylsp, scanner, lanceur);
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
  yyerror (&yylloc, scanner, lanceur, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, lanceur);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, lanceur);
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
#line 530 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1906  */

int demarragelanceuranalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur, std::string mesg)
{
	lanceur->erreur(SVM::Demarrage::Lanceur::Erreur(llocp->first_line,mesg));
	return 1;
}
