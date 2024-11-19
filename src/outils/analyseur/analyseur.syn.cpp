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
#define yyparse         outilssvmpluginanalyseurparse
#define yylex           outilssvmpluginanalyseurlex
#define yyerror         outilssvmpluginanalyseurerror
#define yydebug         outilssvmpluginanalyseurdebug
#define yynerrs         outilssvmpluginanalyseurnerrs


/* Copy the first part of user declarations.  */
#line 20 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:339  */

#include <src/global/global.h>
#define YYDEBUG 0
#include <src/outils/analyseur/includes.h>


extern int outilssvmpluginanalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur, std::string mesg);
extern int outilssvmpluginanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
extern int outilssvmpluginanalyseurlex_init(void *scanner);
extern int outilssvmpluginanalyseurlex_destroy(void *scanner);

namespace SVM_Programme = SVM::Machine::Element::Programme;
namespace SVM_Instruction = SVM::Machine::Element::Processeur::Instruction;

#line 87 "analyseur.syn.cpp" /* yacc.c:339  */

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
#ifndef YY_OUTILSSVMPLUGINANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
# define YY_OUTILSSVMPLUGINANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int outilssvmpluginanalyseurdebug;
#endif
/* "%code requires" blocks.  */
#line 49 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:355  */


struct ValeurGenerationExtension
{
	long int entier;
	std::string chaine;
	bool booleen;
	SVM::Outils::FonctionSP fonction;
	SHARED_PTR(std::string) chaine_facultative;
	SVM::Outils::Fonction::Option option;
};

#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)

#define NOUVELLE_DIRECTIVE(nom,clef,valeur,position,texte) \
	if(extension->nouvelle_directive(#clef,valeur,texte.first_line,texte.last_line)) \
	{ \
		return ::outilssvmpluginanalyseurerror(&position,scanner,extension,erreur,std::string("duplicate directive ")+#nom); \
	}


#line 146 "analyseur.syn.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _INVALIDE_ = 258,
    IDENTIFIANT = 259,
    CHAINE = 260,
    TEXTE = 261,
    PARAMETRES = 262,
    VALEUR_OPTION = 263,
    TYPE_ENTIER = 264,
    TYPE_CHAINE = 265,
    TYPE_BOOLEEN = 266,
    PLUGIN = 267,
    USE = 268,
    DEFINE = 269,
    TYPE = 270,
    STRUCT = 271,
    INSTRUCTION = 272,
    FUNCTION = 273,
    INTERRUPTION = 274,
    SEQUENCER = 275,
    SCHEDULER = 276,
    OPTION = 277,
    ARGUMENT = 278,
    ARGUMENTS = 279,
    WAITING = 280,
    SYSTEM = 281,
    OVERRIDE = 282,
    ATTACH = 283,
    AUTHOR = 284,
    CHANGELOG = 285,
    CHECKS = 286,
    CODE = 287,
    COMMENT = 288,
    COMPARE = 289,
    COMPILE = 290,
    CONSTANT = 291,
    COPY = 292,
    CREATE = 293,
    CURRENT = 294,
    DATE = 295,
    DEFAULT = 296,
    DELETE = 297,
    DESCRIPTION = 298,
    DETACH = 299,
    EXAMPLE = 300,
    EXTRA_FILE = 301,
    FINALISATION = 302,
    HELP = 303,
    INCLUDES = 304,
    INITIALISATION = 305,
    LANG = 306,
    LICENSE = 307,
    LINK = 308,
    MAINTAINER = 309,
    NEWS = 310,
    NOTIFICATION = 311,
    OBJECT = 312,
    PATCH = 313,
    PLUGIN_VERSION = 314,
    PRINT = 315,
    README = 316,
    SCHEDULE = 317,
    SEEALSO = 318,
    SHELL = 319,
    STARTUP = 320,
    STRING = 321,
    SYNOPSIS = 322,
    TEST = 323,
    TITLE = 324,
    USAGE = 325,
    POINT = 326,
    DEUX_POINTS = 327,
    FIN_LIGNE = 328
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  struct ValeurGenerationExtension  YYSTYPE;
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



int outilssvmpluginanalyseurparse (void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur);

#endif /* !YY_OUTILSSVMPLUGINANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 256 "analyseur.syn.cpp" /* yacc.c:358  */

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
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   441

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  93
/* YYNRULES -- Number of rules.  */
#define YYNRULES  166
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  407

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   328

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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   110,   110,   116,   122,   128,   130,   135,   138,   141,
     144,   147,   150,   153,   156,   159,   162,   165,   168,   171,
     174,   177,   180,   183,   186,   189,   192,   195,   198,   201,
     204,   207,   210,   213,   216,   219,   222,   227,   233,   239,
     245,   251,   257,   263,   269,   275,   281,   287,   293,   300,
     306,   312,   318,   324,   330,   336,   346,   349,   355,   369,
     392,   398,   404,   413,   419,   425,   431,   437,   443,   449,
     456,   458,   463,   466,   471,   479,   483,   487,   491,   495,
     499,   503,   507,   511,   515,   522,   530,   538,   546,   554,
     562,   570,   578,   586,   594,   602,   607,   610,   615,   618,
     621,   624,   627,   630,   633,   636,   639,   642,   645,   650,
     663,   673,   681,   689,   697,   711,   726,   734,   742,   750,
     757,   763,   770,   773,   779,   786,   789,   795,   802,   805,
     811,   818,   821,   827,   833,   839,   846,   849,   855,   860,
     866,   873,   876,   881,   886,   893,   902,   905,   911,   918,
     921,   940,   943,   947,   954,   957,   961,   967,   971,   978,
     980,   985,   988,   993,   997,  1003,  1007
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_INVALIDE_", "IDENTIFIANT", "CHAINE",
  "TEXTE", "PARAMETRES", "VALEUR_OPTION", "TYPE_ENTIER", "TYPE_CHAINE",
  "TYPE_BOOLEEN", "PLUGIN", "USE", "DEFINE", "TYPE", "STRUCT",
  "INSTRUCTION", "FUNCTION", "INTERRUPTION", "SEQUENCER", "SCHEDULER",
  "OPTION", "ARGUMENT", "ARGUMENTS", "WAITING", "SYSTEM", "OVERRIDE",
  "ATTACH", "AUTHOR", "CHANGELOG", "CHECKS", "CODE", "COMMENT", "COMPARE",
  "COMPILE", "CONSTANT", "COPY", "CREATE", "CURRENT", "DATE", "DEFAULT",
  "DELETE", "DESCRIPTION", "DETACH", "EXAMPLE", "EXTRA_FILE",
  "FINALISATION", "HELP", "INCLUDES", "INITIALISATION", "LANG", "LICENSE",
  "LINK", "MAINTAINER", "NEWS", "NOTIFICATION", "OBJECT", "PATCH",
  "PLUGIN_VERSION", "PRINT", "README", "SCHEDULE", "SEEALSO", "SHELL",
  "STARTUP", "STRING", "SYNOPSIS", "TEST", "TITLE", "USAGE", "POINT",
  "DEUX_POINTS", "FIN_LIGNE", "$accept", "extension", "nom_extension",
  "directives", "liste_directives", "directive", "langage", "version",
  "auteur", "mainteneur", "news", "readme", "changelog", "licence",
  "usage", "tests", "compilation", "lien", "titre", "date", "synopsis",
  "description", "voir_aussi", "example", "fichier", "type", "test",
  "initialisation_test", "finalisation_test", "correction",
  "initialisation", "startup", "finalisation", "inclusions", "code",
  "shell", "comment", "dependances", "liste_dependances", "dependance",
  "prototype", "prototype_type", "prototype_struct",
  "prototype_instruction", "prototype_fonction", "prototype_interruption",
  "prototype_sequenceur", "prototype_ordonanceur", "prototype_option",
  "prototype_argument", "prototype_arguments", "definitions",
  "liste_definitions", "definition", "definition_type",
  "definition_struct", "definition_instruction", "definition_fonction",
  "definition_interruption", "definition_sequenceur",
  "definition_ordonnanceur", "definition_option", "definition_argument",
  "definition_arguments", "definition_code", "fonction_create",
  "fonction_delete", "fonction_copy_facultative", "fonction_copy",
  "fonction_compare_facultative", "fonction_compare",
  "fonction_constant_facultative", "fonction_constant",
  "fonction_print_facultative", "fonction_print", "fonction_current",
  "fonction_schedule", "fonction_notification_facultative",
  "fonction_notification", "fonction_attach", "fonction_detach",
  "drapeaux_instruction", "point_entree_extension", "aide_facultative",
  "aide", "options_defaut", "options_objet_defaut",
  "options_chaine_defaut", "citation", "fin_ligne_facultative",
  "fin_ligne", "type_option", "type_argument", YY_NULLPTR
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328
};
# endif

#define YYPACT_NINF -332

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-332)))

#define YYTABLE_NINF -96

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      20,    35,    41,  -332,   -24,  -332,    40,   371,  -332,   -17,
     -24,    46,    -5,    12,    13,    15,    57,    17,    21,    22,
      27,    67,    28,    29,    30,    31,    32,    34,    36,    37,
      38,    39,    43,    44,    45,    47,    48,    53,    55,    56,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,   176,   -24,  -332,    85,    -3,   -24,   -24,    58,
     102,   113,   -24,   116,  -332,    59,   -24,   -24,   -24,   124,
      -3,   129,   130,    -3,   131,   132,    -3,    -3,   -24,   -24,
      -3,    18,   133,    -3,   135,   135,   138,   135,   135,   135,
     135,   135,   135,   214,  -332,   101,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,    71,   152,   -24,  -332,
     -24,   144,   -17,   146,   147,   -24,   -24,   -24,   158,   -24,
     160,   171,   172,   173,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   174,   175,   -24,   -24,   -24,   -24,   -24,
     -24,   111,   -24,   -24,   -24,   -24,   -24,   -24,   193,    49,
     -24,  -332,   134,  -332,  -332,   198,  -332,  -332,  -332,   136,
     203,   208,   209,   210,   101,   211,   212,   101,   101,   101,
     328,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,   -17,   -17,  -332,   -24,   -24,   215,   -17,
     -17,   -24,   213,   -24,   -24,   -24,   -24,   -17,   -17,   -17,
     -17,   -17,   218,   -17,   -17,   -17,   -24,   -24,   -17,   220,
     225,   233,   -17,   -17,   217,   -17,   -17,   -17,   -17,   -17,
     -17,    26,  -332,  -332,   -24,   -17,    -3,   -24,   -24,   -24,
     -24,   -24,   -24,  -332,   -24,   -24,  -332,  -332,  -332,  -332,
     -17,   -17,   236,   -17,   -24,   238,   -17,   -17,   -17,   -24,
     -17,   -17,   -24,   -24,   -24,  -332,  -332,   -24,  -332,   -17,
     -24,   -17,   246,   -33,   -33,   -26,   -26,   -10,   -10,   237,
     -17,   -24,   -17,    -1,   -17,   -17,   -17,   -17,   -24,   -11,
     216,   216,  -332,  -332,   221,   219,   219,   -24,   -17,   -24,
     -17,  -332,  -332,   182,   -11,   222,  -332,   101,  -332,   183,
     224,   195,   -17,   -17,   -24,   186,   -11,   223,  -332,  -332,
     -24,   -11,   239,   -11,   226,   258,   -24,   194,   -14,   228,
    -332,   265,   200,   -11,   230,   205,   -11,   239,  -332,   -24,
     269,   -24,  -332,  -332,   206,   -11,   101,  -332,   -24,   -24,
     207,   -11,   228,   -24,   227,   230,   -17,   -24,   275,   -24,
     229,  -332,   -17,   277,   -24,   232,   101,   278,   -24,   228,
     -17,   -24,   279,   -24,   -24,   283,   -24,  -332,   -24,   284,
     101,   -17,   -24,   285,   -17,   -24,   287,   -17,   -24,  -332,
     -17,   -24,   -17,   -24,   -17,   -17,   -17
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     5,     0,     1,    70,     4,   161,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      22,    23,    24,    16,    17,    18,    19,    21,    20,    30,
      31,    32,    33,    34,    26,    27,    28,    25,    29,    35,
      36,   162,   141,     0,     2,     0,   159,   159,   159,     0,
       0,     0,   159,     0,    57,     0,   159,   159,   159,     0,
     159,     0,     0,   159,     0,     0,   159,   159,   159,   159,
     159,     0,     0,   159,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   141,    72,   146,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,     0,   141,     0,   157,
       0,     0,   160,     0,     0,   159,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,   159,   159,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,   147,     0,   143,   142,   144,     0,
       0,     0,     0,     0,   146,     0,     0,   146,   146,   146,
     141,    96,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    39,    43,   158,     0,     0,     0,    47,
      50,     0,     0,   159,     0,     0,     0,    37,    44,    48,
      40,    41,     0,    38,    42,    53,     0,     0,    51,     0,
       0,     0,    49,    45,     0,    85,    86,    88,    89,    90,
      91,     0,   165,   166,     0,    94,   159,     0,   159,     0,
       0,     0,     0,   113,     0,     0,   116,   117,   118,    97,
      46,    67,     0,    52,     0,     0,    65,    66,    63,     0,
      68,    64,     0,     0,     0,   145,   163,     0,   164,    93,
       0,    87,     0,     0,     0,   146,   146,     0,     0,     0,
      54,     0,    62,    58,    61,    60,    92,   148,     0,   151,
     122,   122,   111,   112,   149,     0,     0,     0,    55,     0,
     119,   152,   153,     0,   151,   125,   123,   146,   150,     0,
       0,     0,    69,    59,   159,     0,   151,   128,   126,   110,
     159,   151,     0,   151,   136,     0,   159,     0,   154,   131,
     129,     0,     0,   151,     0,     0,   151,     0,   137,     0,
       0,   159,   155,   156,     0,   151,   146,   132,     0,   159,
       0,   151,     0,   159,     0,     0,   121,     0,     0,   159,
       0,   109,   120,     0,   159,     0,   146,     0,   159,     0,
     124,     0,     0,   159,     0,     0,   159,   114,     0,     0,
     146,   127,     0,     0,   134,     0,     0,   135,     0,   115,
     130,     0,   139,     0,   138,   133,   140
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,   184,
    -332,  -124,  -117,  -115,  -114,  -112,  -111,  -110,  -109,  -108,
    -107,  -332,  -332,   105,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,     8,  -255,     1,  -332,  -332,
    -332,  -332,  -332,  -332,  -331,  -332,  -332,  -332,  -332,   -49,
     -65,  -332,    78,  -163,  -332,  -332,  -269,  -332,   -89,   -44,
      -4,  -332,    65
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     6,     7,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    85,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    11,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    74,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   305,   300,   315,   316,   327,
     328,   339,   340,   356,   357,   332,   334,   347,   348,   344,
     362,   126,   162,   173,   174,   319,   313,   354,   130,   131,
     132,   277,   244
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       9,   145,   129,   180,   148,   309,    72,   151,   152,   299,
     181,   155,   182,   183,   160,   184,   185,   186,   187,   188,
     189,   253,   172,   156,   256,   257,   258,   352,   304,   301,
     311,   376,     1,   133,   134,   242,   243,   276,   138,     4,
      71,     5,   141,   142,   143,   325,   312,    71,   390,     8,
     320,   321,   353,    10,   153,   154,    71,   337,   242,   243,
      73,    79,   342,    71,   345,   157,   180,    75,   158,   127,
       8,    84,    71,   181,   360,   182,   183,   364,   184,   185,
     186,   187,   188,   189,    76,    77,   370,    78,   175,    80,
     128,   208,   375,    81,    82,   212,   176,   177,   178,    83,
      86,    87,    88,    89,    90,   222,    91,   136,    92,    93,
      94,    95,   229,   230,   231,    96,    97,    98,   137,    99,
     100,   139,   302,   303,   203,   101,   204,   102,   103,   144,
     135,   140,   209,   210,   146,   147,   149,   150,   159,   161,
     217,   218,   219,   220,   221,   164,   223,   224,   225,   172,
     205,   228,   206,   207,   329,   232,   233,   280,   235,   236,
     237,   238,   239,   240,   211,   213,   245,   104,   105,   265,
     106,   107,   108,   109,   110,   111,   112,   214,   215,   216,
     226,   227,   234,   163,   179,   165,   166,   167,   168,   169,
     170,   104,   105,   371,   106,   107,   108,   109,   110,   111,
     112,   241,   260,   261,   282,   247,   246,   263,   248,   249,
     266,   267,   268,   387,   250,   251,   252,   254,   255,   264,
     262,   275,   270,   271,   269,    71,   272,   399,   -71,   104,
     105,   273,   106,   107,   108,   109,   110,   111,   112,   274,
     279,   289,   307,   281,   291,   283,   284,   285,   286,    71,
     287,   288,   298,   314,   324,   330,   326,   333,   336,   338,
     290,   299,   318,   331,   349,   292,   351,   343,   293,   294,
     295,   358,   359,   296,   361,   367,   297,   363,   369,   374,
     335,   381,   346,   384,   388,   392,   341,   308,   355,   395,
     398,   401,   350,   403,   310,   259,   306,   171,   365,   378,
     379,   383,   317,   322,   386,   323,   278,   368,     0,     0,
       0,     0,     0,     0,     0,   373,     0,     0,     0,   377,
       0,     0,     0,     0,     0,   382,     0,     0,   -95,     0,
     385,     0,     0,     0,   389,     0,     0,     0,     0,   393,
       0,     0,   396,   104,   105,   366,   106,   107,   108,   109,
     110,   111,   112,     0,   372,     0,     0,     0,     0,     0,
     179,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   391,     0,     0,
     394,     0,     0,     0,   397,     0,     0,     0,   400,     0,
       0,   402,     0,     0,   404,     0,     0,   405,     0,   406,
      12,    13,    14,    15,    16,     0,    17,     0,     0,     0,
       0,    18,     0,     0,    19,     0,    20,    21,    22,     0,
      23,    24,    25,    26,    27,    28,    29,     0,     0,    30,
      31,     0,    32,     0,    33,    34,    35,     0,    36,    37,
      38,    39
};

static const yytype_int16 yycheck[] =
{
       4,    90,     5,   127,    93,     6,    10,    96,    97,    42,
     127,   100,   127,   127,   103,   127,   127,   127,   127,   127,
     127,   184,    48,     5,   187,   188,   189,    41,    38,   284,
      41,   362,    12,    77,    78,     9,    10,    11,    82,     4,
      73,     0,    86,    87,    88,   314,    57,    73,   379,    73,
     305,   306,    66,    13,    98,    99,    73,   326,     9,    10,
      14,     4,   331,    73,   333,    47,   190,    72,    50,    73,
      73,     4,    73,   190,   343,   190,   190,   346,   190,   190,
     190,   190,   190,   190,    72,    72,   355,    72,    17,    72,
       5,   135,   361,    72,    72,   139,    25,    26,    27,    72,
      72,    72,    72,    72,    72,   149,    72,     5,    72,    72,
      72,    72,   156,   157,   158,    72,    72,    72,     5,    72,
      72,     5,   285,   286,   128,    72,   130,    72,    72,     5,
      72,    72,   136,   137,     5,     5,     5,     5,     5,     4,
     144,   145,   146,   147,   148,     7,   150,   151,   152,    48,
       6,   155,     6,     6,   317,   159,   160,   246,   162,   163,
     164,   165,   166,   167,     6,     5,   170,    15,    16,   213,
      18,    19,    20,    21,    22,    23,    24,     6,     6,     6,
       6,     6,    71,   105,    32,   107,   108,   109,   110,   111,
     112,    15,    16,   356,    18,    19,    20,    21,    22,    23,
      24,     8,   206,   207,   248,     7,    72,   211,    72,     6,
     214,   215,   216,   376,     6,     6,     6,     6,     6,     6,
       5,     4,   226,   227,     6,    73,     6,   390,    14,    15,
      16,     6,    18,    19,    20,    21,    22,    23,    24,     6,
     244,     5,     5,   247,     6,   249,   250,   251,   252,    73,
     254,   255,     6,    37,    72,    72,    34,    62,    72,    36,
     264,    42,    41,    39,     6,   269,    72,    28,   272,   273,
     274,     6,    72,   277,    44,     6,   280,    72,    72,    72,
     324,     6,    56,     6,     6,     6,   330,   291,    60,     6,
       6,     6,   336,     6,   298,   190,   288,   113,   347,    72,
     365,    72,   301,   307,    72,   309,   241,   351,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   359,    -1,    -1,    -1,   363,
      -1,    -1,    -1,    -1,    -1,   369,    -1,    -1,     0,    -1,
     374,    -1,    -1,    -1,   378,    -1,    -1,    -1,    -1,   383,
      -1,    -1,   386,    15,    16,   349,    18,    19,    20,    21,
      22,    23,    24,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,   367,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   381,    -1,    -1,
     384,    -1,    -1,    -1,   388,    -1,    -1,    -1,   392,    -1,
      -1,   395,    -1,    -1,   398,    -1,    -1,   401,    -1,   403,
      29,    30,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    43,    -1,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    -1,    63,    64,    65,    -1,    67,    68,
      69,    70
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    75,    76,     4,     0,    77,    78,    73,   164,
      13,   111,    29,    30,    31,    32,    33,    35,    40,    43,
      45,    46,    47,    49,    50,    51,    52,    53,    54,    55,
      58,    59,    61,    63,    64,    65,    67,    68,    69,    70,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    73,   164,    14,   125,    72,    72,    72,    72,     4,
      72,    72,    72,    72,     4,    99,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    15,    16,    18,    19,    20,    21,
      22,    23,    24,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   155,   164,     5,     5,
     162,   163,   164,   163,   163,    72,     5,     5,   163,     5,
      72,   163,   163,   163,     5,   162,     5,     5,   162,     5,
       5,   162,   162,   163,   163,   162,     5,    47,    50,     5,
     162,     4,   156,   156,     7,   156,   156,   156,   156,   156,
     156,   113,    48,   157,   158,    17,    25,    26,    27,    32,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   164,   164,     6,     6,     6,   163,   164,
     164,     6,   163,     5,     6,     6,     6,   164,   164,   164,
     164,   164,   163,   164,   164,   164,     6,     6,   164,   163,
     163,   163,   164,   164,    71,   164,   164,   164,   164,   164,
     164,     8,     9,    10,   166,   164,    72,     7,    72,     6,
       6,     6,     6,   157,     6,     6,   157,   157,   157,   127,
     164,   164,     5,   164,     6,   163,   164,   164,   164,     6,
     164,   164,     6,     6,     6,     4,    11,   165,   166,   164,
     162,   164,   163,   164,   164,   164,   164,   164,   164,     5,
     164,     6,   164,   164,   164,   164,   164,   164,     6,    42,
     140,   140,   157,   157,    38,   139,   139,     5,   164,     6,
     164,    41,    57,   160,    37,   141,   142,   141,    41,   159,
     140,   140,   164,   164,    72,   160,    34,   143,   144,   157,
      72,    39,   149,    62,   150,   163,    72,   160,    36,   145,
     146,   163,   160,    28,   153,   160,    56,   151,   152,     6,
     163,    72,    41,    66,   161,    60,   147,   148,     6,    72,
     160,    44,   154,    72,   160,   153,   164,     6,   163,    72,
     160,   157,   164,   163,    72,   160,   148,   163,    72,   154,
     164,     6,   163,    72,     6,   163,    72,   157,     6,   163,
     148,   164,     6,   163,   164,     6,   163,   164,     6,   157,
     164,     6,   164,     6,   164,   164,   164
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    76,    77,    78,    78,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    99,   100,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   111,   112,   112,   113,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   126,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   141,   142,   143,   143,   144,   145,   145,
     146,   147,   147,   148,   149,   150,   151,   151,   152,   153,
     154,   155,   155,   155,   155,   156,   157,   157,   158,   159,
     159,   160,   160,   160,   161,   161,   161,   162,   162,   163,
     163,   164,   164,   165,   165,   166,   166
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     3,     1,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     5,     4,     4,     4,
       4,     4,     5,     4,     6,     7,     0,     1,     6,     8,
       6,     6,     6,     5,     5,     5,     5,     5,     5,     8,
       0,     3,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     3,     3,
       3,     3,     5,     4,     3,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     9,
       6,     4,     4,     2,    10,    11,     2,     2,     2,     5,
       6,     6,     0,     1,     6,     0,     1,     6,     0,     1,
       6,     0,     1,     6,     6,     6,     0,     1,     6,     6,
       6,     0,     2,     2,     2,     3,     0,     1,     4,     0,
       1,     0,     1,     1,     0,     1,     1,     1,     2,     0,
       1,     1,     2,     1,     1,     1,     1
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
      yyerror (&yylloc, scanner, extension, erreur, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner, extension, erreur); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (extension);
  YYUSE (erreur);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, extension, erreur);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, extension, erreur);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, extension, erreur); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (extension);
  YYUSE (erreur);
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
yyparse (void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur)
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
#line 43 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1429  */
{ 
#if YYDEBUG==1
	outilssvmpluginanalyseurdebug=1;
#endif
}

#line 1574 "analyseur.syn.cpp" /* yacc.c:1429  */
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
#line 111 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1764 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 117 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	extension->nom((yyvsp[-1].chaine));
}
#line 1772 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 123 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1779 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 128 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1786 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 131 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1793 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 136 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1800 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 139 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1807 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 142 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1814 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 145 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1821 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 148 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1828 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 151 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1835 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 154 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1842 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 157 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1849 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 160 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1856 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 163 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1863 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 166 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1870 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 169 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1877 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 172 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1884 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 175 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1891 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 178 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1898 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 181 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1905 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 184 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1912 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 187 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1919 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 190 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1926 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 193 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1933 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 196 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1940 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 199 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1947 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 202 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1954 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 205 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1961 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 208 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1968 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 211 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1975 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 214 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1982 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 217 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1989 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 220 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1996 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 223 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2003 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 228 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(lang,langage,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2011 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 234 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(version,version,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2019 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 240 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(author,auteur,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2027 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 246 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(maintainer,mainteneur,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2035 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 252 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(news,nouveautes,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2043 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 258 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(readme,lisez_moi,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2051 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 264 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(changelog,historique,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2059 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 270 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(license,licence_longue,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2067 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 276 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(usage,licence_courte,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2075 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 282 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(checks,tests,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2083 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 288 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(compile,options_compilation,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2091 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 294 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(link,options_lien,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2099 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 301 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(title,titre,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2107 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 307 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(date,date,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2115 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 313 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(synopsis,synopsis,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2123 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 319 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(description,description,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2131 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 325 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(seealso,voir_aussi,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2139 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 331 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	extension->nouvel_exemple((yyvsp[-3].chaine),(yyvsp[-1].chaine));
}
#line 2147 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 337 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	if(not extension->nouveau_fichier((yyvsp[-3].chaine),(yyvsp[-1].chaine),(yyvsp[-5].chaine)))
	{
		return ::outilssvmpluginanalyseurerror(&(yylsp[-3]),scanner,extension,erreur,std::string("duplicate file ")+(yyvsp[-3].chaine));
	}
}
#line 2158 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 346 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "";
}
#line 2166 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 350 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2174 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 356 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	for(const auto &c: (yyvsp[-3].chaine))
	{
		if(not (((c>='a') and (c<='z')) or ((c>='A') and (c<='Z')) or ((c>='0' and c<='9') or (c=='_'))))
		{
			return ::outilssvmpluginanalyseurerror(&(yylsp[-3]),scanner,extension,erreur,std::string("invalid test name ")+(yyvsp[-3].chaine));
		}
	}
	if(not extension->nouveau_test((yyvsp[-3].chaine),"",(yyvsp[-1].chaine)))
	{
		return ::outilssvmpluginanalyseurerror(&(yylsp[-3]),scanner,extension,erreur,std::string("duplicate test ")+(yyvsp[-3].chaine));
	}
}
#line 2192 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 370 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	for(const auto &c: (yyvsp[-5].chaine))
	{
		if(not (((c>='a') and (c<='z')) or ((c>='A') and (c<='Z')) or ((c>='0' and c<='9') or (c=='_'))))
		{
			return ::outilssvmpluginanalyseurerror(&(yylsp[-5]),scanner,extension,erreur,std::string("invalid test name ")+(yyvsp[-5].chaine));
		}
	}
	for(const auto &c: (yyvsp[-3].chaine))
	{
		if(c=='\n')
		{
			return ::outilssvmpluginanalyseurerror(&(yylsp[-5]),scanner,extension,erreur,std::string("invalid command line for test ")+(yyvsp[-5].chaine));
		}
	}
	if(not extension->nouveau_test((yyvsp[-5].chaine),(yyvsp[-3].chaine),(yyvsp[-1].chaine)))
	{
		return ::outilssvmpluginanalyseurerror(&(yylsp[-5]),scanner,extension,erreur,std::string("duplicate test ")+(yyvsp[-5].chaine));
	}
}
#line 2217 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 393 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(test: initialisation,initialisation_test,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2225 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 399 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(test: finalisation,finalisation_test,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2233 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 405 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	if(not extension->nouvelle_correction((yyvsp[-3].chaine),(yyvsp[-1].chaine)))
	{
		return ::outilssvmpluginanalyseurerror(&(yylsp[-3]),scanner,extension,erreur,std::string("duplicate patch ")+(yyvsp[-3].chaine));
	}
}
#line 2244 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 414 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(initialisation,initialisation,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2252 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 420 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(startup,startup,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2260 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 426 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(finalisation,finalisation,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2268 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 432 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(includes,inclusions,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2276 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 438 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(code,code,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2284 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 444 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	NOUVELLE_DIRECTIVE(shell,shell,(yyvsp[-1].chaine),(yyloc),(yylsp[-1]));
}
#line 2292 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 450 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	extension->nouveau_commentaire((yyvsp[-6].chaine),(yyvsp[-3].chaine),(yyvsp[-2].chaine),(yyvsp[-1].chaine));
}
#line 2300 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 456 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2307 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 459 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2314 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 464 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2321 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 467 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2328 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 472 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-1].chaine),"",(yyloc).first_line,(yyloc).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_dependance(d);
}
#line 2338 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 480 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2346 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 484 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2354 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 488 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2362 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 492 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2370 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 496 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2378 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 500 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2386 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 504 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2394 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 508 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2402 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 512 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2410 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 516 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 2418 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 523 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "TYPE " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2428 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 531 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "STRUCT " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2438 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 539 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << (yyvsp[-3].chaine) << "INSTRUCTION" << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str();
}
#line 2448 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 547 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "FUNCTION" << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str();
}
#line 2458 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 555 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "INTERRUPTION " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2468 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 563 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "SEQUENCER " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2478 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 571 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "SCHEDULER " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2488 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 579 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "OPTION " << (yyvsp[-3].chaine) << " -" << (yyvsp[-2].chaine) << " " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2498 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 587 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "ARGUMENT " << (yyvsp[-2].chaine) << " " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2508 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 595 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << "ARGUMENTS " << (yyvsp[-1].chaine);
	(yyval.chaine) = oss.str(); 
}
#line 2518 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 603 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2525 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 608 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2532 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 611 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2539 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 616 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2546 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 619 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2553 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 622 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2560 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 625 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2567 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 628 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2574 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 631 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2581 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 634 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2588 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 637 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2595 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 640 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2602 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 643 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2609 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 646 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 2616 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 651 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-8].chaine),(yyvsp[-7].chaine),(yylsp[-7]).first_line,(yylsp[-7]).last_line);
	d->nouvelle_fonction("delete",(yyvsp[-5].fonction));
	d->nouvelle_fonction("copy",(yyvsp[-4].fonction));
	d->nouvelle_fonction("compare",(yyvsp[-3].fonction));
	d->nouvelle_fonction("constant",(yyvsp[-2].fonction));
	d->nouvelle_fonction("print",(yyvsp[-1].fonction));
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2631 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 664 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-5].chaine),(yyvsp[-4].chaine),(yylsp[-4]).first_line,(yylsp[-4]).last_line);
	d->nouvelle_fonction("delete",(yyvsp[-2].fonction));
	d->nouvelle_fonction("copy",(yyvsp[-1].fonction));
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2643 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 674 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-3].chaine),(yyvsp[-2].chaine),(yylsp[-2]).first_line,(yylsp[-2]).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2653 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 682 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-3].chaine),(yyvsp[-2].chaine),(yylsp[-2]).first_line,(yylsp[-2]).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2663 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 690 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-1].chaine),"",(yyloc).first_line,(yyloc).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2673 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 698 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-9].chaine),(yyvsp[-8].chaine),(yylsp[-8]).first_line,(yylsp[-8]).last_line);
	d->nouvelle_fonction("create",(yyvsp[-6].fonction));
	d->nouvelle_fonction("delete",(yyvsp[-5].fonction));
	d->nouvelle_fonction("current",(yyvsp[-4].fonction));
	d->nouvelle_fonction("attach",(yyvsp[-3].fonction));
	d->nouvelle_fonction("detach",(yyvsp[-2].fonction));
	d->nouvelle_fonction("print",(yyvsp[-1].fonction));
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2689 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 712 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-10].chaine),(yyvsp[-9].chaine),(yylsp[-9]).first_line,(yylsp[-9]).last_line);
	d->nouvelle_fonction("create",(yyvsp[-7].fonction));
	d->nouvelle_fonction("delete",(yyvsp[-6].fonction));
	d->nouvelle_fonction("schedule",(yyvsp[-5].fonction));
	d->nouvelle_fonction("notification",(yyvsp[-4].fonction));
	d->nouvelle_fonction("attach",(yyvsp[-3].fonction));
	d->nouvelle_fonction("detach",(yyvsp[-2].fonction));
	d->nouvelle_fonction("print",(yyvsp[-1].fonction));
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2706 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 727 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-1].chaine),"",(yyloc).first_line,(yyloc).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2716 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 735 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-1].chaine),"",(yyloc).first_line,(yyloc).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2726 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 743 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionObjet>((yyvsp[-1].chaine),"",(yyloc).first_line,(yyloc).last_line);
	d->nouvelle_aide((yyvsp[0].chaine_facultative));
	extension->nouvelle_definition(d); 
}
#line 2736 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 751 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto d = std::make_shared<SVM::Outils::DefinitionCode>((yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
	extension->nouvelle_definition(d); 
}
#line 2745 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 758 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2753 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 764 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2761 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 770 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = SHARED_PTR(SVM::Outils::Fonction)();
}
#line 2769 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 774 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = (yyvsp[0].fonction);
}
#line 2777 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 780 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2785 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 786 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = SHARED_PTR(SVM::Outils::Fonction)();
}
#line 2793 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 790 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = (yyvsp[0].fonction);
}
#line 2801 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 796 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2809 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 802 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = SHARED_PTR(SVM::Outils::Fonction)();
}
#line 2817 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 806 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = (yyvsp[0].fonction);
}
#line 2825 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 812 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2833 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 818 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = SHARED_PTR(SVM::Outils::Fonction)();
}
#line 2841 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 822 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = (yyvsp[0].fonction);
}
#line 2849 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 828 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2857 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 834 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2865 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 840 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2873 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 846 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = SHARED_PTR(SVM::Outils::Fonction)();
}
#line 2881 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 850 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = (yyvsp[0].fonction);
}
#line 2889 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 856 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2897 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 861 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2905 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 867 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.fonction) = std::make_shared<SVM::Outils::Fonction>((yyvsp[-4].option),(yyvsp[-1].chaine),(yylsp[-1]).first_line,(yylsp[-1]).last_line);
}
#line 2913 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 873 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "";
}
#line 2921 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 877 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[-1].chaine);
	(yyval.chaine) += "SYSTEM ";
}
#line 2930 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 882 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[-1].chaine);
	(yyval.chaine) += "WAITING ";
}
#line 2939 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 887 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[-1].chaine);
	(yyval.chaine) += "OVERRIDE ";
}
#line 2948 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 894 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	std::ostringstream oss;
	oss << (yyvsp[-2].chaine) << "." << (yyvsp[0].chaine);
	(yyval.chaine) = oss.str();
}
#line 2958 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 902 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine_facultative) = SHARED_PTR(std::string)();
}
#line 2966 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 906 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine_facultative) = std::make_shared<std::string>((yyvsp[0].chaine));
}
#line 2974 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 912 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[-1].chaine);
}
#line 2982 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 918 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::AUCUNE;
}
#line 2990 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 922 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::DEFAUT;
}
#line 2998 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 940 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::AUCUNE;
}
#line 3006 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 944 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::DEFAUT;
}
#line 3014 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 948 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::OBJET;
}
#line 3022 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 954 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::AUCUNE;
}
#line 3030 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 958 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::DEFAUT;
}
#line 3038 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 962 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.option) = SVM::Outils::Fonction::Option::CHAINE;
}
#line 3046 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 968 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 3054 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 972 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 3062 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 978 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 3069 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 981 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 3076 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 986 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 3083 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 989 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 3090 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 994 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "BLN";
}
#line 3098 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 998 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 3106 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1004 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "INT";
}
#line 3114 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 1008 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "STR";
}
#line 3122 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;


#line 3126 "analyseur.syn.cpp" /* yacc.c:1646  */
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
      yyerror (&yylloc, scanner, extension, erreur, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, extension, erreur, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, extension, erreur);
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
                  yystos[yystate], yyvsp, yylsp, scanner, extension, erreur);
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
  yyerror (&yylloc, scanner, extension, erreur, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, extension, erreur);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, extension, erreur);
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
#line 1013 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1906  */


int outilssvmpluginanalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur, std::string mesg)
{
	std::ostringstream oss;
	oss << "Unable to parse input: " << mesg; 
	erreur=std::make_shared<SVM::Outils::Erreur>(oss.str());
	erreur->position(llocp->first_line,llocp->last_line);
	return 1;
}

