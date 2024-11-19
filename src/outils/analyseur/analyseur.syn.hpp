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
#line 49 "/home/pappy/Programmation/svm/projet/src/outils/analyseur/analyseur.syn.ypp" /* yacc.c:1909  */


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


#line 73 "analyseur.syn.hpp" /* yacc.c:1909  */

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
