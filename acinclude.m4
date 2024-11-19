# 
# Simple Virtual Machine - A versatile and robust architecture to
# easily write applications.
# Copyright (C) 2021  Julien BRUGUIER
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

#special variable definition
#$1 is the variable name
#$2 is the default value
#$3 is the help string (not used here, because I don't know to do it)
AC_DEFUN([AC_VAR_DEFINITION],[
AC_MSG_CHECKING([$1 variable definition])
if [[[ x$$1 == x ]]] ; then
 $1=$2
fi
AC_SUBST($1)
AC_MSG_RESULT([$$1])
])

#Try to build a program from a file with a C compiler
#$1 is the source file
#$2 is the header file
#$3 is the objet file
#$4 are the options
#$5 is the code to execute if the build failed
#$6 is the code to execute if the build worked
AC_DEFUN([AC_TRY_BUILD_FILE], [
AC_REQUIRE([AC_PROG_CC])
COMPILE=0
$CC $4 $1 -o $3 2> /dev/null > /dev/null && COMPILE=1
if ! test -f $3 ; then
  rm $1 $2 2> /dev/null > /dev/null
  $5
else
  if [[[ $COMPILE == 0 ]]] ; then
    rm $1 $2 $3 2> /dev/null > /dev/null
    $5
  else
    rm $1 $2 $3 2> /dev/null > /dev/null
    $6
  fi
fi
])

#Try to build a program from a file with a C++ compiler
#$1 is the source file
#$2 is the header file
#$3 is the objet file
#$4 are the options
#$5 is the code to execute if the build failed
#$6 is the code to execute if the build worked
AC_DEFUN([AC_TRY_BUILD_FILE_CXX], [
AC_REQUIRE([AC_PROG_CXX])
COMPILE=$($CXX $4 $1 -o $3 2>&1)
if test ${#COMPILE} -gt 2  ; then
	rm $3 2> /dev/null > /dev/null
fi
if ! test -f $3 ; then
  rm $1 $2 2> /dev/null > /dev/null
  $5
else
  rm $1 $2 $3 2> /dev/null > /dev/null
  if [[[ $COMPILE == 0 ]]] ; then
    $5
  else
    $6
  fi
fi
])

#versions of programs

define(FLEXVERSION,2.5.31)
define(BISONVERSION,1.875d)

#check whether flex is able to build the lexical parser

AC_DEFUN([AM_PROG_LEX_ABLE],[
AC_REQUIRE([AC_PROG_LEX])
AC_REQUIRE([AC_PARSER_STRICT_CHECK])
AC_MSG_CHECKING([whether $LEX is able to build lexical parser])
FLEXTESTFILEFLEX=flextestfile.lex
FLEXTESTFILEC=lex.yy.c
FLEXTESTFILE=flextest$ac_exeext
cat > $FLEXTESTFILEFLEX << EOF
%{
#include <stdlib.h>
void test(void)
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
	yy_switch_to_buffer(YY_CURRENT_BUFFER);
	yy_scan_string("");
}
%}
%option yy_scan_string
%option nounput
%s START
%%
%%

int yywrap(void)
{
	return 1;
}

int main()
{
	yyin=stdin;
	yylex();
	yylex_destroy();
	return 0;
}
EOF
$LEX $FLEXTESTFILEFLEX 2> /dev/null
if ! test -f $FLEXTESTFILEC  ; then
  rm $FLEXTESTFILEFLEX 2> /dev/null > /dev/null
  AC_MSG_RESULT([no])
  if [[[ $PARSERSTRICTCHECK = 1 ]]] ; then
    AC_MSG_ERROR([You need to upgrade $LEX to version FLEXVERSION or later!])
  else
    AC_MSG_WARN([Compilation may fail if you change $LEX files !])
  fi
else
  rm $FLEXTESTFILEFLEX 2> /dev/null > /dev/null
  AC_TRY_BUILD_FILE($FLEXTESTFILEC , , $FLEXTESTFILE , -Wall , [
  AC_MSG_RESULT([no])
  if [[[ $PARSERSTRICTCHECK = 1 ]]] ; then
    AC_MSG_ERROR([You need to upgrade $LEX to version FLEXVERSION or later!])
  else
    AC_MSG_WARN([Compilation may fail if you change $LEX files !])
  fi
  ],[
  AC_MSG_RESULT([yes])
  ])
fi
])

#check whether bison is able to build the syntax parser

AC_DEFUN([AC_PROG_YACC_ABLE],[
AC_REQUIRE([AC_PROG_YACC])
AC_REQUIRE([AC_PARSER_STRICT_CHECK])
AC_MSG_CHECKING([whether $YACC is able to build syntax parser])
BISONTESTFILEBISON=bisontestfile.y
BISONTESTFILEPREFIX=bisontestfile
BISONTESTFILEC=bisontestfile.tab.c
BISONTESTFILEH=bisontestfile.tab.h
BISONTESTFILE=bisontest$ac_exeext
cat > $BISONTESTFILEBISON << EOF
%{
#include <stdlib.h>
#include "$BISONTESTFILEH"

void yyerror();
int yylex();

#define YYDEBUG	1
int yydebug=0;
int resultat;
%}
%union {
	int entier;
	char caractere;
}

%token NOMBRE

%type<entier> nombre

%start arf

%parse-param {char **chaine}

%%
EOF
echo -n 'arf: nombre { resultat=$'>> $BISONTESTFILEBISON
echo '1; };' >> $BISONTESTFILEBISON
echo 'nombre: NOMBRE { $$=0; };' >> $BISONTESTFILEBISON
cat >> $BISONTESTFILEBISON << EOF
%%
void yyerror()
{
	exit(1);
}

int yylex()
{
	return 0;
}

int main(void)
{
	char *chaine;
	yyparse(&chaine);
	return 0;
}
EOF
$YACC -d --file-prefix=$BISONTESTFILEPREFIX $BISONTESTFILEBISON 2> /dev/null
if ! test -f $BISONTESTFILEC ; then
  rm $BISONTESTFILEBISON 2> /dev/null > /dev/null
  AC_MSG_RESULT([no])
  if [[[ $PARSERSTRICTCHECK = 1 ]]] ; then
    AC_MSG_ERROR([You need to upgrade $YACC to version BISONVERSION or later !])
  else
    AC_MSG_WARN([Compilation may fail if you change $YACC files !])
  fi
else
  if ! test -f $BISONTESTFILEH ; then
    rm $BISONTESTFILEBISON 2> /dev/null > /dev/null
    AC_MSG_RESULT([no])
    if [[[ $PARSERSTRICTCHECK = 1 ]]] ; then
      AC_MSG_ERROR([You need to upgrade $YACC to version BISONVERSION or later !])
    else
      AC_MSG_WARN([Compilation may fail if you change $YACC files !])
    fi
  else
    rm $BISONTESTFILEBISON 2> /dev/null > /dev/null
    AC_TRY_BUILD_FILE($BISONTESTFILEC , $BISONTESTFILEH , $BISONTESTFILE , -Wall , [
    AC_MSG_RESULT([no])
    if [[[ $PARSERSTRICTCHECK = 1 ]]] ; then
      AC_MSG_ERROR([You need to upgrade $YACC to version BISONVERSION or later !])
    else
      AC_MSG_WARN([Compilation may fail if you change $YACC files !])
    fi
    ],[
    AC_MSG_RESULT([yes])
    ])
  fi
fi
])


#
#check if C compiler accepts these options
#$1 options to check
#
AC_DEFUN([AC_CFLAGS_GCC_OPTION],[
AC_REQUIRE([AC_PROG_CC])
AC_MSG_CHECKING([whether $CC accepts $1])
CTESTFILEC=testfilec.c
CTESTFILE=testfilec$ac_exeext
cat > $CTESTFILEC << EOF
int main(int argc , char * const *argv)
{
	return 0;
}
EOF
AC_TRY_BUILD_FILE($CTESTFILEC ,  , $CTESTFILE , $1 , [
AC_MSG_RESULT([no])
dnl AC_MSG_ERROR([Your compiler does not accept $1. You loose...])
],[
CFLAGS="$CFLAGS $1"
AC_SUBST(CFLAGS)
AC_MSG_RESULT([yes])
])
])

#
#check if C++ compiler accepts these options
#$1 options to check
#
AC_DEFUN([AC_CXXFLAGS_GXX_OPTION_MANDATORY],[
AC_REQUIRE([AC_PROG_CXX])
AC_MSG_CHECKING([whether $CXX accepts $1])
CTESTFILECXX=testfilec.cpp
CTESTFILE=testfilec$ac_exeext
cat > $CTESTFILECXX << EOF
int main(int argc , char * const *argv)
{
	return 0;
}
EOF
AC_TRY_BUILD_FILE_CXX($CTESTFILECXX ,  , $CTESTFILE , $1 , [
AC_MSG_RESULT([no])
AC_MSG_ERROR([Your compiler does not accept $1. You loose...])
],[
AM_CXXFLAGS="$AM_CXXFLAGS $1"
AC_SUBST(AM_CXXFLAGS)
AC_MSG_RESULT([yes])
])
])

#
#check if C++ compiler accepts these options
#$1 options to check
#
AC_DEFUN([AC_CXXFLAGS_GXX_OPTION_OPTIONAL],[
AC_REQUIRE([AC_PROG_CXX])
AC_MSG_CHECKING([whether $CXX accepts $1])
CTESTFILECXX=testfilec.cpp
CTESTFILE=testfilec$ac_exeext
cat > $CTESTFILECXX << EOF
int main(int argc , char * const *argv)
{
	return 0;
}
EOF
AC_TRY_BUILD_FILE_CXX($CTESTFILECXX ,  , $CTESTFILE , $1 , [
AC_MSG_RESULT([no])
dnl AC_MSG_ERROR([Your compiler does not accept $1. You loose...])
],[
AM_CXXFLAGS="$AM_CXXFLAGS $1"
AC_SUBST(AM_CXXFLAGS)
AC_MSG_RESULT([yes])
])
])

#
# Support of the parser build feature
#
AC_DEFUN([AC_PARSER_STRICT_CHECK],[
AC_MSG_CHECKING([if we want to be able to rebuild the parser])
AC_ARG_ENABLE(parser-build,
[  --disable-parser-build  disable strict check of parser tools],[
if eval "test x$enable_parser_build = xno" ; then
PARSERSTRICTCHECK=0
AC_MSG_RESULT([no])
else
PARSERSTRICTCHECK=1
AC_MSG_RESULT([yes])
fi
],[
PARSERSTRICTCHECK=1
AC_MSG_RESULT([yes])
])
])

