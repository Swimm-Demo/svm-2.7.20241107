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
#include <src/machine/extensions/description.h>
#include <src/machine/extensions/automate_deterministe.h>

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(RegexParametresElement);

	DECL_SHARED_PTR(EtatNonDeterministe);
	struct EtatNonDeterministe
	{
		EtatNonDeterministe(const size_t indice)
		:_indice(indice) {}
		bool operator<(const EtatNonDeterministe& etat) const
		{
			return _indice<(etat._indice);
		}
		bool operator!=(const EtatNonDeterministe& etat) const
		{
			return _indice!=(etat._indice);
		}
		static EtatNonDeterministeSP nouvel_etat()
		{
			static size_t generateur_indices = 0;
			return std::make_shared<EtatNonDeterministe>(++generateur_indices);
		}
		struct InferieurDeterminise
		{
			bool operator() (const EtatNonDeterministeSP& a, const EtatNonDeterministeSP& b) const
			{
				if(a->_etats.size()!=b->_etats.size())
				{
					return a->_etats.size()<b->_etats.size();
				}
				for(auto ita=a->_etats.begin(),itb=b->_etats.begin() ; ita!=a->_etats.end() ; ++ita,++itb)
				{
					if(**ita!=**itb)
					{
						return **ita<**itb;
					}
				}
				return false;
			}
		};
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const EtatNonDeterministeSP& e)
		{
			f << e->_indice;
			if(not e->_etats.empty())
			{
				f << "[";
				for(auto& ee:e->_etats)
				{
					f << " " << ee;
				}
				f << " ]";
			}
			return f;
		}
		size_t _indice;
		std::set<EtatNonDeterministeSP, SVM::Global::SPinferieurSP<EtatNonDeterministe> > _etats;
	};

	struct AutomateNonDeterministe
	{
		AutomateNonDeterministe()
		{ }
		AutomateNonDeterministe(const RegexParametresElementCSP& t)
		{
			EtatNonDeterministeSP i = EtatNonDeterministe::nouvel_etat();
			EtatNonDeterministeSP f = EtatNonDeterministe::nouvel_etat();
			ajout_initial(i);
			ajout_final(f);
			ajout_transition(i,t,f);
		}
		void ajout_initial(const EtatNonDeterministeSP& etat)
		{
			_initial.insert(etat);
		}
		void ajout_final(const EtatNonDeterministeSP& etat)
		{
			_final.insert(etat);
		}
		void ajout_vide()
		{
			auto e = EtatNonDeterministe::nouvel_etat();
			_initial.insert(e);
			_final.insert(e);
		}
		void ajout_final_depuis_initial()
		{
			_final.insert(_initial.begin(),_initial.end());
		}
		void ajout_boucle_instantanee()
		{
			for(auto& eav:_final)
			{
				for(auto& ear:_initial)
				{
					ajout_transition_instantanee(eav,ear);
				}
			}
		}
		void ajout_transition(const EtatNonDeterministeSP& depart, const RegexParametresElementCSP& lettre, const EtatNonDeterministeSP& arrivee)
		{
			auto it = _transitions.find(depart);
			if(it==_transitions.end())
			{
				it = _transitions.insert(std::make_pair(depart,decltype(_transitions)::mapped_type())).first;
			}
			auto itit = it->second.find(lettre);
			if(itit==it->second.end())
			{
				itit = it->second.insert(std::make_pair(lettre,decltype(it->second)::mapped_type())).first;
			}
			itit->second.insert(arrivee);
		}
		void ajout_transition_instantanee(const EtatNonDeterministeSP& depart, const EtatNonDeterministeSP& arrivee)
		{
			_transitions_instantanees.insert(std::make_pair(depart,arrivee));
		}
		void ajout_automate_serie(const AutomateNonDeterministe& a)
		{
			if(_initial.empty())
			{
				_initial = a._initial;
			}
			_transitions.insert(a._transitions.begin(),a._transitions.end());
			_transitions_instantanees.insert(a._transitions_instantanees.begin(),a._transitions_instantanees.end());
			for(auto& eav:_final)
			{
				for(auto& ear:a._initial)
				{
					ajout_transition_instantanee(eav,ear);
				}
			}
			_final = a._final;
		}
		void ajout_automate_parallele(const AutomateNonDeterministe& a)
		{
			_initial.insert(a._initial.begin(),a._initial.end());
			_final.insert(a._final.begin(),a._final.end());
			_transitions.insert(a._transitions.begin(),a._transitions.end());
			_transitions_instantanees.insert(a._transitions_instantanees.begin(),a._transitions_instantanees.end());
		}
		AutomateDeterministeSP determinise(const std::string& regex)
		{
			elimine_transitions_instantanees();
			std::set<EtatNonDeterministeSP,EtatNonDeterministe::InferieurDeterminise> etats;
			decltype(_initial) i;
			decltype(_final) f;
			decltype(_transitions) t;

			EtatNonDeterministeSP initial = EtatNonDeterministe::nouvel_etat();
			initial->_etats=_initial;
			i.insert(initial);

			etats.insert(initial);
			std::list<EtatNonDeterministeSP> a_traiter = { initial };

			while(not a_traiter.empty())
			{
				EtatNonDeterministeSP etat = a_traiter.back();
				a_traiter.pop_back();
				decltype(t)::mapped_type associations;
				for(auto& ee:etat->_etats)
				{
					//std::cerr << "Etat traite : " << ee << std::endl;
					auto it = _transitions.find(ee);
					if(it==_transitions.end())
						continue;
					for(auto& tt:it->second)
					{
						//std::cerr << "Lettre consideree : " << *tt.first << std::endl;
						SVM_Extension::RegexParametresElementSP lc;
						for(lc = tt.first->copie() ; static_cast<bool>(lc) ; lc=lc->joker())
						{
							//std::cerr << "Lettre testee : " << *lc << std::endl;
							for(auto& eeee:etat->_etats)
							{
								//std::cerr << "Sous etat recherche : " << eeee << std::endl;
								auto itee = _transitions.find(eeee);
								if(itee == _transitions.end())
									continue;
								for(auto& ttee:itee->second)
								{
									//std::cerr << "Sous lettre recherche : " << *ttee.first << std::endl;
									//std::cerr << ee << "/" << eeee << " -> " << *lc << "/" << *ttee.first << std::endl;
									if(*ttee.first == *lc)
									{
										auto itit = associations.find(tt.first);
										if(itit==associations.end())
										{
											//std::cerr << "Ajout nouvelle association : " << *tt.first << std::endl;
											itit=associations.insert(std::make_pair(tt.first,decltype(associations)::mapped_type())).first;
										}
										itit->second.insert(ttee.second.begin(),ttee.second.end());
										// std::cerr << "Ajout " << itee->first << " -- " << *tt.first << "(" << *lc << ") -->"; for(auto& ss:ttee.second) { std::cerr << " " << ss; } std::cerr << " / " ; for(auto& aa:associations) { std::cerr << " " << *aa.first << " ->"; for(auto& aaaa:aa.second) { std::cerr << " " << aaaa;} } std::cerr << std::endl;
									}
									if(not tt.first->_modifiable)
										continue;
									lc->_modifiable = false;
									//std::cerr << ee << "/" << eeee << " -> " << *lc << "/" << *ttee.first << std::endl;
									if(*ttee.first == *lc)
									{
										auto itit = associations.find(tt.first);
										if(itit==associations.end())
										{
											//std::cerr << "Ajout nouvelle association : " << *tt.first << std::endl;
											itit=associations.insert(std::make_pair(tt.first,decltype(associations)::mapped_type())).first;
										}
										itit->second.insert(ttee.second.begin(),ttee.second.end());
										// std::cerr << "Ajout " << itee->first << " -- " << *tt.first << "(" << *lc << ") -->"; for(auto& ss:ttee.second) { std::cerr << " " << ss; } std::cerr << " / " ; for(auto& aa:associations) { std::cerr << " " << *aa.first << " ->"; for(auto& aaaa:aa.second) { std::cerr << " " << aaaa;} } std::cerr << std::endl;
									}
									lc->_modifiable = true;
								}
							}
						}
					}
				}
				auto it2 = t.insert(std::make_pair(etat,decltype(associations)())).first;
				for(auto &a:associations)
				{
					EtatNonDeterministeSP arrivee = EtatNonDeterministe::nouvel_etat();
					arrivee->_etats = a.second;
					auto itit = it2->second.insert(std::make_pair(a.first,decltype(associations)::mapped_type())).first;
					auto etat_a_ajouter = etats.insert(arrivee);
					itit->second.insert(*etat_a_ajouter.first);
					// std::cerr << "Nouvelle transition AD : " << etat << " -- " << *a.first << " --> " << arrivee << std::endl;
					if(etat_a_ajouter.second)
					{
						a_traiter.push_back(*etat_a_ajouter.first);
					}
				}
			}
			std::map<EtatNonDeterministeSP,size_t,EtatNonDeterministe::InferieurDeterminise> correspondance_etats;
			size_t indice=0;
			for(auto& e:etats)
			{
				for(auto& ee:e->_etats)
				{
					if(_final.find(ee)!=_final.end())
					{
						f.insert(e);
					}
				}
				correspondance_etats.insert(std::make_pair(e,indice++));
			}
			AutomateDeterministeSP automate = std::make_shared<AutomateDeterministe>(regex);
			ASSERT(i.size()==1,"automate deterministe avec plus d'un etat initial");
			automate->_initial = correspondance_etats[*i.begin()];
			for(auto& ff:f)
			{
				automate->_final.insert(correspondance_etats[ff]);
			}
			automate->_transitions.resize(indice);
			for(auto& tt:t)
			{
				decltype(automate->_transitions)::value_type association;
				for(auto& ttt:tt.second)
				{
					if(ttt.second.size()!=1)
					{
						AutomateNonDeterministe a;
						a._initial = i;
						a._final = f;
						a._transitions = t;
						//std::cerr << a;
					}
					ASSERT(ttt.second.size()==1,"automate deterministe avec plus d'un etat apres une transition");
					association.insert(std::make_pair(ttt.first,correspondance_etats[*ttt.second.begin()]));
				}
				automate->_transitions[correspondance_etats[tt.first]] = association;
			}
			//std::cerr << "{{{" << std::endl << *this << std::endl << static_cast<std::string>(*automate) << "}}}" << std::endl;
			return automate;
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const AutomateNonDeterministe& a)
		{
			f << "AutomateNonDeterministe:" << std::endl;
			f << "Initiaux :";
			for(auto& i:a._initial)
			{
				f << " " << (*i);
			}
			f << std::endl;
			f << "Finaux :";
			for(auto& i:a._final)
			{
				f << " " << (*i);
			}
			f << std::endl;
			f << "Transitions:" << std::endl;
			for(auto &t:a._transitions)
			{
				for(auto& tt:t.second)
				{
					for(auto& ttt:tt.second)
					{
						f << t.first << " === " << *tt.first << " ===> " << ttt << std::endl;
					}
				}
			}
			f << "Transitions instantanees :" << std::endl;
			for(auto& t:a._transitions_instantanees)
			{
				f << t.first << " ===> " << t.second << std::endl;
			}
			f << "========================" << std::endl;
			return f;
		}
		private:
		std::set<EtatNonDeterministeSP,SVM::Global::SPinferieurSP<EtatNonDeterministe> > etats_accessibles(const std::set<EtatNonDeterministeSP,SVM::Global::SPinferieurSP<EtatNonDeterministe> >& etats)
		{
			auto dest = etats;
			decltype(dest) temp;
			do
			{
				temp.clear();
				for(auto& dtt:dest)
				{
					auto itdest=_transitions_instantanees.equal_range(dtt);
					temp.insert(dtt);
					for(auto it=itdest.first ; it!=itdest.second ; ++it)
					{
						temp.insert(it->second);
					}
				}
				dest.swap(temp);
			}
			while(temp.size()!=dest.size());
			return dest;
		}
		void elimine_transitions_instantanees()
		{
			decltype(_transitions) t;
			decltype(_initial) i = etats_accessibles(_initial);
			for(auto& d:_transitions)
			{
				for(auto& dt:d.second)
				{
					auto dest = etats_accessibles(dt.second);
					auto it = t.insert(std::make_pair(d.first,decltype(t)::mapped_type())).first;
					auto itit = it->second.insert(std::make_pair(dt.first,decltype(it->second)::mapped_type())).first;
					itit->second.swap(dest);
				}
			}
			_initial.swap(i);
			_transitions.swap(t);
			_transitions_instantanees.clear();
		}
		std::set<EtatNonDeterministeSP, SVM::Global::SPinferieurSP<EtatNonDeterministe> > _initial;
		std::set<EtatNonDeterministeSP, SVM::Global::SPinferieurSP<EtatNonDeterministe> > _final;
		std::map<EtatNonDeterministeSP, std::map<RegexParametresElementCSP,std::set<EtatNonDeterministeSP, SVM::Global::SPinferieurSP<EtatNonDeterministe> >, SVM::Global::SPinferieurSP<RegexParametresElement> > , SVM::Global::SPinferieurSP<EtatNonDeterministe> > _transitions;
		std::multimap<EtatNonDeterministeSP, EtatNonDeterministeSP , SVM::Global::SPinferieurSP<EtatNonDeterministe> > _transitions_instantanees;

	};
}
}
}
