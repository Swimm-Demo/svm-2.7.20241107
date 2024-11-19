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

#include <src/machine/elements/valeur/code.h>
#include <src/machine/elements/valeur/instruction.h>
#include <src/machine/elements/processeur/instructions/affectation.h>
#include <src/machine/elements/processeur/instructions/saut.h>
#include <src/machine/elements/processeur/instructions/appel.h>
#include <src/machine/elements/processeur/instructions/retour.h>
#include <src/machine/elements/processeur/instructions/arret.h>
#include <src/machine/elements/processeur/instructions/local.h>
#include <src/machine/elements/processeur/instructions/decalage.h>
#include <src/machine/elements/processeur/instructions/bibliotheque.h>
#include <src/machine/elements/processeur/instructions/drapeau.h>
#include <src/machine/elements/processeur/instructions/interruption.h>
#include <src/machine/elements/processeur/instructions/memoire.h>
#include <src/machine/elements/processeur/instructions/debogage.h>
#include <src/machine/elements/processeur/instructions/extension.h>
#include <src/machine/elements/processeur/instructions/conditions.h>
#include <src/machine/elements/programme/analyseur/analyseur.syn.hpp>
