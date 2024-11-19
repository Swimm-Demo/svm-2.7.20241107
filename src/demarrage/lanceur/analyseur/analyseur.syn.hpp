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
#line 45 "/home/pappy/Programmation/svm/projet/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" /* yacc.c:1909  */


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



#line 78 "analyseur.syn.hpp" /* yacc.c:1909  */

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
