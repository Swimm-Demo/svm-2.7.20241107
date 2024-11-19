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
#define yyparse         machineelementsprogrammeanalyseurparse
#define yylex           machineelementsprogrammeanalyseurlex
#define yyerror         machineelementsprogrammeanalyseurerror
#define yydebug         machineelementsprogrammeanalyseurdebug
#define yynerrs         machineelementsprogrammeanalyseurnerrs


/* Copy the first part of user declarations.  */
#line 20 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:339  */

#include <src/global/global.h>
#define YYDEBUG 0
#include <src/machine/elements/programme/analyseur/includes.h>


extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position, std::string mesg);
extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
extern int machineelementsprogrammeanalyseurlex_init(void *scanner);
extern int machineelementsprogrammeanalyseurlex_destroy(void *scanner);

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
#ifndef YY_MACHINEELEMENTSPROGRAMMEANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
# define YY_MACHINEELEMENTSPROGRAMMEANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int machineelementsprogrammeanalyseurdebug;
#endif
/* "%code requires" blocks.  */
#line 51 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:355  */


struct ValeurProgramme
{
	long int entier;
	std::string chaine;
	bool booleen;
	SVM_Valeur::InstructionSP instruction;
	SVM_Programme::ValeurSP valeur;
	SVM_Programme::LectureMemoireSP lecture_memoire;
	SVM_Programme::AdresseSP adresse;
	std::vector<SVM_Programme::ValeurSP> bloc;
	SVM_Programme::CibleSP cible;
	SVM_Programme::ConditionSP condition;
	SVM_Valeur::ExpressionConditionSP expression_condition;
	SVM_Valeur::PointEntreeExtension point_entree_extension;
	SVM_Programme::ChaineSP chaine_immediate;
	SVM_Programme::TypeSP type;
	SVM_Programme::DescriptionBlocAliasSP types_avec_alias;
	std::vector<SVM_Programme::DescriptionBlocAliasSP> liste_types_avec_alias;
	SVM_Programme::DescriptionBlocSP types_anonymes;
	std::vector<SVM_Programme::DescriptionBlocSP> liste_types_anonymes;
	SVM_Programme::ParametreSP parametre;
	std::vector<SVM_Programme::ParametreSP> liste_parametres;
	SVM_Valeur::ParametreMarqueur::Marqueur marqueur;
	SVM_Processeur::Instruction::DebogageArretMemoire::Type type_arret_memoire;
};

#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)

#line 155 "analyseur.syn.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _INVALIDE_ = 258,
    ENTIER = 259,
    IDENTIFIANT = 260,
    MOT_CLEF = 261,
    CHAINE_MONOLIGNE = 262,
    CHAINE_MULTILIGNE = 263,
    VRAI = 264,
    FAUX = 265,
    CONST = 266,
    POINTEUR_COURANT = 267,
    TAILLE = 268,
    CASCADE_MC = 269,
    GLOBAL_MC = 270,
    TYPE_ENTIER = 271,
    TYPE_CHAINE = 272,
    TYPE_BOOLEEN = 273,
    TYPE_POINTEUR = 274,
    TYPE_BIBLIOTHEQUE = 275,
    TYPE_SYMBOLE = 276,
    TYPE_POINT_ENTREE_EXTENSION = 277,
    TYPE_INTERRUPTION = 278,
    TYPE_AUTOMATIQUE = 279,
    INTERRUPTION_ECHEC = 280,
    INTERRUPTION_PROCESSEUR = 281,
    INTERRUPTION_MEMOIRE = 282,
    INTERRUPTION_TERMINAISON = 283,
    INTERRUPTION_NUMERIQUE = 284,
    INTERRUPTION_PREMIER = 285,
    INTERRUPTION_SECOND = 286,
    INTERRUPTION_SECURITE = 287,
    INTERRUPTION_PERIPHERIQUE = 288,
    INTERRUPTION_CLONE = 289,
    INTERRUPTION_TERMINAL = 290,
    INTERRUPTION_GENERIC = 291,
    DEBUT_PARENTHESE = 292,
    FIN_PARENTHESE = 293,
    DEBUT_CROCHET = 294,
    FIN_CROCHET = 295,
    DEBUT_ACCOLADE = 296,
    FIN_ACCOLADE = 297,
    VIRGULE = 298,
    PLUS = 299,
    MOINS = 300,
    FOIS = 301,
    DIVISION = 302,
    ADRESSE = 303,
    SYMBOLE = 304,
    LECTURE_MEMOIRE = 305,
    POINT = 306,
    EXCLAMATION = 307,
    INTERROGATION = 308,
    DEUX_POINTS = 309,
    INFERIEUR = 310,
    INFERIEUR_INFERIEUR = 311,
    SUPERIEUR = 312,
    SUPERIEUR_SUPERIEUR = 313,
    INFERIEUR_SUPERIEUR = 314,
    EGAL = 315,
    INFERIEUR_OU_EGAL = 316,
    SUPERIEUR_OU_EGAL = 317,
    POINT_VIRGULE = 318,
    QUAND = 319,
    PAS_QUAND = 320,
    LEVE = 321,
    DEFINI = 322,
    INITIALISE = 323,
    DANS = 324,
    EST = 325,
    APPELABLE = 326,
    DEBUGUEUR = 327,
    ARRET = 328,
    AJOUT = 329,
    ENLEVE = 330,
    LECTURE = 331,
    ECRITURE = 332,
    ACCES = 333,
    SUPPRESSION = 334,
    EXPLIQUE = 335,
    AFFECTATION = 336,
    INSTRUCTION_LABEL = 337,
    INSTRUCTION_SYMBOL = 338,
    INSTRUCTION_GOTO = 339,
    INSTRUCTION_CALL = 340,
    INSTRUCTION_RETURN = 341,
    INSTRUCTION_SHUTDOWN = 342,
    INSTRUCTION_LOCAL = 343,
    INSTRUCTION_SHIFT = 344,
    INSTRUCTION_LIBRARY = 345,
    INSTRUCTION_FLAG = 346,
    INSTRUCTION_INTERRUPTION = 347,
    INSTRUCTION_MEMORY = 348,
    INSTRUCTION_DEBUG = 349,
    FIN_LIGNE = 350
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  struct ValeurProgramme  YYSTYPE;
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



int machineelementsprogrammeanalyseurparse (void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position);

#endif /* !YY_MACHINEELEMENTSPROGRAMMEANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 287 "analyseur.syn.cpp" /* yacc.c:358  */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   610

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  96
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  211
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  283

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   350

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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   138,   138,   144,   146,   150,   153,   158,   162,   168,
     172,   176,   180,   184,   188,   192,   196,   200,   204,   208,
     212,   216,   222,   226,   230,   234,   238,   244,   250,   256,
     260,   266,   270,   276,   282,   286,   292,   297,   301,   308,
     312,   316,   320,   324,   328,   335,   341,   345,   349,   353,
     357,   361,   367,   371,   377,   381,   388,   392,   396,   400,
     404,   408,   412,   416,   422,   426,   433,   437,   441,   447,
     451,   458,   462,   468,   472,   479,   483,   487,   493,   497,
     504,   525,   529,   535,   539,   543,   547,   551,   557,   576,
     580,   585,   592,   596,   603,   609,   615,   619,   626,   630,
     634,   638,   642,   646,   650,   654,   658,   662,   666,   670,
     674,   680,   687,   690,   694,   699,   706,   712,   716,   720,
     724,   732,   735,   739,   745,   749,   753,   757,   761,   765,
     769,   773,   777,   783,   787,   794,   798,   804,   808,   814,
     818,   826,   829,   835,   839,   843,   847,   851,   855,   859,
     863,   867,   871,   878,   881,   888,   892,   896,   902,   906,
     910,   914,   918,   922,   926,   930,   934,   938,   942,   946,
     952,   956,   962,   965,   972,   975,   981,   985,   990,   994,
     998,  1002,  1016,  1020,  1024,  1028,  1032,  1036,  1040,  1044,
    1048,  1052,  1056,  1060,  1064,  1068,  1072,  1078,  1082,  1086,
    1090,  1094,  1098,  1102,  1106,  1110,  1114,  1118,  1122,  1126,
    1130,  1134
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_INVALIDE_", "ENTIER", "IDENTIFIANT",
  "MOT_CLEF", "CHAINE_MONOLIGNE", "CHAINE_MULTILIGNE", "VRAI", "FAUX",
  "CONST", "POINTEUR_COURANT", "TAILLE", "CASCADE_MC", "GLOBAL_MC",
  "TYPE_ENTIER", "TYPE_CHAINE", "TYPE_BOOLEEN", "TYPE_POINTEUR",
  "TYPE_BIBLIOTHEQUE", "TYPE_SYMBOLE", "TYPE_POINT_ENTREE_EXTENSION",
  "TYPE_INTERRUPTION", "TYPE_AUTOMATIQUE", "INTERRUPTION_ECHEC",
  "INTERRUPTION_PROCESSEUR", "INTERRUPTION_MEMOIRE",
  "INTERRUPTION_TERMINAISON", "INTERRUPTION_NUMERIQUE",
  "INTERRUPTION_PREMIER", "INTERRUPTION_SECOND", "INTERRUPTION_SECURITE",
  "INTERRUPTION_PERIPHERIQUE", "INTERRUPTION_CLONE",
  "INTERRUPTION_TERMINAL", "INTERRUPTION_GENERIC", "DEBUT_PARENTHESE",
  "FIN_PARENTHESE", "DEBUT_CROCHET", "FIN_CROCHET", "DEBUT_ACCOLADE",
  "FIN_ACCOLADE", "VIRGULE", "PLUS", "MOINS", "FOIS", "DIVISION",
  "ADRESSE", "SYMBOLE", "LECTURE_MEMOIRE", "POINT", "EXCLAMATION",
  "INTERROGATION", "DEUX_POINTS", "INFERIEUR", "INFERIEUR_INFERIEUR",
  "SUPERIEUR", "SUPERIEUR_SUPERIEUR", "INFERIEUR_SUPERIEUR", "EGAL",
  "INFERIEUR_OU_EGAL", "SUPERIEUR_OU_EGAL", "POINT_VIRGULE", "QUAND",
  "PAS_QUAND", "LEVE", "DEFINI", "INITIALISE", "DANS", "EST", "APPELABLE",
  "DEBUGUEUR", "ARRET", "AJOUT", "ENLEVE", "LECTURE", "ECRITURE", "ACCES",
  "SUPPRESSION", "EXPLIQUE", "AFFECTATION", "INSTRUCTION_LABEL",
  "INSTRUCTION_SYMBOL", "INSTRUCTION_GOTO", "INSTRUCTION_CALL",
  "INSTRUCTION_RETURN", "INSTRUCTION_SHUTDOWN", "INSTRUCTION_LOCAL",
  "INSTRUCTION_SHIFT", "INSTRUCTION_LIBRARY", "INSTRUCTION_FLAG",
  "INSTRUCTION_INTERRUPTION", "INSTRUCTION_MEMORY", "INSTRUCTION_DEBUG",
  "FIN_LIGNE", "$accept", "programme", "liste_instructions", "ancre",
  "instruction", "instruction_affectation", "instruction_saut",
  "instruction_appel", "instruction_retour", "instruction_arret",
  "instruction_local", "instruction_decalage", "instruction_bibliotheque",
  "instruction_drapeau", "instruction_interruption", "instruction_memoire",
  "instruction_debogage", "instruction_extension", "valeur",
  "valeur_immediate", "entier", "entier_immediat", "chaine",
  "chaine_immediate", "booleen", "booleen_immediat", "pointeur",
  "pointeur_immediat", "adresse", "adresse_immediate", "symbole_immediat",
  "point_entree_extension", "point_entree_extension_immediat",
  "point_entree_extension_pur", "interruption", "interruption_immediate",
  "bloc", "liste_valeurs", "lecture_memoire", "cible", "condition",
  "expression_condition", "liste_types_avec_alias", "types_avec_alias",
  "types_anonymes", "liste_types_anonymes", "repetition", "type_memoire",
  "liste_parametres", "parametre", "marqueur", "mot_clef", "cascade",
  "globale", "operation", "type_arret_memoire", "substitution_mot_clef",
  "substitution_instruction", YY_NULLPTR
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
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350
};
# endif

#define YYPACT_NINF -196

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-196)))

#define YYTABLE_NINF -210

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -196,    15,   277,  -196,  -196,   -30,  -196,  -196,  -196,  -196,
      20,  -196,    66,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,    66,   478,    34,   228,    68,
     -15,    20,   339,    30,  -196,  -196,    40,    48,   211,   211,
       9,    19,     0,   189,   148,   172,   530,    -4,   168,  -196,
     -36,   -34,  -196,  -196,  -196,  -196,  -196,  -196,  -196,   -13,
    -196,  -196,    28,  -196,    39,    42,  -196,  -196,  -196,  -196,
    -196,    31,  -196,  -196,  -196,  -196,  -196,    52,  -196,    36,
      74,   -30,    58,  -196,  -196,  -196,  -196,    35,   119,  -196,
       6,  -196,  -196,  -196,    45,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,    21,  -196,    49,    72,    88,  -196,   250,  -196,
       1,    75,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
      65,    66,    65,   339,   339,  -196,    65,  -196,  -196,  -196,
      66,   -15,  -196,   148,   148,  -196,   558,   558,   211,  -196,
    -196,  -196,   233,   148,  -196,  -196,   478,   443,  -196,  -196,
     -15,   -15,   -15,   -15,    44,    66,    96,  -196,  -196,    44,
      44,    44,  -196,   478,    90,    66,  -196,  -196,  -196,  -196,
      66,   519,  -196,   396,  -196,    65,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,   211,   211,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,   233,  -196,   107,  -196,
      92,   122,  -196,  -196,    31,  -196,  -196,  -196,  -196,  -196,
     -15,  -196,  -196,  -196,  -196,  -196,  -196,  -196,   115,   137,
     150,  -196,   290,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,   -31,   233,   186,    44,  -196,  -196,   396,
    -196,  -196,  -196,  -196,   165,   122,   233,  -196,  -196,  -196,
    -196,  -196,  -196
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,    66,    81,    71,    72,    75,    76,
       0,    82,     0,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,   112,     0,     0,     0,
       0,     0,     0,     0,   210,   211,   199,   200,   201,   202,
     121,   121,   172,   206,   198,   207,   208,   174,   197,     6,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,    54,    56,    57,    58,
      59,     0,    83,    61,    60,    94,    62,     0,    55,     0,
       0,     0,     0,    81,    68,    78,    79,     0,     0,   113,
       0,    67,    88,    84,     0,    89,    69,    70,   116,   110,
     117,   132,     0,    64,     0,    69,     0,    73,     0,   119,
      65,     0,    77,   153,    92,    93,     7,     8,   118,   120,
     121,     0,   121,     0,     0,    29,   121,    65,    31,   173,
       0,     0,    34,     0,     0,    37,     0,     0,    44,    96,
      97,   175,     0,    46,   176,   177,     0,     0,     5,     4,
       0,     0,     0,     0,     0,     0,     0,    95,    63,     0,
       0,     0,   111,   114,     0,     0,   125,   124,   126,   127,
       0,     0,   131,    52,    27,   121,    30,   122,   123,    32,
      33,    35,    36,    38,    42,    40,    43,   144,   145,   146,
     147,   148,   149,   150,   151,   143,     0,   152,    45,   133,
     135,   141,    47,    50,    49,   178,   179,   180,   181,    51,
       0,    24,    25,    26,    22,    80,    23,   153,     0,     0,
       0,   115,     0,   128,   129,   130,   170,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   166,   169,   167,   158,
     160,   159,   161,   162,   163,   164,   165,   168,   191,   192,
     193,   194,   195,   196,   155,   154,   156,   157,   171,    28,
      41,    39,   139,     0,     0,     0,     0,   137,    48,    53,
      85,    86,    87,    90,     0,   141,     0,   134,   136,   142,
      91,   138,   140
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196,    14,  -196,
     -35,     3,   -26,    24,  -196,   -12,    60,    26,    61,  -196,
      32,   171,   -29,   196,    -9,   -37,  -196,  -196,    -2,    16,
     -24,    50,  -196,   -54,  -195,  -196,   -58,    53,     4,  -196,
    -196,  -196,  -196,  -196,  -196,  -196,  -196,  -196
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,   254,    66,
     102,   103,   104,    96,   106,    69,    84,    85,    71,    72,
     109,   197,    74,    75,   138,    76,    77,    90,    86,   111,
     125,   112,   198,   199,   200,   263,   267,   201,   173,   255,
     256,   257,   130,   142,   147,   210,   258,    79
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      78,   262,    99,    95,   114,    67,   126,   275,   131,   139,
     -74,   141,   276,   122,   129,     3,    65,   128,   133,   135,
     107,    80,    25,     4,    78,    81,    68,    97,    70,    67,
     110,   115,    12,    28,    73,    81,   119,   119,    91,   127,
      89,   127,    97,    97,   140,   116,   162,  -209,     4,   163,
      68,  -205,    70,   117,   120,   121,   105,    12,    73,   148,
    -203,   149,   118,   118,    27,     6,     7,   -70,   150,    30,
    -204,    83,  -120,   123,   124,     6,     7,   154,    11,   157,
      30,   282,   159,   123,   124,    87,    88,   156,    93,    27,
     165,    98,   164,   108,    30,    30,   174,   167,   176,   139,
     139,   217,   179,    25,   132,    94,   158,   182,   183,   151,
     139,   107,   107,   114,    28,   166,    30,   202,    30,   215,
     152,   110,   110,   153,   218,   219,   220,   184,   185,   123,
     124,    97,    97,   155,   140,   140,   119,   222,   209,   265,
     115,    97,   114,  -118,    78,   140,   172,   105,   105,    67,
     264,   259,   127,   270,   186,     6,     7,   127,   127,   127,
     203,    78,   118,   160,   161,   154,    67,   114,   266,   115,
      68,    78,    70,   177,   178,   271,    67,   221,    73,     6,
       7,   175,   119,   119,   108,   108,   134,    68,   272,    70,
     180,   278,   181,     4,   115,    73,   274,    68,    30,    70,
     260,   261,    12,   280,   113,    73,    82,   204,   118,   118,
     277,   211,   212,   213,   214,   216,   100,   281,     6,     7,
      97,   269,    30,     0,   225,   223,    25,     0,     0,     0,
     224,   279,    92,    83,    27,   114,     0,    28,    81,    30,
      11,   143,   144,   145,     0,     0,     0,   114,   146,   187,
     188,   189,   190,   191,   192,   193,   194,   195,     0,     0,
      29,    30,   115,     0,   127,    25,     0,    78,     0,     0,
     196,   268,    67,     0,   115,     0,    28,     0,    30,     0,
       0,     4,     5,    30,     6,     7,     8,     9,    10,    11,
      12,     0,     0,    68,     0,    70,     0,     6,     7,     0,
       0,    73,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,    26,   168,   169,   170,
     171,     0,    27,     0,     0,    28,    29,    30,   273,    31,
      32,    33,     0,     0,     0,     0,     0,     0,     0,     0,
      30,    34,    35,     4,   100,     0,     6,     7,     8,     9,
       0,     0,    12,     0,     0,     0,     0,     0,     0,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,    25,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    28,    29,    30,
       0,     0,    32,     0,     0,     0,     0,     0,     0,     0,
       4,     5,   226,     6,     7,     8,     9,    10,    11,    12,
       0,   101,   227,   228,   229,   230,   231,   232,   233,   234,
     235,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     0,     0,     0,   236,   237,   238,
       0,    27,     0,     0,    28,    29,    30,     0,    31,    32,
       0,   239,   240,   241,   242,   243,   244,   245,   246,   247,
       0,     0,   248,   249,   250,   251,   252,   253,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,     0,     4,     5,     0,     6,     7,     8,     9,    10,
      11,    12,     0,    30,     0,    31,     0,     0,     0,     0,
       0,     0,     0,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,     0,     0,     0,   205,
     206,   207,   208,    27,    81,     0,    28,    29,    30,     0,
      31,    32,     0,     0,     0,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   136,   137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,    31,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    30,     0,
      31
};

static const yytype_int16 yycheck[] =
{
       2,   196,    31,    29,    33,     2,    41,    38,    43,    46,
       9,    15,    43,     4,    14,     0,     2,    41,    44,    45,
      32,    51,    37,     4,    26,     5,     2,    29,     2,    26,
      32,    33,    13,    48,     2,     5,    38,    39,     4,    41,
      26,    43,    44,    45,    46,     5,    40,    51,     4,    43,
      26,    51,    26,     5,    38,    39,    32,    13,    26,    95,
      51,    95,    38,    39,    45,     7,     8,    66,    81,    50,
      51,     5,    71,    64,    65,     7,     8,    46,    12,     5,
      50,   276,    47,    64,    65,    25,    25,    51,    28,    45,
      69,    30,    94,    32,    50,    50,   120,     9,   122,   136,
     137,     5,   126,    37,    43,    37,    82,   133,   134,    81,
     147,   123,   124,   142,    48,    66,    50,   143,    50,   154,
      81,   123,   124,    81,   159,   160,   161,   136,   137,    64,
      65,   133,   134,    81,   136,   137,   138,    47,   147,    47,
     142,   143,   171,    71,   146,   147,    71,   123,   124,   146,
      43,   175,   154,    38,   138,     7,     8,   159,   160,   161,
     146,   163,   138,    44,    45,    46,   163,   196,    46,   171,
     146,   173,   146,   123,   124,    38,   173,   163,   146,     7,
       8,   121,   184,   185,   123,   124,    14,   163,    38,   163,
     130,     5,   131,     4,   196,   163,   222,   173,    50,   173,
     184,   185,    13,    38,    33,   173,    10,   146,   184,   185,
     264,   150,   151,   152,   153,   155,     5,   275,     7,     8,
     222,   217,    50,    -1,   171,   165,    37,    -1,    -1,    -1,
     170,   266,     4,     5,    45,   264,    -1,    48,     5,    50,
      12,    73,    74,    75,    -1,    -1,    -1,   276,    80,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      49,    50,   264,    -1,   266,    37,    -1,   269,    -1,    -1,
      37,   210,   269,    -1,   276,    -1,    48,    -1,    50,    -1,
      -1,     4,     5,    50,     7,     8,     9,    10,    11,    12,
      13,    -1,    -1,   269,    -1,   269,    -1,     7,     8,    -1,
      -1,   269,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    -1,    39,    67,    68,    69,
      70,    -1,    45,    -1,    -1,    48,    49,    50,    38,    52,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    64,    65,     4,     5,    -1,     7,     8,     9,    10,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      -1,    72,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    41,    42,    43,
      -1,    45,    -1,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    -1,    66,    67,    68,    69,    70,    71,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    76,
      77,    78,    79,    45,     5,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    97,    98,     0,     4,     5,     7,     8,     9,    10,
      11,    12,    13,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    39,    45,    48,    49,
      50,    52,    53,    54,    64,    65,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   117,   119,   121,
     123,   124,   125,   126,   128,   129,   131,   132,   134,   153,
      51,     5,   129,     5,   122,   123,   134,   122,   124,   114,
     133,     4,     4,   122,    37,   118,   119,   134,   124,   128,
       5,    72,   116,   117,   118,   119,   120,   121,   124,   126,
     134,   135,   137,   127,   128,   134,     5,     5,   119,   134,
     135,   135,     4,    64,    65,   136,   116,   134,   136,    14,
     148,   116,   124,   118,    14,   118,    14,    15,   130,   131,
     134,    15,   149,    73,    74,    75,    80,   150,    95,    95,
      81,    81,    81,    81,    46,    81,    51,     5,   119,    47,
      44,    45,    40,    43,   134,    69,    66,     9,    67,    68,
      69,    70,    71,   144,   136,   122,   136,   137,   137,   136,
     122,   124,   118,   118,   130,   130,   135,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    37,   127,   138,   139,
     140,   143,   118,   114,   124,    76,    77,    78,    79,   130,
     151,   124,   124,   124,   124,   116,   122,     5,   116,   116,
     116,   114,    47,   122,   122,   143,     6,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    41,    42,    43,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    66,    67,
      68,    69,    70,    71,   114,   145,   146,   147,   152,   136,
     135,   135,   140,   141,    43,    47,    46,   142,   124,   144,
      38,    38,    38,    38,   118,    38,    43,   139,     5,   116,
      38,   142,   140
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    96,    97,    98,    98,    98,    98,    99,    99,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   101,   101,   101,   101,   101,   102,   103,   104,
     104,   105,   105,   106,   107,   107,   108,   109,   109,   110,
     110,   110,   110,   110,   110,   111,   112,   112,   112,   112,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   116,   116,   117,   117,   117,   118,
     118,   119,   119,   120,   120,   121,   121,   121,   122,   122,
     123,   123,   123,   124,   124,   124,   124,   124,   125,   126,
     126,   126,   127,   127,   128,   129,   130,   130,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   132,   133,   133,   133,   133,   134,   135,   135,   135,
     135,   136,   136,   136,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   138,   138,   139,   139,   140,   140,   141,
     141,   142,   142,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   144,   144,   145,   145,   145,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     147,   147,   148,   148,   149,   149,   150,   150,   151,   151,
     151,   151,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     3,     3,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     4,     2,
       3,     2,     3,     3,     2,     3,     3,     2,     3,     4,
       3,     4,     3,     3,     2,     3,     2,     3,     4,     3,
       3,     3,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       3,     1,     1,     1,     2,     5,     5,     5,     2,     2,
       5,     6,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     0,     1,     2,     3,     2,     1,     1,     1,
       1,     0,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     2,     1,     1,     3,     1,     3,     2,     4,     1,
       3,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
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
      yyerror (&yylloc, scanner, nom, code, erreur, position, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner, nom, code, erreur, position); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (nom);
  YYUSE (code);
  YYUSE (erreur);
  YYUSE (position);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, nom, code, erreur, position);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, nom, code, erreur, position);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, nom, code, erreur, position); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (nom);
  YYUSE (code);
  YYUSE (erreur);
  YYUSE (position);
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
yyparse (void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position)
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
#line 45 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1429  */
{ 
#if YYDEBUG==1
	machineelementsprogrammeanalyseurdebug=1;
#endif
}

#line 1621 "analyseur.syn.cpp" /* yacc.c:1429  */
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
#line 139 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1811 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 144 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1818 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 147 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	code->ajoute_instruction((yyvsp[-1].instruction));
}
#line 1826 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 151 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1833 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 154 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
}
#line 1840 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 159 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	code->ajoute_etiquette((yyvsp[0].chaine));
}
#line 1848 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 163 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	code->ajoute_symbole((yyvsp[0].chaine));
}
#line 1856 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 169 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1864 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 173 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1872 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 177 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1880 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 181 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1888 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 185 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1896 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 189 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1904 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 193 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1912 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 197 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1920 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 201 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1928 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 205 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1936 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 209 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1944 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 213 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1952 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 217 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = (yyvsp[0].instruction);
}
#line 1960 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 223 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::AffectationValeur>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].valeur),(yyvsp[0].adresse));
}
#line 1968 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 227 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::AffectationBloc>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].bloc),(yyvsp[0].valeur));
}
#line 1976 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 231 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::AffectationInstruction>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].instruction),(yyvsp[0].adresse));
}
#line 1984 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 235 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::AffectationInstruction>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].instruction),(yyvsp[0].adresse));
}
#line 1992 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 239 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::AffectationInstruction>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].instruction),(yyvsp[0].adresse));
}
#line 2000 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 245 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Saut>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].cible),(yyvsp[0].condition));
}
#line 2008 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 251 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Appel>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].cible),(yyvsp[-1].valeur),(yyvsp[0].condition));
}
#line 2016 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 257 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Retour>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].condition));
}
#line 2024 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 261 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Retour>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].entier),(yyvsp[0].condition));
}
#line 2032 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 267 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::ArretProcesseur>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].condition));
}
#line 2040 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 271 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::ArretMachine>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].valeur),(yyvsp[0].condition));
}
#line 2048 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 277 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Local>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].booleen),(yyvsp[0].valeur));
}
#line 2056 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 283 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Decalage>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].adresse));
}
#line 2064 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 287 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Decalage>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].valeur),(yyvsp[0].adresse));
}
#line 2072 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 293 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Bibliotheque>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].valeur),(yyvsp[0].valeur));
}
#line 2080 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 298 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Drapeau>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].valeur));
}
#line 2088 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 302 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Drapeau>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].valeur),true);
}
#line 2096 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 309 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Interruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),true,false,(yyvsp[-1].valeur),(yyvsp[0].cible));
}
#line 2104 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 313 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Interruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),true,false,(yyvsp[0].valeur));
}
#line 2112 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 317 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Interruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),false,true,(yyvsp[-1].valeur),(yyvsp[0].cible));
}
#line 2120 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 321 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Interruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),false,true,(yyvsp[0].valeur));
}
#line 2128 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 325 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Interruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),false,false,(yyvsp[-1].valeur),(yyvsp[0].cible));
}
#line 2136 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 329 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Interruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),false,false,(yyvsp[0].valeur));
}
#line 2144 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 336 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Memoire>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].booleen),(yyvsp[0].liste_types_avec_alias));
}
#line 2152 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 342 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::DebogageArretProcesseur>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position));
}
#line 2160 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 346 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::DebogageArretProcesseur>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].valeur));
}
#line 2168 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 350 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::DebogageArretMemoire>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-2].booleen),(yyvsp[-1].type_arret_memoire),(yyvsp[0].adresse));
}
#line 2176 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 354 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::DebogageExplicationAdresse>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].adresse));
}
#line 2184 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 358 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::DebogageExplicationValeur>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[0].valeur));
}
#line 2192 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 362 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::DebogageArretInterruption>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].booleen),(yyvsp[0].valeur));
}
#line 2200 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 368 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Extension>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),(yyvsp[-1].valeur),(yyvsp[0].liste_parametres));
}
#line 2208 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 372 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.instruction) = std::make_shared<SVM_Instruction::Extension>(SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position),std::make_shared<SVM_Programme::PointEntreeExtension>(SVM_Valeur::PointEntreeExtension((yyvsp[-3].chaine),(yyvsp[-1].chaine))),(yyvsp[0].liste_parametres));
}
#line 2216 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 378 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2224 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 382 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return true; },"any type");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2233 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 389 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2241 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 393 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].chaine_immediate);
}
#line 2249 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 397 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2257 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 401 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2265 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 405 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2273 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 409 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2281 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 413 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2289 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 417 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::ConstanteExtension>((yyvsp[-1].point_entree_extension),(yyvsp[0].chaine_immediate),SVM_Valeur::PositionInstruction(nom,(yyloc).first_line,(yyloc).last_line,position));
}
#line 2297 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 423 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2305 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 427 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::ENTIER; },"INT");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2314 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 434 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::Entier>((yyvsp[0].entier));
}
#line 2322 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 438 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::Entier>(-(yyvsp[0].entier));
}
#line 2330 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 442 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::TaillePointeur>((yyvsp[0].valeur));
}
#line 2338 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 448 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].chaine_immediate);
}
#line 2346 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 452 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::CHAINE; },"STR");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2355 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 459 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine_immediate) = std::make_shared<SVM_Programme::Chaine>((yyvsp[0].chaine),false);
}
#line 2363 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 463 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine_immediate) = std::make_shared<SVM_Programme::Chaine>((yyvsp[0].chaine),true);
}
#line 2371 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 469 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2379 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 473 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::BOOLEEN; },"BLN");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2388 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 480 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::Booleen>(true);
}
#line 2396 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 484 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::Booleen>(false);
}
#line 2404 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 488 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::BooleenCondition>((yyvsp[0].expression_condition));
}
#line 2412 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 494 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2420 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 498 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::POINTEUR; },"PTR");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2429 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 505 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	auto a = std::dynamic_pointer_cast<SVM_Programme::AdresseConstante>((yyvsp[-2].adresse));
	auto t = std::dynamic_pointer_cast<SVM_Programme::Entier>((yyvsp[0].valeur));
	if(static_cast<bool>(a) and static_cast<bool>(t))
	{
		size_t ra = *a;
		long long int rt = *t;
		if(rt<0)
		{
			std::ostringstream oss;
			oss << "Invalid pointer size " << rt << ".";
			return ::machineelementsprogrammeanalyseurerror(&(yyloc),scanner,nom,code,erreur,position,oss.str());
		}
		(yyval.valeur) = std::make_shared<SVM_Programme::Pointeur>(ra,static_cast<size_t>(rt));
	}
	else
	{
		(yyval.valeur) = std::make_shared<SVM_Programme::PointeurConstruit>((yyvsp[-2].adresse),(yyvsp[0].valeur));
	}
}
#line 2454 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 526 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::PointeurAlias>((yyvsp[0].chaine));
}
#line 2462 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 530 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::PointeurCourant>();
}
#line 2470 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 536 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.adresse) = (yyvsp[0].adresse);
}
#line 2478 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 540 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.adresse) = std::make_shared<SVM_Programme::AdresseDepuisPointeur>((yyvsp[0].valeur));
}
#line 2486 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 544 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.adresse) = std::make_shared<SVM_Programme::AdresseDepuisIndicePointeur>((yyvsp[-3].valeur),(yyvsp[-1].valeur));
}
#line 2494 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 548 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.adresse) = std::make_shared<SVM_Programme::AdresseDecalage>((yyvsp[-3].adresse),1,(yyvsp[-1].valeur));
}
#line 2502 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 552 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.adresse) = std::make_shared<SVM_Programme::AdresseDecalage>((yyvsp[-3].adresse),-1,(yyvsp[-1].valeur));
}
#line 2510 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 558 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.adresse) = std::make_shared<SVM_Programme::AdresseConstante>((yyvsp[0].entier));
}
#line 2518 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 577 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::SymboleLocal>((yyvsp[0].valeur));
}
#line 2526 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 581 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[-2].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE; },"LIB");
	(yyval.valeur) = std::make_shared<SVM_Programme::SymboleDebut>((yyvsp[-2].lecture_memoire));
}
#line 2535 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 586 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[-3].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE; },"LIB");
	(yyval.valeur) = std::make_shared<SVM_Programme::SymboleComplet>((yyvsp[-3].lecture_memoire),(yyvsp[-1].valeur));
}
#line 2544 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 593 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2552 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 597 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION; },"PEP");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2561 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 604 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::PointEntreeExtension>((yyvsp[0].point_entree_extension));
}
#line 2569 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 610 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.point_entree_extension) = SVM_Valeur::PointEntreeExtension((yyvsp[-2].chaine),(yyvsp[0].chaine));
}
#line 2577 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 616 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2585 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 620 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::INTERRUPTION; },"IRQ");
	(yyval.valeur) = (yyvsp[0].lecture_memoire);
}
#line 2594 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 627 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::ECHEC);
}
#line 2602 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 631 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR);
}
#line 2610 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 635 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE);
}
#line 2618 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 639 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::TERMINAISON);
}
#line 2626 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 643 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::NUMERIQUE);
}
#line 2634 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 647 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::PREMIER);
}
#line 2642 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 651 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::SECOND);
}
#line 2650 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 655 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::SECURITE);
}
#line 2658 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 659 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::PERIPHERIQUE);
}
#line 2666 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 663 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::CLONE);
}
#line 2674 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 667 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::TERMINAL);
}
#line 2682 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 671 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionInterne>(SVM_Valeur::Interruption::InterruptionInterne::GENERIQUE);
}
#line 2690 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 675 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = std::make_shared<SVM_Programme::InterruptionExterne>((yyvsp[0].valeur));
}
#line 2698 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 681 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.bloc) = (yyvsp[-1].bloc);
}
#line 2706 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 687 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.bloc) = { SVM_Programme::ValeurSP() };
}
#line 2714 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 691 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.bloc) = { (yyvsp[0].valeur) };
}
#line 2722 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 695 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.bloc) = (yyvsp[-1].bloc);
	(yyval.bloc).push_back(SVM_Programme::ValeurSP());
}
#line 2731 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 700 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.bloc) = (yyvsp[-2].bloc);
	(yyval.bloc).push_back((yyvsp[0].valeur));
}
#line 2740 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 707 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.lecture_memoire) = std::make_shared<SVM_Programme::LectureMemoire>((yyvsp[0].adresse));
}
#line 2748 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 713 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.cible) = std::make_shared<SVM_Programme::CibleStatique>((yyvsp[0].chaine));
}
#line 2756 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 717 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.cible) = std::make_shared<SVM_Programme::CibleDynamique>((yyvsp[0].chaine_immediate));
}
#line 2764 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 721 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.cible) = std::make_shared<SVM_Programme::CibleDynamique>((yyvsp[0].valeur));
}
#line 2772 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 725 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyvsp[0].lecture_memoire)->type_voulu([] (const SVM_Memoire::Type& type) -> bool { return type==SVM_Memoire::Type::TypeInterne::CHAINE or type==SVM_Memoire::Type::TypeInterne::SYMBOLE; },"STR or SYM");
	(yyval.cible) = std::make_shared<SVM_Programme::CibleDynamique>((yyvsp[0].lecture_memoire));
}
#line 2781 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 732 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.condition) = std::make_shared<SVM_Programme::PasDeCondition>();
}
#line 2789 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 736 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.condition) = std::make_shared<SVM_Programme::ConditionNormale>((yyvsp[0].expression_condition));
}
#line 2797 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 740 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.condition) = std::make_shared<SVM_Programme::ConditionInverse>((yyvsp[0].expression_condition));
}
#line 2805 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 746 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionBooleen>((yyvsp[-1].valeur));
}
#line 2813 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 750 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionDrapeau>((yyvsp[-1].valeur));
}
#line 2821 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 754 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionAdresseDefinie>((yyvsp[-1].adresse));
}
#line 2829 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 758 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionAdresseInitialisee>((yyvsp[-1].adresse));
}
#line 2837 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 762 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionIndiceDansPointeur>((yyvsp[-2].valeur),(yyvsp[0].valeur));
}
#line 2845 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 766 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionAdresseDansPointeur>((yyvsp[-2].adresse),(yyvsp[0].valeur));
}
#line 2853 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 770 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionAdresseEstType>((yyvsp[-2].adresse),(yyvsp[0].type));
}
#line 2861 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 774 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionCibleAppelable>((yyvsp[-1].cible));
}
#line 2869 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 778 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.expression_condition) = std::make_shared<SVM_Instruction::ConditionDebugueur>();
}
#line 2877 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 784 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_types_avec_alias) = { (yyvsp[0].types_avec_alias) };
}
#line 2885 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 788 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_types_avec_alias) = (yyvsp[-2].liste_types_avec_alias);
	(yyval.liste_types_avec_alias).push_back((yyvsp[0].types_avec_alias));
}
#line 2894 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 795 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.types_avec_alias) = std::make_shared<SVM_Programme::DescriptionBlocAlias>((yyvsp[0].types_anonymes));
}
#line 2902 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 799 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.types_avec_alias) = std::make_shared<SVM_Programme::DescriptionBlocAlias>((yyvsp[-2].types_anonymes),(yyvsp[0].chaine));
}
#line 2910 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 805 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.types_anonymes) = std::make_shared<SVM_Programme::DescriptionBlocAtome>((yyvsp[-1].type),(yyvsp[0].valeur));
}
#line 2918 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 809 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.types_anonymes) = std::make_shared<SVM_Programme::DescriptionBlocCompose>((yyvsp[-2].liste_types_anonymes),(yyvsp[0].valeur));
}
#line 2926 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 815 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_types_anonymes) = { (yyvsp[0].types_anonymes) };
}
#line 2934 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 819 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_types_anonymes) = (yyvsp[-2].liste_types_anonymes);
	(yyval.liste_types_anonymes).push_back((yyvsp[0].types_anonymes));
}
#line 2943 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 826 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = SVM_Programme::ValeurSP();
}
#line 2951 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 830 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.valeur) = (yyvsp[0].valeur);
}
#line 2959 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 836 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::AUTOMATIQUE);
}
#line 2967 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 840 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::ENTIER);
}
#line 2975 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 844 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::CHAINE);
}
#line 2983 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 848 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::BOOLEEN);
}
#line 2991 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 852 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::POINTEUR);
}
#line 2999 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 856 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE);
}
#line 3007 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 860 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::SYMBOLE);
}
#line 3015 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 864 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION);
}
#line 3023 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 868 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeInterne>(SVM_Memoire::Type::TypeInterne::INTERRUPTION);
}
#line 3031 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 872 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type) = std::make_shared<SVM_Programme::TypeExtension>((yyvsp[0].valeur));
}
#line 3039 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 878 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_parametres) = {};
}
#line 3047 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 882 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.liste_parametres) = (yyvsp[-1].liste_parametres);
	(yyval.liste_parametres).push_back((yyvsp[0].parametre));
}
#line 3056 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 889 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.parametre)=std::make_shared<SVM_Programme::ParametreValeur>((yyvsp[0].valeur));
}
#line 3064 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 893 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.parametre)=std::make_shared<SVM_Programme::ParametreMarqueur>((yyvsp[0].marqueur));
}
#line 3072 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 897 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.parametre)=std::make_shared<SVM_Programme::ParametreMotClef>((yyvsp[0].chaine));
}
#line 3080 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 903 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR;
}
#line 3088 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 907 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::SUPERIEUR;
}
#line 3096 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 911 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR_INFERIEUR;
}
#line 3104 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 915 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::SUPERIEUR_SUPERIEUR;
}
#line 3112 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 919 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR_SUPERIEUR;
}
#line 3120 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 923 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::EGAL;
}
#line 3128 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 927 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::INFERIEUR_OU_EGAL;
}
#line 3136 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 931 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::SUPERIEUR_OU_EGAL;
}
#line 3144 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 935 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::DEBUT_ACCOLADE;
}
#line 3152 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 939 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::VIRGULE;
}
#line 3160 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 943 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::POINT_VIRGULE;
}
#line 3168 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 947 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.marqueur) = SVM_Valeur::ParametreMarqueur::Marqueur::FIN_ACCOLADE;
}
#line 3176 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 953 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 3184 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 957 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = (yyvsp[0].chaine);
}
#line 3192 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 962 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = false;
}
#line 3200 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 966 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = true;
}
#line 3208 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 972 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = false;
}
#line 3216 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 976 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = true;
}
#line 3224 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 982 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = true;
}
#line 3232 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 986 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.booleen) = false;
}
#line 3240 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 991 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_arret_memoire) = SVM_Processeur::Instruction::DebogageArretMemoire::Type::LECTURE;
}
#line 3248 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 995 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_arret_memoire) = SVM_Processeur::Instruction::DebogageArretMemoire::Type::ECRITURE;
}
#line 3256 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 999 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_arret_memoire) = SVM_Processeur::Instruction::DebogageArretMemoire::Type::ACCES;
}
#line 3264 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 1003 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.type_arret_memoire) = SVM_Processeur::Instruction::DebogageArretMemoire::Type::SUPPRESSION;
}
#line 3272 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 1017 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "INT";
}
#line 3280 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 1021 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "STR";
}
#line 3288 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 1025 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "BLN";
}
#line 3296 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 1029 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "PTR";
}
#line 3304 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 1033 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "LIB";
}
#line 3312 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 1037 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "SYM";
}
#line 3320 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 1041 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "PEP";
}
#line 3328 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 1045 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "IRQ";
}
#line 3336 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 1049 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "AUTO";
}
#line 3344 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 1053 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "RAISED";
}
#line 3352 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 1057 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "DEFINED";
}
#line 3360 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 1061 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "INITIALISED";
}
#line 3368 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 1065 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "IN";
}
#line 3376 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 1069 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "IS";
}
#line 3384 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 1073 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "CALLABLE";
}
#line 3392 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1079 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "debug";
}
#line 3400 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1083 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "library";
}
#line 3408 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1087 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "label";
}
#line 3416 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1091 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "symbol";
}
#line 3424 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1095 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "goto";
}
#line 3432 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1099 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "call";
}
#line 3440 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1103 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "return";
}
#line 3448 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1107 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "shutdown";
}
#line 3456 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1111 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "local";
}
#line 3464 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1115 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "shift";
}
#line 3472 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1119 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "flag";
}
#line 3480 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1123 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "interruption";
}
#line 3488 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1127 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "memory";
}
#line 3496 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1131 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "when";
}
#line 3504 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1135 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1646  */
    {
	(yyval.chaine) = "unless";
}
#line 3512 "analyseur.syn.cpp" /* yacc.c:1646  */
    break;


#line 3516 "analyseur.syn.cpp" /* yacc.c:1646  */
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
      yyerror (&yylloc, scanner, nom, code, erreur, position, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, nom, code, erreur, position, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, nom, code, erreur, position);
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
                  yystos[yystate], yyvsp, yylsp, scanner, nom, code, erreur, position);
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
  yyerror (&yylloc, scanner, nom, code, erreur, position, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, nom, code, erreur, position);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, nom, code, erreur, position);
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
#line 1141 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1906  */


int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position, std::string mesg)
{
	std::ostringstream oss;
	oss << "Unable to parse code " << nom << ": " << mesg; 
	erreur=std::make_shared<SVM_Valeur::Interruption>(SVM_Valeur::Interruption::InterruptionInterne::ECHEC,oss.str());
	erreur->position(SVM_Valeur::PositionInstruction(nom,llocp->first_line,llocp->last_line,position));
	return 1;
}

