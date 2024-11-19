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
	struct Systeme
	{
		template<typename T_ID>
		static void nom_thread(const T_ID& tid, const std::string& nom)
		{
// __LINUX__
// BSD
// __APPLE__
// _WIN32
#if defined(__APPLE__)
			::pthread_setname_np(nom.substr(0,14).c_str());
#else
			::pthread_setname_np(tid,nom.substr(0,14).c_str());
#endif
		}
		static void sigset(sigset_t *ss, int signal)
		{
#if defined(__APPLE__)
			sigemptyset(ss);
			sigaddset(ss,signal);
#else
			::sigemptyset(ss);
			::sigaddset(ss,signal);
#endif
		}
	};
}
}
