/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 48 "/home/pappy/Programmation/svm/projet/src/machine/extensions/analyseur/analyseur.syn.ypp" /* yacc.c:1909  */


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

#line 74 "analyseur.syn.hpp" /* yacc.c:1909  */

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
