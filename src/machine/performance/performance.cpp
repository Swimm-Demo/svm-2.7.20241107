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

#include <src/machine/performance/performance.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/processus/processus.h>
#include <src/machine/debugueur/json.h>

using namespace SVM::Machine::Performance;

PerformanceSP Performance::creation(const SVM::Machine::PerfSP& perf, const SVM::Machine::HistoriqueSP& historique)
{
	if(not perf)
	{
		return PerformanceSP();
	}
	bool b = static_cast<bool>(perf->_historique);
	auto h = b?perf->_historique:historique;
	if(perf->_brut)
	{
		return std::make_shared<PerformanceBrute>(h,b);
	}
	else
	{
		return std::make_shared<PerformanceRapport>(h,perf->_details,b);
	}
}

void PerformanceRapport::evenement(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const bool remplacee, const std::chrono::time_point<std::chrono::steady_clock>& debut, const std::chrono::time_point<std::chrono::steady_clock>& fin, const SVM::Machine::Element::Noyau::NoyauSP& noyau, const size_t taille_pile)
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto duree = std::chrono::duration_cast<std::chrono::microseconds>(fin-debut);
	auto it = _statistiques.find(adresse);
	if(it==_statistiques.end())
	{
		Statistiques statistiques;
		if(remplacee)
		{
			statistiques._occurences_remplacee = 1;
		}
		statistiques._attente = adresse.instruction()->attente();
		statistiques._minimum = duree;
		statistiques._maximum = duree;
		statistiques._cumule = duree;
		_statistiques.insert(std::make_pair(adresse,statistiques));
		return;
	}
	++it->second._occurences;
	if(remplacee)
	{
		++it->second._occurences_remplacee;
	}
	if(duree<it->second._minimum)
	{
		it->second._minimum=duree;
	}
	if(duree>it->second._maximum)
	{
		it->second._maximum=duree;
	}
	it->second._cumule += duree;
}

void PerformanceBrute::evenement(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const bool remplacee, const std::chrono::time_point<std::chrono::steady_clock>& debut, const std::chrono::time_point<std::chrono::steady_clock>& fin, const SVM::Machine::Element::Noyau::NoyauSP& noyau, const size_t taille_pile)
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto it_adresse = _identifiants_adresse.insert(std::make_pair(adresse,_identifiants_adresse.size()+1));
	auto id_adresse = it_adresse.first->second;
	if(it_adresse.second)
	{
		creation_adresse(adresse,id_adresse);
	}
	auto id_noyau = noyau->_identifiant;
	auto processus = noyau->_processus.lock();
	auto id_processus = processus->_identifiant;

	auto debut_us = std::chrono::duration_cast<std::chrono::microseconds>(debut-_reference).count();
	auto temps = std::chrono::duration_cast<std::chrono::microseconds>(fin-debut).count();
	
	//SVM::Machine::Trace(_historique,"Performance event",_brut) << SVM::Machine::Debugueur::JSON::objet({ { "type" , SVM::Machine::Debugueur::JSON::chaine("run") } , { "id" , SVM::Machine::Debugueur::JSON::entier(id_adresse) } , { "location" , SVM::Machine::Debugueur::JSON::objet({ {"svm" , SVM::Machine::Debugueur::JSON::entier(_identifiant_machine) } , { "kernel" , SVM::Machine::Debugueur::JSON::entier(id_noyau) } , { "process" , SVM::Machine::Debugueur::JSON::entier(id_processus) } , { "stack" , SVM::Machine::Debugueur::JSON::entier(taille_pile) } }) } , { "time" , SVM::Machine::Debugueur::JSON::objet({ {"duration", SVM::Machine::Debugueur::JSON::entier(temps) } , { "unit" , SVM::Machine::Debugueur::JSON::chaine("us") } , { "at" , SVM::Machine::Debugueur::JSON::entier(debut_us) } }) } , { "override" , SVM::Machine::Debugueur::JSON::booleen(remplacee) } });
	// Optimisation
	SVM::Machine::Trace(_historique,"Performance event",_brut) << "{ \"id\": " << id_adresse << ", \"location\": { \"kernel\": " << id_noyau << ", \"process\": " << id_processus << ", \"stack\": " << taille_pile << ", \"svm\": " << _identifiant_machine << " }, \"override\": " << (remplacee?"true":"false") << ", \"time\": { \"at\": " << debut_us << ", \"duration\": " << temps << ", \"unit\": \"us\" }, \"type\": \"run\" }";
}

void PerformanceRapport::rapport()
{
	unsigned long long int occurences_totales = 0;
	unsigned long long int occurences_attente_totales = 0;
	std::chrono::microseconds temps_total(0);
	std::chrono::microseconds temps_attente_total(0);
	std::list<Calculs> calculs;
	for(const auto& s: _statistiques)
	{
		calculs.emplace_back(s.first,s.second);
		temps_total += s.second._cumule;
		occurences_totales += s.second._occurences;
		if(s.second._attente)
		{
			occurences_attente_totales +=s.second._occurences;
			temps_attente_total += s.second._cumule;
		}
	}
	std::ostringstream oss;

	oss	<< std::endl
		<< "===========" << std::endl
		<< " Profiling " << std::endl
		<< "===========" << std::endl;

	{
	std::multimap<std::chrono::microseconds,Calculs> temps_cumule;
	for(const auto& c: calculs)
	{
		temps_cumule.insert(std::make_pair(c._cumule,c));
	}
	Tableau::Resultat<Tableau::NumerosCadre, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,200>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,70>  > resultat_temps_cumule;
	resultat_temps_cumule & "" & "Cumulated" & "Percentile" & "Occurences" & "Minimal" & "Average" & "Maximal" & "Speed" & "Stability" & "Accuracy" & "Address" & "Instruction" & "Location and flags";
	std::chrono::microseconds accumule_temps(0);
	std::list<unsigned char> paliers = { 5, 10, 25, 50, 75, 90, 95, 99 };
	for(auto it=temps_cumule.rbegin() ; it!=temps_cumule.rend() ; ++it)
	{
		std::string percentile;
		accumule_temps += it->first;
		auto ratio = (100*accumule_temps.count())/temps_total.count();
		for( ; ; )
		{
			if(paliers.empty())
				break;
			if(ratio>=paliers.front())
			{
				percentile = Tableau::Pourcentage(paliers.front());
				paliers.pop_front();
			}
			else
			{
				break;
			}
		}
		resultat_temps_cumule | "index" | it->first | percentile | it->second._occurences | it->second._minimum | it->second._moyen | it->second._maximum | Tableau::Pourcentage(it->second._vitesse) | Tableau::Pourcentage(it->second._stabilite_relative) | Tableau::Pourcentage(it->second._fiabilite) | it->second._texte_adresse | it->second._texte_instruction | it->second._texte_extra;
	}
	oss	<< std::endl
		<< "Instructions by cumulated time:" << std::endl
		<< std::endl
		<< resultat_temps_cumule;
	}
	
	{
	std::multimap<std::pair<size_t,std::chrono::microseconds>,Calculs> occurences;
	for(const auto& c: calculs)
	{
		occurences.insert(std::make_pair(std::make_pair(c._occurences,c._cumule),c));
	}
	Tableau::Resultat<Tableau::NumerosCadre, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,200>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,70>  > resultat_occurences;
	resultat_occurences & "" & "Occurences" & "Cumulated" & "Minimal" & "Average" & "Maximal" & "Speed" & "Stability" & "Accuracy" & "Address" & "Instruction" & "Location and flags";
	size_t nombre=0;
	for(auto it=occurences.rbegin() ; it!=occurences.rend() ; ++it)
	{
		if(++nombre>_details) break;
		resultat_occurences | "index"| it->first.first | it->second._cumule | it->second._minimum | it->second._moyen | it->second._maximum | Tableau::Pourcentage(it->second._vitesse) | Tableau::Pourcentage(it->second._stabilite_relative) | Tableau::Pourcentage(it->second._fiabilite) | it->second._texte_adresse | it->second._texte_instruction | it->second._texte_extra;
	}

	oss	<< std::endl
		<< "Instructions by occurences (Top " << _details << "):" << std::endl
		<< std::endl
		<< resultat_occurences;
	}
	
	bool attente = false;
	for( ; ; )
	{
	std::multimap<std::pair<std::chrono::microseconds,size_t>,Calculs> temps_moyen;
	for(const auto& c: calculs)
	{
		if(c._attente==attente)
		{
			temps_moyen.insert(std::make_pair(std::make_pair(c._moyen,c._occurences),c));
		}
	}
	if(not temps_moyen.empty())
	{
	Tableau::Resultat<Tableau::NumerosCadre, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,200>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,70>  > resultat;
	resultat & "" & "Average" & "Occurences" & "Cumulated" & "Minimal" & "Maximal" & "Speed" & "Stability" & "Accuracy" & "Address" & "Instruction" & "Location and flags";
	size_t nombre=0;
	for(auto it=temps_moyen.rbegin() ; it!=temps_moyen.rend() ; ++it)
	{
		if(++nombre>_details) break;
		resultat | "index" | it->first.first | it->second._occurences | it->second._cumule | it->second._minimum | it->second._maximum | Tableau::Pourcentage(it->second._vitesse) | Tableau::Pourcentage(it->second._stabilite_relative) | Tableau::Pourcentage(it->second._fiabilite) | it->second._texte_adresse | it->second._texte_instruction | it->second._texte_extra;
	}
	oss	<< std::endl
		<< "Instructions" << (attente?" WAITING":"") << " by average time (Top " << _details << "):" << std::endl
		<< std::endl
		<< resultat;
	}
	if(attente)
	{
		break;
	}
	else
	{
		attente = true;
	}
	}
	attente = false;
	for( ; ; )
	{
	std::multimap<std::pair<unsigned char,size_t>,Calculs> stabilite;
	for(const auto& c: calculs)
	{
		if(c._attente==attente)
		{
			stabilite.insert(std::make_pair(std::make_pair(c._stabilite_relative,c._occurences),c));
		}
	}
	if(not stabilite.empty())
	{
	Tableau::Resultat<Tableau::NumerosCadre, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,200>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,70>  > resultat;
	resultat & "" & "Stability" & "Occurences" & "Cumulated" & "Minimal" & "Average" & "Maximal" & "Speed" & "Accuracy" & "Address" & "Instruction" & "Location and flags";
	size_t nombre=0;
	for(auto it=stabilite.begin() ; it!=stabilite.end() ; ++it)
	{
		if(++nombre>_details) break;
		resultat | "index" | Tableau::Pourcentage(it->first.first) | it->second._occurences | it->second._cumule | it->second._minimum | it->second._moyen | it->second._maximum | Tableau::Pourcentage(it->second._vitesse) | Tableau::Pourcentage(it->second._fiabilite) | it->second._texte_adresse | it->second._texte_instruction | it->second._texte_extra;
	}
	oss	<< std::endl
		<< "Instructions" << (attente?" WAITING":"") << " by instability (Top " << _details << "):" << std::endl
		<< std::endl
		<< resultat;
	}
	if(attente)
	{
		break;
	}
	else
	{
		attente = true;
	}
	}
	attente = false;
	for( ; ; )
	{
	std::multimap<std::pair<unsigned char,size_t>,Calculs> vitesse;
	for(const auto& c: calculs)
	{
		if(c._attente==attente)
		{
			vitesse.insert(std::make_pair(std::make_pair(c._vitesse,c._occurences),c));
		}
	}
	if(not vitesse.empty())
	{
	Tableau::Resultat<Tableau::NumerosCadre, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::DROITE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,20>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,200>, Tableau::ColonneCadre<Tableau::Alignement::GAUCHE,70>  > resultat;
	resultat & "" & "Speed" & "Occurences" & "Cumulated" & "Minimal" & "Average" & "Maximal" & "Stability" & "Accuracy" & "Address" & "Instruction" & "Location and flags";
	size_t nombre=0;
	for(auto it=vitesse.begin() ; it!=vitesse.end() ; ++it)
	{
		if(++nombre>_details) break;
		resultat | "index" | Tableau::Pourcentage(it->first.first) | it->second._occurences | it->second._cumule | it->second._minimum | it->second._moyen | it->second._maximum | Tableau::Pourcentage(it->second._stabilite_relative) | Tableau::Pourcentage(it->second._fiabilite) | it->second._texte_adresse | it->second._texte_instruction | it->second._texte_extra;
	}
	oss	<< std::endl
		<< "Instructions" << (attente?" WAITING":"") << " by slowness (Top " << _details << "):" << std::endl
		<< std::endl
		<< resultat;
	}
	if(attente)
	{
		break;
	}
	else
	{
		attente = true;
	}
	}

	oss	<< std::endl
		<< "===============" << std::endl
		<< " Code coverage " << std::endl
		<< "===============" << std::endl
		<< std::endl;
	std::map<SVM::Machine::Element::Valeur::CodeSP,std::map<size_t,size_t>,ParIdentifiant> codes;
	for(const auto& s: _statistiques)
	{
		auto it = codes.insert(std::make_pair(s.first._code,std::map<size_t,size_t>()));
		it.first->second.insert(std::make_pair(s.first._locale,s.second._occurences));
	}
	size_t instructions_touchees = 0;
	size_t instructions_totales = 0;
	for(const auto& c: codes)
	{
		oss	<< std::endl
			<< "Coverage of <" << c.first->nom() << ">: " << c.second.size() << "/" << c.first->taille() << " (" << (c.first->taille()?(100*c.second.size()/c.first->taille()):100) << "\%)" << std::endl
			<< std::endl;
		c.first->trace(oss,SHARED_PTR(size_t)(),SVM::Machine::Element::Valeur::Code::Decorations::EXTRA,c.second);
		oss	<< std::endl;
		instructions_totales += c.first->taille();
		instructions_touchees += c.second.size();
	}
	unsigned long long int occurences_nonattente_totales = occurences_totales-occurences_attente_totales;
	std::chrono::microseconds temps_nonattente_total = temps_total-temps_attente_total;
	auto format = [] (auto t) -> std::string
	{
		std::ostringstream us;
		std::ostringstream ms;
		std::ostringstream s;
		std::ostringstream m;
		std::ostringstream h;
		std::ostringstream j;
		us << (t%1000) << "us";
		t/=1000;
		if(t>0)
		{
			ms << (t%1000) << "ms ";
			t/=1000;
			if(t>0)
			{
				s << (t%60) << "s ";
				t/=60;
				if(t>0)
				{
					m << (t%60) << "m ";
					t/=60;
					if(t>0)
					{
						h << (t%24) << "h ";
						t/=24;
						if(t>0)
						{
							j << t << "d ";
						}
					}
				}
			}
		}
		return j.str()+h.str()+m.str()+s.str()+ms.str()+us.str();
	};
	oss	<< std::endl
		<< "=========" << std::endl
		<< " Summary " << std::endl
		<< "=========" << std::endl
		<< std::endl;

	std::vector<std::string> titre = { "All", "Waiting", "Non-waiting" };
	std::vector<std::chrono::microseconds> temps = { temps_total , temps_attente_total , temps_nonattente_total };
	std::vector<unsigned long long int> nombre = { occurences_totales , occurences_attente_totales , occurences_nonattente_totales };
	for(size_t i=0 ; i<3 ; ++i)
	{
		auto moy = (nombre[i]?(temps[i].count()/nombre[i]):0);
		oss	<< "Profiling (" << titre[i] << " instructions):" << std::endl
			<< "  Cumulated time: " << temps[i].count() << "us (" << format(temps[i].count()) << ")" << std::endl
			<< "  Executed      : " << nombre[i] << " instruction" << (nombre[i]==1?"":"s") << std::endl
			<< "  Average time  : " << moy << "us (" << format(moy) << ")" <<  std::endl
			<< std::endl;
	}

	oss	<< std::endl
		<< "Code coverage:" << std::endl
		<< "  Global: " << instructions_touchees << "/" << instructions_totales << " (" << (instructions_totales?(100*instructions_touchees/instructions_totales):100) << "\%)" << std::endl;

	SVM::Machine::Trace(_historique,"Performance report") << oss.str();
	_statistiques.clear();
}

void PerformanceBrute::rapport()
{
	std::set<SVM::Machine::Element::Valeur::CodeSP> codes;
	for(const auto& a:_identifiants_adresse)
	{
		codes.insert(a.first._code);
	}
	size_t id_adresse = _identifiants_adresse.size();
	for(const auto& c:codes)
	{
		for(size_t l=0 ; l<c->taille() ; ++l)
		{
			SVM::Machine::Element::Valeur::AdresseInstruction adresse(c,l);
			auto it = _identifiants_adresse.find(adresse);
			if(it==_identifiants_adresse.end())
			{
				creation_adresse(adresse,++id_adresse);
			}
		}
	}
	_identifiants_adresse.clear();
}

void PerformanceBrute::creation_adresse(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const size_t id_adresse)
{
	Calculs c(adresse,Statistiques());
	//SVM::Machine::Trace(_historique,"Performance event",_brut) << SVM::Machine::Debugueur::JSON::objet({ { "type" , SVM::Machine::Debugueur::JSON::chaine("new") } , { "id" , SVM::Machine::Debugueur::JSON::entier(id_adresse) } , { "location" , SVM::Machine::Debugueur::JSON::objet({ { "svm" , SVM::Machine::Debugueur::JSON::entier(_identifiant_machine) } }) } , { "address" , SVM::Machine::Debugueur::JSON::objet({ {"code" , SVM::Machine::Debugueur::JSON::objet({ { "name" , SVM::Machine::Debugueur::JSON::chaine(adresse._code->_nom) } , { "id" , SVM::Machine::Debugueur::JSON::entier(adresse._code->_identifiant) } }) } , { "local" , SVM::Machine::Debugueur::JSON::entier(adresse._locale) } }) } , { "instruction" , SVM::Machine::Debugueur::JSON::objet({ {"text", SVM::Machine::Debugueur::JSON::chaine(c._texte_instruction) } , { "extra" , SVM::Machine::Debugueur::JSON::chaine(c._texte_extra) }, { "waiting", SVM::Machine::Debugueur::JSON::booleen(adresse.instruction()->attente()) } }) } });
	// Optimisation
	SVM::Machine::Trace(_historique,"Performance event",_brut) << "{ \"address\": { \"code\": { \"id\": " << adresse._code->_identifiant << ", \"name\": \"" << SVM::Global::Chaine::echappe(adresse._code->_nom) << "\" }, \"local\": " << adresse._locale << " }, \"id\": " << id_adresse << ", \"instruction\": { \"extra\": \"" << SVM::Global::Chaine::echappe(c._texte_extra) << "\", \"text\": \"" << SVM::Global::Chaine::echappe(c._texte_instruction) << "\", \"waiting\": " << (adresse.instruction()->attente()?"true":"false") << " }, \"location\": { \"svm\": " << _identifiant_machine << " }, \"type\": \"new\" }";
}
