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

#ifndef YY_MACHINEDEBUGUEURANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
# define YY_MACHINEDEBUGUEURANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int machinedebugueuranalyseurdebug;
#endif
/* "%code requires" blocks.  */
#line 45 "/home/pappy/Programmation/svm/projet/src/machine/debugueur/analyseur/analyseur.syn.ypp" /* yacc.c:1909  */


struct ValeurJSON
{
	SVM::Machine::Debugueur::JSON_Interne::Entier::Type entier;
	SVM::Machine::Debugueur::JSON_Interne::Chaine::Type chaine;
	SVM::Machine::Debugueur::JSON_Interne::Objet::Membres membres_objet;
	std::pair<std::string,SVM::Machine::Debugueur::JSON_Interne::ValeurSP> membre_objet;
	SVM::Machine::Debugueur::JSON_Interne::Tableau::Membres membres_tableau;
	SVM::Machine::Debugueur::JSON_Interne::EntierSP valeur_entier;
	SVM::Machine::Debugueur::JSON_Interne::ChaineSP valeur_chaine;
	SVM::Machine::Debugueur::JSON_Interne::BooleenSP valeur_booleen;
	SVM::Machine::Debugueur::JSON_Interne::ObjetSP valeur_objet;
	SVM::Machine::Debugueur::JSON_Interne::TableauSP valeur_tableau;
	SVM::Machine::Debugueur::JSON_Interne::ValeurSP valeur;
};

#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)



#line 73 "analyseur.syn.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _INVALIDE_ = 258,
    CHAINE = 259,
    ENTIER = 260,
    VRAI = 261,
    FAUX = 262,
    DEUX_POINTS = 263,
    VIRGULE = 264,
    ACCOLADE_OUVRANTE = 265,
    ACCOLADE_FERMANTE = 266,
    CROCHET_OUVRANT = 267,
    CROCHET_FERMANT = 268
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  struct ValeurJSON  YYSTYPE;
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



int machinedebugueuranalyseurparse (void *scanner, SVM::Machine::Debugueur::JSON_Interne::ValeurSP& valeur);

#endif /* !YY_MACHINEDEBUGUEURANALYSEUR_ANALYSEUR_SYN_HPP_INCLUDED  */
