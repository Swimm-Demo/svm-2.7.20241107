/*
 * Simple Virtual Machine - A versatile and robust architecture to
 * easily write applications.
 * Copyright (C) 2021  Julien BRUGUIER
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <src/demarrage/lanceur/lanceur.h>
#include <src/demarrage/lanceur/analyseur/analyseur.syn.hpp>

/*

#!/usr/bin/svm
DESCRIPTION
bla
END
DEBUG "debug"
COREDUMP "core"
PLUGIN "libsvmplugin.so"
PLUGIN "libsvmhelper.so"
PLUGIN "libsvmrun.so"
OPTION -t FLAG test HELP "enable test"
OPTION -v COUNT verbose HELP "verbose"
OPTION -i STR input HELP "input file"
OPTION -l INT level HELP "algo level"
ARGUMENT STR seed HELP "seed"
ARGUMENT INT iterations HELP "iterations"
ARGUMENTS arguments
PROCESS "main"
	CODE "main" INLINE
	:new INT/i
	...
	END
	MEMORY test verbose seed iterations arguments
	SCHEDULER run.multithread
END
PROCESS "aside"
	CODE "toolbox" FILE "source.lib.svm"
	MEMORY test verbose input arguments
	SCHEDULER run.multithread
	SEQUENCER run.stack
END
*/
