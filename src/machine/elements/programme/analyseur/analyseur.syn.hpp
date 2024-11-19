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
#line 51 "/home/pappy/Programmation/svm/projet/src/machine/elements/programme/analyseur/analyseur.syn.ypp" /* yacc.c:1909  */


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

#line 82 "analyseur.syn.hpp" /* yacc.c:1909  */

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
