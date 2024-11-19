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

syntax match SectionHeader "^[^:]*:" contains=SectionKey,SectionOption

syntax keyword SectionKey attach contained
syntax keyword SectionKey author contained
syntax keyword SectionKey changelog contained
syntax keyword SectionKey checks contained
syntax keyword SectionKey code contained
syntax keyword SectionKey compile contained
syntax keyword SectionKey constant contained
syntax keyword SectionKey copy contained
syntax keyword SectionKey compare contained
syntax keyword SectionKey create contained
syntax keyword SectionKey current contained
syntax keyword SectionKey date contained
syntax keyword SectionKey delete contained
syntax keyword SectionKey description contained
syntax keyword SectionKey detach contained
syntax keyword SectionKey example contained
syntax keyword SectionKey finalisation contained
syntax keyword SectionKey help contained
syntax keyword SectionKey includes contained
syntax keyword SectionKey initialisation contained
syntax keyword SectionKey lang contained
syntax keyword SectionKey license contained
syntax keyword SectionKey link contained
syntax keyword SectionKey maintainer contained
syntax keyword SectionKey news contained
syntax keyword SectionKey notification contained
syntax keyword SectionKey patch contained
syntax keyword SectionKey print contained
syntax keyword SectionKey readme contained
syntax keyword SectionKey schedule contained
syntax keyword SectionKey seealso contained
syntax keyword SectionKey shell contained
syntax keyword SectionKey startup contained
syntax keyword SectionKey synopsis contained
syntax keyword SectionKey test contained
syntax keyword SectionKey title contained
syntax keyword SectionKey usage contained
syntax keyword SectionKey version contained

syntax keyword SectionOption default contained
syntax keyword SectionOption object contained
syntax keyword SectionOption string contained

syntax match SectionHeaderIdentifier "^\(comment\|file\)[^:]*:" contains=SectionKeyIdentifier
syntax keyword SectionKeyIdentifier comment contained
syntax keyword SectionKeyIdentifier file contained

syntax keyword PluginName PLUGIN

syntax keyword PluginPart USE
syntax keyword PluginPart DEFINE

syntax match InstructionHeader "^.*INSTRUCTION" contains=InstructionOption,PluginObject

syntax keyword PluginObject TYPE
syntax keyword PluginObject STRUCT
syntax keyword PluginObject INSTRUCTION contained
syntax keyword PluginObject FUNCTION
syntax keyword PluginObject INTERRUPTION
syntax keyword PluginObject SEQUENCER
syntax keyword PluginObject SCHEDULER
syntax keyword PluginObject OPTION
syntax keyword PluginObject ARGUMENT
syntax keyword PluginObject ARGUMENTS

syntax keyword InstructionOption SYSTEM contained
syntax keyword InstructionOption WAITING contained
syntax keyword InstructionOption OVERRIDE contained

syntax region SectionBody start="%{\n" end="\n%}" contains=SectionBodyEdge,SectionVariable,SectionMacro,SVMType,SVMFunction,SVMConstant keepend transparent

syntax match SectionBodyEdge "%{\n" contained
syntax match SectionBodyEdge "\n%}" contained

syntax match SectionBodyEdge "%{ \?%\?}"
syntax match SectionBodyEdge "%{\n%}"

syntax region SectionBody start="${\n" end="\n$}" contains=SectionBodyEdge,SectionVariable,SectionMacro,SVMType,SVMFunction,SVMConstant keepend transparent

syntax match SectionBodyEdge "${\n" contained
syntax match SectionBodyEdge "\n$}" contained

syntax match SectionBodyEdge "${ \?$\?}"
syntax match SectionBodyEdge "${\n$}"

syntax region SectionBody start=":{\n" end="\n:}" contains=SectionBodyEdge,SectionVariable,SectionMacro,SVMType,SVMFunction,SVMConstant keepend transparent

syntax match SectionBodyEdge ":{\n" contained
syntax match SectionBodyEdge "\n:}" contained

syntax match SectionBodyEdge ":{ \?:\?}"
syntax match SectionBodyEdge ":{\n:}"

syntax keyword SectionVariable svm contained
syntax keyword SectionVariable argc contained
syntax keyword SectionVariable argv contained
syntax keyword SectionVariable handler contained
syntax keyword SectionVariable object contained
syntax keyword SectionVariable handler_left contained
syntax keyword SectionVariable object_left contained
syntax keyword SectionVariable handler_right contained
syntax keyword SectionVariable object_right contained
syntax keyword SectionVariable process contained
syntax keyword SectionVariable kernel contained
syntax keyword SectionVariable state contained
syntax keyword SectionVariable parameter contained
syntax keyword SectionVariable value contained
syntax keyword SectionVariable string contained
syntax keyword SectionVariable type contained
syntax keyword SectionVariable copy contained

syntax keyword SectionMacro ARGV_VALUE contained
syntax keyword SectionMacro ARGV_PLUGIN contained
syntax keyword SectionMacro ARGV_MARKER contained
syntax keyword SectionMacro ARGV_KEYWORD contained
syntax keyword SectionMacro ARGV_STRUCT contained
syntax keyword SectionMacro ARGV_VARIABLE contained
syntax keyword SectionMacro NEW_VALUE contained
syntax keyword SectionMacro NEW_PLUGIN contained
syntax keyword SectionMacro NEW_STRUCT contained
syntax keyword SectionMacro NEW_STRING contained
syntax keyword SectionMacro RAW_STRING contained
syntax keyword SectionMacro NEW_NULL_VALUE contained
syntax keyword SectionMacro NEW_NULL_PLUGIN contained
syntax keyword SectionMacro NEW_NULL_STRUCT contained
syntax keyword SectionMacro ERROR_INTERNAL contained
syntax keyword SectionMacro ERROR_EXTERNAL contained
syntax keyword SectionMacro CONST_PEP contained
syntax keyword SectionMacro CURRENT contained
syntax keyword SectionMacro RETURN contained
syntax keyword SectionMacro VARIABLE_GLOBAL contained
syntax keyword SectionMacro VARIABLE_LOCAL contained
syntax keyword SectionMacro VARIABLE_DELETE contained

syntax region SectionBodyShort start=+"+ end=+"+ skip=+\\"+ keepend

syntax match Comment "#.*"

highlight link SectionKey Type
highlight link SectionKeyIdentifier Type
highlight link SectionOption Function
highlight link PluginName Keyword
highlight link PluginObject Keyword
highlight link PluginPart Todo
highlight link InstructionOption Special
highlight link SectionBodyEdge Constant
highlight link SectionBodyShort Constant
highlight link SectionVariable Tag
highlight link SectionMacro Tag
highlight link SVMType Type
highlight link SVMFunction Function
highlight link SVMConstant Constant
