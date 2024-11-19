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

#pragma once

#include <src/global/global.h>
#ifdef HAVE_PTHREAD_NP_H
#include <pthread_np.h>
#endif

namespace SVM
{
namespace Global
{
	struct ClefInvalide
	{
	};

	struct Installation
	{
		static std::string valeur(const std::string& clef)
		{
			auto v=Installation::variables();
			auto it = v.find(clef);
			if(it==v.end())
			{
				throw ClefInvalide();
			}
			return it->second;
		}
		static std::set<std::string> clefs()
		{
			std::set<std::string> r;
			for(const auto &c : Installation::variables())
			{
				r.insert(r.end(),c.first);
			}
			return r;
		}
		static const std::map<std::string,std::string>& variables()
		{
#define CHAINE(v) #v
#define CHAINE_MACRO(v) CHAINE(v)
			static const std::map<std::string, std::string> v = {
				{ "name", PACKAGE_NAME },
				{ "version", PACKAGE_VERSION },
				{ "debug_site", DEBUGGER_SITE },
				{ "default_style", DEFAULT_STYLE },
				{ "bugreport", PACKAGE_BUGREPORT },
				{ "reactive_signal", CHAINE_MACRO(REACTIVE_SIGNAL) },
				{ "bindir", REP_BINAIRE },
				{ "binext", EXT_BINAIRE },
				{ "includedir", REP_INCLUDE },
				{ "libdir", REP_INTERFACE },
				{ "prefix", REP_INSTALL },
				{ "eprefix", REP_EXECINSTALL },
				{ "datadir", REP_DONNEES },
				{ "mandir", REP_DOCUMENTATION },
				{ "machine_plugins", MACHINE_PLUGINS },
				{ "system_plugins", SYSTEM_PLUGINS }
			};
			return v;
		};
	};
}
}
