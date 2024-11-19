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

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Synchronisation
{
	DECL_UNIQUE_PTR(Lecteur);
	DECL_UNIQUE_PTR(Ecrivain);

	DECL_SHARED_PTR(Acces);
	struct Acces : public std::enable_shared_from_this<Acces>
	{
		friend struct Lecteur;
		friend struct Ecrivain;
		explicit Acces(const bool synchronise = true)
		:_synchronise(synchronise), _lecteurs(0) {}
		LecteurUP lecteur();
		EcrivainUP ecrivain();
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Acces& acces)
		{
			f << "Lock " << &acces << " ";
			if(acces._synchronise)
			{
				f << "enabled (" << acces._lecteurs << " readers)";
			}
			else
			{
				f << "disabled";
			}
			return f;
		}
		private:
			bool _synchronise;
			std::mutex _protection;
			std::mutex _ecrivain;
			size_t _lecteurs;
	};

	struct Lecteur
	{
		friend struct Acces;
		~Lecteur()
		{
			if(not _acces->_synchronise) return;
			std::lock_guard<std::mutex> verrou(_acces->_protection);
			--_acces->_lecteurs;
			if(_acces->_lecteurs==0)
			{
				_acces->_ecrivain.unlock();
			}
		}
		friend std::ostream& operator<<(std::ostream& f, const LecteurUP& lecteur)
		{
			f << "Lock guard read [" << *lecteur->_acces << "]";
			return f;
		}
		private:
			Lecteur(const SHARED_PTR(Acces)& acces)
			:_acces(acces)
			{
				if(not _acces->_synchronise) return;
				std::lock_guard<std::mutex> verrou(_acces->_protection);
				++_acces->_lecteurs;
				if(_acces->_lecteurs==1)
				{
					_acces->_ecrivain.lock();
				}
			}
			SHARED_PTR(Acces) _acces;
	};

	struct Ecrivain
	{
		friend struct Acces;
		~Ecrivain()
		{
			if(not _acces->_synchronise) return;
			_acces->_ecrivain.unlock();
		}
		friend std::ostream& operator<<(std::ostream& f, const EcrivainUP& ecrivain)
		{
			f << "Lock guard write [" << *ecrivain->_acces << "]";
			return f;
		}
		private:
			Ecrivain(const SHARED_PTR(Acces)& acces)
			:_acces(acces)
			{
				if(not _acces->_synchronise) return;
				_acces->_ecrivain.lock();
			}
			SHARED_PTR(Acces) _acces;
	};
}
}
}
}
