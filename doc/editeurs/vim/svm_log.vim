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

runtime syntax/svm.vim

syntax region trace start="^### Simple Virtual Machine" end="^$" contains=header
syntax match header "^### Simple Virtual Machine.*" contained

syntax region coredump start="^Core dump:$" end="^$" contains=header

syntax region flat start="^Processor:$" end="^Code at.*:$" contains=processor,code,subsection,subsubsection keepend
syntax region flat start="^ Labels:$" end="^\(Code at.*\|Memory\):$" contains=code,memory,subsection keepend
syntax region flat start="^ Aliases:$" end="^$" contains=subsection keepend

syntax match processor "^Processor:$" contained
syntax match code "^Code at.*:$" contained
syntax match memory "^Memory:$" contained
syntax match subsection "^ [^ ].*:$" contained
syntax match subsubsection "^  [^ ].*:$" contained

syntax match here "^====== HERE ======$"
syntax match here "^==================$"
hi link header Todo
hi link here Error
hi link coredump Keyword
hi link processor Function
hi link code Function
hi link memory Function
hi link subsection Constant
hi link subsubsection Type

set noautoindent
