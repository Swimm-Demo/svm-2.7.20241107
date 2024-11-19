" 
" Simple Virtual Machine - A versatile and robust architecture to
" easily write applications.
" Copyright (C) 2021  Julien BRUGUIER
" 
" This program is free software: you can redistribute it and/or modify
" it under the terms of the GNU General Public License as published by
" the Free Software Foundation, either version 3 of the License, or
" (at your option) any later version.
" 
" This program is distributed in the hope that it will be useful,
" but WITHOUT ANY WARRANTY; without even the implied warranty of
" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
" GNU General Public License for more details.
" 
" You should have received a copy of the GNU General Public License
" along with this program.  If not, see <http://www.gnu.org/licenses/>.
" 
syntax clear
syntax match Comment "#.*\n"
syntax match Special "[A-Z][A-Z_0-9]*"
syntax match Keyword "[&\.$!]"
syntax keyword Type INT
syntax keyword Type STR
syntax keyword Type BLN
syntax keyword Type PTR
syntax keyword Type PEP
syntax keyword Type LIB
syntax keyword Type SYM
syntax keyword Type IRQ
syntax keyword Type AUTO
syntax match Keyword ":[ \t]*[a-z0-9_]\+\([ \t]*\.[ \t]*[a-z0-9_]\+\)\?"
syntax match Comment ":debug[ \t].*\n"
syntax keyword Constant TRUE
syntax keyword Constant FALSE
syntax match Constant "\<-\?[0-9]\+\>"
syntax match Constant "\"\([^"\\]\|\\.\)*\"" contains=escape
syntax match escape "\\n" contained
syntax match escape "\\r" contained
syntax match escape "\\t" contained
syntax match escape "\\\\" contained
syntax match escape "\\\"" contained
syntax match escape "\\\0[0-9a-fA-F][0-9a-fA-F]" contained
hi link escape Special
syntax match Constant "\"\"\"\(\n\|[^"]\|\"[^"]\|\"\"[^"]\)*\"\"\""
syntax region Comment start="^[ \t]*DESCRIPTION" end="^[ \t]*END"
