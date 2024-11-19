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

#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/memoire/memoire.h>
#include <src/machine/elements/memoire/entier.h>
#include <src/machine/elements/memoire/pointeur.h>
#include <src/machine/debugueur/debugueur.h>

using namespace SVM::Machine::Element::Memoire;

MemoireSP Memoire::creation_memoire(SVM::Machine::Debugueur::DebugueurSP& debugueur, const SVM_Noyau::ControleAccesSP& controle_acces)
{
	auto memoire = std::make_shared<Memoire>(controle_acces);
	memoire->_piege = SVM::Machine::Debugueur::Debugueur::ajoute_piege<PiegeMemoire>(debugueur,memoire);
	return memoire;
}

SVM_Valeur::Pointeur Memoire::allocation(const DescriptionBloc& bloc)
{
	size_t taille_bloc_demande = bloc._elements.size();
	if(static_cast<bool>(_utilisation))
	{
		_utilisation->augmente<SVM_Noyau::TropDeMemoireUtilisee>(taille_bloc_demande);
		if(static_cast<bool>(_piege)) { _piege->quota(_utilisation); }
	}
	auto memoire_libre_disponible = _memoire_libre.lower_bound(taille_bloc_demande);
	size_t adresse_debut;
	if(memoire_libre_disponible == _memoire_libre.end())
	{
		if(_memoire_libre.empty())
		{
			adresse_debut = _elements.size();
			_elements.insert(_elements.end(),taille_bloc_demande,ElementSP());
		}
		else
		{
			bool trouve = false;
			for(auto derniers_blocs = _memoire_libre.begin() ; derniers_blocs!=_memoire_libre.end() ; ++derniers_blocs)
			{
				if(derniers_blocs->second+derniers_blocs->first==_elements.size())
				{
					trouve=true;
					adresse_debut = derniers_blocs->second;
					_elements.insert(_elements.end(),taille_bloc_demande-derniers_blocs->first,ElementSP());
					_memoire_libre.erase(derniers_blocs);
					break;
				}
			}
			if(not trouve)
			{
				adresse_debut = _elements.size();
				_elements.insert(_elements.end(),taille_bloc_demande,ElementSP());
			}
		}
	}
	else
	{
		adresse_debut = memoire_libre_disponible->second;
		size_t taille_bloc_trouve = memoire_libre_disponible->first;
		_memoire_libre.erase(memoire_libre_disponible);
		if(taille_bloc_trouve-taille_bloc_demande>0)
		{
			_memoire_libre.insert(std::make_pair(taille_bloc_trouve-taille_bloc_demande,adresse_debut+taille_bloc_demande));
		}
	}
	size_t adresse = adresse_debut;
	for(auto& element: bloc._elements)
	{
		_elements[adresse].reset(new Element(element));
		if(static_cast<bool>(_piege))
		{ 
			_elements[adresse]->ajoute_reference_memoire(this->shared_from_this(),adresse);
		}
		++adresse;
	}
	if(static_cast<bool>(_piege)) { _piege->allocation(adresse_debut,bloc); }
	return SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(adresse_debut),taille_bloc_demande);
}

std::pair<bool,SVM_Valeur::Pointeur> Memoire::allocation(const DescriptionBloc& bloc, const SVM_Valeur::AdresseMemoire& adresse)
{
	size_t taille_bloc_demande = bloc._elements.size();
	bool trouve=false;
	for(auto& l: _memoire_libre)
	{
		if((adresse._adresse>=l.second) and ((adresse._adresse+taille_bloc_demande)<=(l.second+l.first)))
		{
			trouve = true;
			break;
		}
		if((adresse._adresse>=l.second) and ((l.second+l.first)==_elements.size()))
		{
			trouve = true;
			break;
		}
	}
	if(not trouve and not (adresse._adresse>=_elements.size()))
	{
		return std::make_pair(false,SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(0),0));
	}
	if(static_cast<bool>(_utilisation))
	{
		_utilisation->augmente<SVM_Noyau::TropDeMemoireUtilisee>(taille_bloc_demande);
		if(static_cast<bool>(_piege)) { _piege->quota(_utilisation); }
	}
	bool decoupage = false;
	decltype(_memoire_libre) memoire_libre;
	for(auto& l: _memoire_libre)
	{
		if((adresse._adresse>=l.second) and ((adresse._adresse+taille_bloc_demande)<=(l.second+l.first)))
		{
			if(adresse._adresse>l.second)
			{
				memoire_libre.insert(std::make_pair(adresse._adresse-l.second,l.second));
				decoupage = true;
			}
			if((adresse._adresse+taille_bloc_demande)<(l.second+l.first))
			{
				memoire_libre.insert(std::make_pair((l.second+l.first)-(adresse._adresse+taille_bloc_demande),adresse._adresse+taille_bloc_demande));
				decoupage = true;
			}
		}
		else if((adresse._adresse>=l.second) and ((l.second+l.first)==_elements.size()))
		{
			if(adresse._adresse>l.second)
			{
				memoire_libre.insert(std::make_pair(adresse._adresse-l.second,l.second));
				decoupage = true;
			}
		}
		else
		{
			memoire_libre.insert(l);
		}
	}
	_memoire_libre.swap(memoire_libre);
	if(not decoupage and (adresse._adresse>_elements.size()))
	{
		_memoire_libre.insert(std::make_pair(adresse._adresse-_elements.size(),_elements.size()));
	}
	if((adresse._adresse+taille_bloc_demande)>_elements.size())
	{
		_elements.insert(_elements.end(),adresse._adresse+taille_bloc_demande-_elements.size(),ElementSP());
	}
	size_t adresse_bloc = adresse._adresse;
	for(auto& element: bloc._elements)
	{
		_elements[adresse_bloc].reset(new Element(element));
		if(static_cast<bool>(_piege))
		{ 
			_elements[adresse_bloc]->ajoute_reference_memoire(this->shared_from_this(),adresse_bloc);
		}
		++adresse_bloc;
	}
	if(static_cast<bool>(_piege)) { _piege->allocation(adresse._adresse,bloc); }
	return std::make_pair(true,SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(adresse._adresse),taille_bloc_demande));
}

void Memoire::liberation(const std::set<SVM_Valeur::Pointeur>& bloc)
{
	size_t compteur = 0;
	for(auto& b:bloc)
	{
		SVM_Valeur::Pointeur vrai_bloc = b;
		if(vrai_bloc.apres()._adresse>_elements.size())
		{
			vrai_bloc._taille = _elements.size()-vrai_bloc._adresse._adresse;
		}
		for(size_t adresse = vrai_bloc._adresse._adresse ; adresse<vrai_bloc.apres()._adresse ; ++adresse)
		{
			if(static_cast<bool>(_elements[adresse]))
			{
				SVM_Synchronisation::EcrivainUP ecrivain;
				if(static_cast<bool>(_elements[adresse]->_acces))
				{
					ecrivain = _elements[adresse]->_acces->ecrivain();
				}
				if(static_cast<bool>(_piege))
				{
					_piege->point_arret_suppression(SVM_Valeur::AdresseMemoire(adresse),_elements[adresse]->_type,_elements[adresse]->_valeur);
					_elements[adresse]->enleve_reference_memoire(this->shared_from_this(),adresse);
				}
				_elements[adresse].reset();
				++compteur;
			}
		}
		_memoire_libre.insert(std::make_pair(vrai_bloc._taille,vrai_bloc._adresse._adresse));
	}
	std::map<size_t,bool> coupures;
	for(auto& ml:_memoire_libre)
	{
		auto cd = coupures.insert(std::make_pair(ml.second,true));
		if(not cd.second and not cd.first->second)
		{
			coupures.erase(cd.first);
		}
		auto cf = coupures.insert(std::make_pair(ml.second+ml.first,false));
		if(not cf.second and cf.first->second)
		{
			coupures.erase(cf.first);
		}
	}
	_memoire_libre.clear();
	size_t adresse_debut = 0;
	size_t adresse_fin = 0;
	bool precedent = false;
	for(auto& c:coupures)
	{
		//SVM_TRACE("coupure : " << c.first << " -> " << c.second << "/" << precedent << " [" << adresse_debut << ";" << adresse_fin << "]");
		if(not c.second)
		{
			adresse_fin = c.first;
		}
		if(not precedent and c.second)
		{
			if(adresse_fin>0 and (adresse_debut!=adresse_fin))
			{
				//SVM_TRACE( "ajout " << (adresse_fin-adresse_debut) << " -> " << adresse_debut );
				_memoire_libre.insert(std::make_pair(adresse_fin-adresse_debut,adresse_debut));
			}
			adresse_debut = c.first;
		}
		precedent=c.second; 
	}
	//SVM_TRACE( "ajout " << (adresse_fin-adresse_debut) << " -> " << adresse_debut );
	if(adresse_debut!=adresse_fin)
	{
		_memoire_libre.insert(std::make_pair(adresse_fin-adresse_debut,adresse_debut));
	}
	if(static_cast<bool>(_utilisation))
	{
		_utilisation->diminue<SVM_Noyau::TropDeMemoireUtilisee>(compteur);
		if(static_cast<bool>(_piege)) { _piege->quota(_utilisation); }
	}
	if(static_cast<bool>(_piege))
	{
		std::vector<PiegeMemoireElement> v;
		for(const auto& b: bloc)
		{
			for(size_t a = b.adresse()._adresse ; a<b.apres()._adresse ; ++a)
			{
				v.push_back(PiegeMemoireElement(a,TypeSP(),ValeurSP()));
			}
		}
		_piege->transformation(v,std::vector<PiegeMemoireAlias>());
	}
}

void Memoire::verification_alias(const DescriptionBloc& bloc)
{
	for(auto& alias:bloc._alias)
	{
		if(existence_alias(alias.first))
		{
			throw AliasDejaDefini(alias.first);
		}
	}
}

void Memoire::definition_alias(const DescriptionBloc& bloc, SVM_Valeur::Pointeur& emplacement)
{
	for(auto& alias:bloc._alias)
	{
		if(not _alias.insert(std::make_pair(alias.first,SVM_Valeur::Pointeur(emplacement.adresse()+(alias.second._debut),alias.second._taille))).second)
		{
			throw AliasDejaDefini(alias.first);
		}
	}
	if(static_cast<bool>(_piege))
	{
		std::vector<PiegeMemoireAlias> v;
		for(const auto& a:bloc._alias)
		{
			v.push_back(PiegeMemoireAlias(a.first,std::make_shared<SVM_Valeur::Pointeur>(emplacement.adresse()+(a.second._debut),a.second._taille)));
		}
		_piege->transformation(std::vector<PiegeMemoireElement>(),v);
	}
}

void Memoire::suppression_alias(const std::set<std::string>& alias)
{
	for(const auto& a:alias)
	{
		auto ra = _alias.find(a);
		if(ra!=_alias.end())
		{
			_alias.erase(ra);
		}
	}
	if(static_cast<bool>(_piege))
	{
		std::vector<PiegeMemoireAlias> v;
		for(const auto& a:alias)
		{
			v.push_back(PiegeMemoireAlias(a,SVM_Valeur::PointeurSP()));
		}
		_piege->transformation(std::vector<PiegeMemoireElement>(),v);
	}
}

void Memoire::definition_alias(const std::string& alias, SVM_Valeur::Pointeur& emplacement)
{
	auto it = _alias.insert(std::make_pair(alias,emplacement));
	if(not it.second)
	{
		throw AliasDejaDefini(alias);
	}
	if(static_cast<bool>(_piege))
	{
		_piege->transformation(PiegeMemoireAlias(alias,std::make_shared<SVM_Valeur::Pointeur>(emplacement)));
	}
}

bool Memoire::existence_alias(const std::string& alias)
{
	return _alias.find(alias)!=_alias.end();
}

SVM_Valeur::Pointeur Memoire::resolution_alias(const std::string& alias)
{
	auto ra = _alias.find(alias);
	if(ra!=_alias.end())
	{
		return ra->second;
	}
	throw AliasInexistant(alias);
}

template<bool strict>
ValeurSP Memoire::lecture(const SVM_Valeur::AdresseMemoire& adresse, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus, const bool point_arret)
{
	if(adresse._adresse>=_elements.size())
	{
		throw LectureAdresseNonDefinie(adresse);
	}
	ElementSP element = _elements[adresse._adresse];
	if(not element)
	{
		throw LectureAdresseNonDefinie(adresse);
	}
	SVM_Synchronisation::LecteurUP lecteur;
	if(static_cast<bool>(element->_acces))
	{
		lecteur = element->_acces->lecteur();
	}
	if(point_arret and static_cast<bool>(_piege))
	{
		_piege->point_arret_lecture(adresse,element->_type,element->_valeur);
	}
	if(not validateur_type(element->_type))
	{
		throw LectureAdresseTypeIncompatible(adresse,element->_type,types_attendus);
	}
	if(not element->_valeur)
	{
		if(strict)
		{
			throw LectureAdresseNonInitialisee(adresse);
		}
		else
		{
			return std::make_shared<ValeurNulle>(element->_type);
		}
	}
	return element->_valeur;
}
template ValeurSP Memoire::lecture<true>(const SVM_Valeur::AdresseMemoire& adresse, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus, const bool point_arret);
template ValeurSP Memoire::lecture<false>(const SVM_Valeur::AdresseMemoire& adresse, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus, const bool point_arret);

template<bool strict>
std::vector<ValeurSP> Memoire::lecture(const SVM_Valeur::Pointeur& pointeur, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus, const bool point_arret)
{
	if(pointeur.apres()>_elements.size())
	{
		if(pointeur.adresse()._adresse>_elements.size())
		{
			throw LectureAdresseNonDefinie(pointeur.adresse());
		}
		else
		{
			throw LectureAdresseNonDefinie(SVM_Valeur::AdresseMemoire(_elements.size()));
		}
	}
	std::vector<SVM_Synchronisation::LecteurUP> lecteurs;
	for(size_t adresse = pointeur.adresse()._adresse ; adresse<pointeur.apres()._adresse ; ++adresse)
	{
		ElementSP element = _elements[adresse];
		if(not element)
		{
			throw LectureAdresseNonDefinie(SVM_Valeur::AdresseMemoire(adresse));
		}
		if(static_cast<bool>(element->_acces))
		{
			lecteurs.push_back(element->_acces->lecteur());
		}
		if(point_arret and static_cast<bool>(_piege))
		{
			_piege->point_arret_lecture(SVM_Valeur::AdresseMemoire(adresse),element->_type,element->_valeur);
		}
		if(not validateur_type(element->_type))
		{
			throw LectureAdresseTypeIncompatible(SVM_Valeur::AdresseMemoire(adresse),element->_type,types_attendus);
		}
		if(strict)
		{
			if(not element->_valeur)
			{
				throw LectureAdresseNonInitialisee(SVM_Valeur::AdresseMemoire(adresse));
			}
		}
	}
	std::vector<ValeurSP> valeurs;
	for(size_t adresse = pointeur.adresse()._adresse ; adresse<pointeur.apres()._adresse ; ++adresse)
	{
		ElementSP element = _elements[adresse];
		if(not strict)
		{
			if(not element->_valeur)
			{
				valeurs.push_back(std::make_shared<ValeurNulle>(element->_type));
				continue;
			}
		}
		valeurs.push_back(element->_valeur);
	}
	return valeurs;
}
template std::vector<ValeurSP> Memoire::lecture<true>(const SVM_Valeur::Pointeur& pointeur, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus, const bool point_arret);
template std::vector<ValeurSP> Memoire::lecture<false>(const SVM_Valeur::Pointeur& pointeur, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus, const bool point_arret);

template<bool strict>
std::vector<ValeurSP> Memoire::lecture(const SVM_Valeur::Pointeur& pointeur, const DescriptionBloc& zone, const bool point_arret)
{
	if(zone.taille()!=pointeur.taille())
	{
		throw TailleBlocIncompatibles(zone.taille(),pointeur.taille());
	}
	if(pointeur.apres()>_elements.size())
	{
		if(pointeur.adresse()._adresse>_elements.size())
		{
			throw LectureAdresseNonDefinie(pointeur.adresse());
		}
		else
		{
			throw LectureAdresseNonDefinie(SVM_Valeur::AdresseMemoire(_elements.size()));
		}
	}
	std::vector<SVM_Synchronisation::LecteurUP> lecteurs;
	for(size_t adresse = pointeur.adresse()._adresse, indice=0 ; adresse<pointeur.apres()._adresse ; ++adresse,++indice)
	{
		ElementSP element = _elements[adresse];
		if(not element)
		{
			throw LectureAdresseNonDefinie(SVM_Valeur::AdresseMemoire(adresse));
		}
		if(static_cast<bool>(element->_acces))
		{
			lecteurs.push_back(element->_acces->lecteur());
		}
		if(point_arret and static_cast<bool>(_piege))
		{
			_piege->point_arret_lecture(SVM_Valeur::AdresseMemoire(adresse),element->_type,element->_valeur);
		}
		if(not (zone.element(indice)==Type::TypeInterne::AUTOMATIQUE or element->_type==zone.element(indice)))
		{
			std::ostringstream oss;
			oss << zone.element(indice);
			throw LectureAdresseTypeIncompatible(SVM_Valeur::AdresseMemoire(adresse),element->_type,oss.str());
		}
		if(strict)
		{
			if(not element->_valeur)
			{
				throw LectureAdresseNonInitialisee(SVM_Valeur::AdresseMemoire(adresse));
			}
		}
	}
	std::vector<ValeurSP> valeurs;
	for(size_t adresse = pointeur.adresse()._adresse ; adresse<pointeur.apres()._adresse ; ++adresse)
	{
		ElementSP element = _elements[adresse];
		if(not strict)
		{
			if(not element->_valeur)
			{
				valeurs.push_back(std::make_shared<ValeurNulle>(element->_type));
				continue;
			}
		}
		valeurs.push_back(element->_valeur);
	}
	return valeurs;
}
template std::vector<ValeurSP> Memoire::lecture<true>(const SVM_Valeur::Pointeur& pointeur, const DescriptionBloc& zone, const bool point_arret);
template std::vector<ValeurSP> Memoire::lecture<false>(const SVM_Valeur::Pointeur& pointeur, const DescriptionBloc& zone, const bool point_arret);

void Memoire::ecriture(const SVM_Valeur::AdresseMemoire& adresse, const ValeurSP& valeur)
{
	ASSERT(static_cast<bool>(valeur),"ecriture en memoire sans valeur");
	if(adresse._adresse>=_elements.size())
	{
		throw EcritureAdresseNonDefinie(adresse);
	}
	ElementSP element = _elements[adresse._adresse];
	if(not element)
	{
		throw EcritureAdresseNonDefinie(adresse);
	}
	SVM_Synchronisation::EcrivainUP ecrivain;
	if(static_cast<bool>(element->_acces))
	{
		ecrivain = element->_acces->ecrivain();
	}
	if(static_cast<bool>(_piege))
	{
		_piege->point_arret_ecriture(adresse,element->_type,element->_valeur,*valeur,valeur,_piege);
	}
	if(element->_type == Type(Type::TypeInterne::AUTOMATIQUE))
	{
		element->_type = *valeur;
	}
	else if(Type(Type::TypeInterne::AUTOMATIQUE)==*valeur)
	{
		ASSERT(valeur->nulle(),"ecriture valeur automatique non nulle");
	}
	else
	{
		if(not (element->_type== *valeur))
		{
			throw EcritureAdresseTypeIncompatible(adresse,*valeur,element->_type);
		}
	}
	if(static_cast<bool>(_piege) and static_cast<bool>(element->_valeur))
	{
		element->_valeur->enleve_debugueur_element(element);
	}
	if(valeur->nulle())
	{
		element->_valeur.reset();
	}
	else if(valeur->_deplacable)
	{
		valeur->_deplacable = false;
		element->_valeur = valeur;
	}
	else
	{
		element->_valeur = valeur->copie();
	}
	if(static_cast<bool>(_piege) and static_cast<bool>(element->_valeur))
	{
		element->_valeur->ajoute_debugueur_element(element);
	}
	if(static_cast<bool>(_piege))
	{
		element->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),adresse._adresse);
	}
}

void Memoire::ecriture(const SVM_Valeur::Pointeur& pointeur, const std::vector<ValeurSP>& valeurs)
{
	if(pointeur.taille()!=valeurs.size())
	{
		throw TailleBlocIncompatibles(valeurs.size(),pointeur.taille());
	}
	SVM_Valeur::AdresseMemoire adresse = pointeur.adresse();
	std::vector<SVM_Synchronisation::EcrivainUP> ecrivains;
	for(auto& v:valeurs)
	{
		if(adresse._adresse>=_elements.size())
		{
			throw EcritureAdresseNonDefinie(adresse);
		}
		ElementSP element = _elements[adresse._adresse];
		if(not element)
		{
			throw EcritureAdresseNonDefinie(adresse);
		}
		if(static_cast<bool>(element->_acces))
		{
			ecrivains.push_back(element->_acces->ecrivain());
		}
		if(static_cast<bool>(_piege))
		{
			_piege->point_arret_ecriture(adresse,element->_type,element->_valeur,*v,v,_piege);
		}
		if(Type(Type::TypeInterne::AUTOMATIQUE)==*v)
		{
			ASSERT(v->nulle(),"ecriture valeur automatique non nulle");
		}
		else
		{
			if(not ((element->_type== *v) or (element->_type == Type(Type::TypeInterne::AUTOMATIQUE))))
			{
				throw EcritureAdresseTypeIncompatible(adresse,*v,element->_type);
			}
		}
		++adresse;
	}
	adresse = pointeur.adresse();
	for(auto& v:valeurs)
	{
		ElementSP element = _elements[adresse._adresse];
		if(static_cast<bool>(_piege) and static_cast<bool>(element->_valeur))
		{
			element->_valeur->enleve_debugueur_element(element);
		}
		if(element->_type == Type(Type::TypeInterne::AUTOMATIQUE))
		{
			element->_type = *v;
		}
		if(v->nulle())
		{
			element->_valeur.reset();
		}
		else if(v->_deplacable)
		{
			v->_deplacable = false;
			element->_valeur = v;
		}
		else
		{
			element->_valeur = v->copie();
		}
		if(static_cast<bool>(_piege) and static_cast<bool>(element->_valeur))
		{
			element->_valeur->ajoute_debugueur_element(element);
		}
		++adresse;
	}
	if(static_cast<bool>(_piege))
	{
		for(size_t a = pointeur.adresse()._adresse ; a<pointeur.apres()._adresse ; ++a)
		{
			_elements[a]->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),a);
		}
	}
}

void Memoire::change_type(const SVM_Valeur::AdresseMemoire& adresse, const Type& type)
{
	if(adresse._adresse>=_elements.size())
	{
		throw EcritureAdresseNonDefinie(adresse);
	}
	ElementSP element = _elements[adresse._adresse];
	if(not element)
	{
		throw EcritureAdresseNonDefinie(adresse);
	}
	SVM_Synchronisation::EcrivainUP ecrivain;
	if(static_cast<bool>(element->_acces))
	{
		ecrivain = element->_acces->ecrivain();
	}
	if(static_cast<bool>(_piege))
	{
		_piege->point_arret_ecriture(adresse,element->_type,element->_valeur,type,SVM_Memoire::ValeurSP(),_piege);
	}
	if(static_cast<bool>(_piege) and static_cast<bool>(element->_valeur))
	{
		element->_valeur->enleve_debugueur_element(element);
	}
	element->_type = type;
	element->_valeur.reset();
	if(static_cast<bool>(_piege))
	{
		element->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),adresse._adresse);
	}
}

void Memoire::change_type(const SVM_Valeur::Pointeur& pointeur, const DescriptionBloc& zone)
{
	if(pointeur.taille()!=zone.taille())
	{
		throw TailleBlocIncompatibles(pointeur.taille(),zone.taille());
	}
	SVM_Valeur::AdresseMemoire adresse = pointeur.adresse();
	std::vector<SVM_Synchronisation::EcrivainUP> ecrivains;
	for(size_t i=0 ; i<zone.taille() ; ++i)
	{
		if(adresse._adresse>=_elements.size())
		{
			throw EcritureAdresseNonDefinie(adresse);
		}
		ElementSP element = _elements[adresse._adresse];
		if(not element)
		{
			throw EcritureAdresseNonDefinie(adresse);
		}
		if(static_cast<bool>(element->_acces))
		{
			ecrivains.push_back(element->_acces->ecrivain());
		}
		if(static_cast<bool>(_piege))
		{
			_piege->point_arret_ecriture(adresse,element->_type,element->_valeur,zone.element(i),SVM_Memoire::ValeurSP(),_piege);
		}
		++adresse;
	}
	adresse = pointeur.adresse();
	for(size_t i=0 ; i<pointeur.taille() ; ++i)
	{
		ElementSP element = _elements[adresse._adresse+i];
		if(static_cast<bool>(_piege) and static_cast<bool>(element->_valeur))
		{
			element->_valeur->enleve_debugueur_element(element);
		}
		element->_type = zone.element(i);
		element->_valeur.reset();
	}
	if(static_cast<bool>(_piege))
	{
		for(size_t a = pointeur.adresse()._adresse ; a<pointeur.apres()._adresse ; ++a)
		{
			_elements[a]->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),a);
		}
	}
}

void Memoire::active_synchronisation(const SVM_Valeur::Pointeur& pointeur)
{
	auto adresse = pointeur.adresse();
	for(size_t i=0 ; i<pointeur.taille() ; ++i)
	{
		ElementSP element = _elements[adresse._adresse];
		if(not element)
		{
			throw SynchronisationAdresseNonDefinie(adresse);
		}
		if(not element->_acces)
		{
			element->_acces = std::make_shared<SVM_Synchronisation::Acces>();
		}
		++adresse;
	}
}

void Memoire::desactive_synchronisation(const SVM_Valeur::Pointeur& pointeur)
{
	auto adresse = pointeur.adresse();
	for(size_t i=0 ; i<pointeur.taille() ; ++i)
	{
		ElementSP element = _elements[adresse._adresse];
		if(not element)
		{
			throw DesynchronisationAdresseNonDefinie(adresse);
		}
		if(not element->_acces)
		{
			element->_acces.reset();
		}
		++adresse;
	}
}

void Memoire::decalage(const SVM_Valeur::AdresseMemoire& adresse, const long long int increment)
{
	ValeurSP valeur = lecture(adresse,[] (const Type& type) -> bool { return type==Type::TypeInterne::ENTIER; },"INT");
	if(static_cast<bool>(_piege))
	{
		_piege->point_arret_ecriture(adresse,*valeur,valeur,*valeur,(*valeur)+increment,_piege);
	}
	(*valeur)+=increment;
	if(static_cast<bool>(_piege))
	{
		_elements[adresse._adresse]->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),adresse._adresse);
	}
}

std::set<SVM_Valeur::Pointeur> Memoire::accessible(const SVM_Valeur::Pointeur& pointeur) const
{
	std::set<SVM_Valeur::Pointeur> cascade = { pointeur };
	std::list<SVM_Valeur::Pointeur> liste = { pointeur };
	while(not liste.empty())
	{
		SVM_Valeur::Pointeur p = liste.front();
		liste.pop_front();
		//SVM_TRACE("pointeur considere :" << p);
		for(SVM_Valeur::AdresseMemoire adresse=p.adresse() ; adresse<p.apres() ; ++adresse)
		{
			if(not _elements[adresse._adresse])
			{
				//SVM_TRACE("adresse " << adresse << " rejetee : non definie");
				continue;
			}
			if(not (Type(Type::TypeInterne::POINTEUR) == _elements[adresse._adresse]->_type))
			{
				//SVM_TRACE("adresse " << adresse << " rejetee : pas un pointeur");
				continue;
			}
			if(not _elements[adresse._adresse]->_valeur)
			{
				//SVM_TRACE("adresse " << adresse << " rejetee : non initialisee");
				continue;
			}
			PointeurSP pp = std::dynamic_pointer_cast<Pointeur>(_elements[adresse._adresse]->_valeur);
			ASSERT(static_cast<bool>(pp),"valeur aurait du etre un pointeur");
			SVM_Valeur::Pointeur ppp = pp->_valeur;
			auto it=cascade.insert(ppp);
			if(it.second)
			{
				//SVM_TRACE("ajoute pointeur " << ppp << " pour la prochaine boucle depuis l'adresse " << adresse);
				liste.push_back(ppp);
			}
		}
	}
	std::map<size_t,bool> coupures;
	for(auto& ml:cascade)
	{
		auto cd = coupures.insert(std::make_pair(ml.adresse()._adresse,true));
		if(not cd.second and not cd.first->second)
		{
			coupures.erase(cd.first);
		}
		auto cf = coupures.insert(std::make_pair(ml.apres()._adresse,false));
		if(not cf.second and cf.first->second)
		{
			coupures.erase(cf.first);
		}
	}
	cascade.clear();
	size_t adresse_debut = 0;
	size_t adresse_fin = 0;
	bool precedent = false;
	for(auto& c:coupures)
	{
		//SVM_TRACE("coupure : " << c.first << " -> " << c.second << "/" << precedent << " [" << adresse_debut << ";" << adresse_fin << "]");
		if(not c.second)
		{
			adresse_fin = c.first;
		}
		if(not precedent and c.second)
		{
			if(adresse_fin>0)
			{
				//SVM_TRACE( "ajout &" << adresse_debut << "*" << (adresse_fin-adresse_debut));
				cascade.insert(SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(adresse_debut),adresse_fin-adresse_debut));
			}
			adresse_debut = c.first;
		}
		precedent=c.second; 
	}
	//SVM_TRACE( "ajout &" << adresse_debut << "*" << (adresse_fin-adresse_debut));
	cascade.insert(SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(adresse_debut),adresse_fin-adresse_debut));
	return cascade;
}

template<bool strict>
std::set<std::string> Memoire::alias_inclus(const std::set<SVM_Valeur::Pointeur>& pointeurs) const
{
	std::set<std::string> alias;
	for(auto& p:pointeurs)
	{
		for(auto& a:_alias)
		{
			if((strict and p.inclus(a.second)) or (not strict and p.intersecte(a.second)))
			{
				alias.insert(a.first);
			}
		}
	}
	return alias;
}
template std::set<std::string> Memoire::alias_inclus<true>(const std::set<SVM_Valeur::Pointeur>& pointeurs) const;
template std::set<std::string> Memoire::alias_inclus<false>(const std::set<SVM_Valeur::Pointeur>& pointeurs) const;

void Memoire::compatible(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination, std::vector<SVM_Synchronisation::LecteurUP>& lecteurs, std::vector<SVM_Synchronisation::EcrivainUP>& ecrivains) const
{
	if(origine.taille()!=destination.taille())
	{
		throw TailleBlocIncompatibles(origine.taille(),destination.taille());
	}
	SVM_Valeur::AdresseMemoire o = origine.adresse();
	SVM_Valeur::AdresseMemoire d = destination.adresse();
	for( ; origine.interieur(o) and destination.interieur(d) ; ++o, ++d)
	{
		SVM_TRACE(origine << "/" << o << " vs " << destination << "/" << d);
		if(o._adresse>=_elements.size())
		{
			throw LectureAdresseNonDefinie(o);
		}
		if(d._adresse>=distante->_elements.size())
		{
			throw EcritureAdresseNonDefinie(d);
		}
		ElementSP eo = _elements[o._adresse];
		ElementSP ed = distante->_elements[d._adresse];
		if(not eo)
		{
			throw LectureAdresseNonDefinie(o);
		}
		if(not ed)
		{
			throw EcritureAdresseNonDefinie(d);
		}
		if(static_cast<bool>(eo->_acces))
		{
			lecteurs.push_back(eo->_acces->lecteur());
		}
		if(static_cast<bool>(ed->_acces))
		{
			ecrivains.push_back(ed->_acces->ecrivain());
		}
		if(ed->_type==Type::TypeInterne::AUTOMATIQUE)
			continue;
		if(eo->_type==Type::TypeInterne::AUTOMATIQUE)
		{
			throw TypesIncompatibles(o,eo->_type,d,ed->_type);
		}
		if(eo->_type == ed->_type)
			continue;
		throw TypesIncompatibles(o,eo->_type,d,ed->_type);
	}
}

void Memoire::copie(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination)
{
	if(this==distante.get() and (origine==destination)) return;
	std::vector<SVM_Synchronisation::LecteurUP> lecteurs;
	std::vector<SVM_Synchronisation::EcrivainUP> ecrivains;
	compatible(origine,distante,destination,lecteurs,ecrivains);
	for(size_t adresse = origine.adresse()._adresse ; adresse<origine.apres()._adresse ; ++adresse)
	{
		if(not _elements[adresse]->_type.copiable())
		{
			throw CopieNonDisponible(SVM_Valeur::AdresseMemoire(adresse),_elements[adresse]->_type);
		}
	}
	if(origine.adresse()<destination.adresse())
	{
		for(size_t o = origine.apres()._adresse, d=destination.apres()._adresse ; ; )
		{
			if(o<=origine.adresse()._adresse) break;
			--o;
			--d;
			ElementSP& eo = _elements[o];
			ElementSP& ed = distante->_elements[d];
			if(static_cast<bool>(distante->_piege))
			{
				distante->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(d),ed->_type,ed->_valeur,eo->_type,eo->_valeur,_piege);
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->enleve_debugueur_element(ed);
			}
			if(ed->_type == Type::TypeInterne::AUTOMATIQUE)
			{
				ed->_type = eo->_type;
			}
			if(not eo->_valeur)
			{
				ed->_valeur.reset();
			}
			else
			{
				ed->_valeur = eo->_valeur->copie();
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->ajoute_debugueur_element(ed);
			}
			auto vpo = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(eo->_valeur);
			auto vpd = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(ed->_valeur);
			if(static_cast<bool>(vpo) and static_cast<bool>(vpd))
			{
				vpd->decalage(vpo,origine,destination);
			}
			if(static_cast<bool>(distante->_piege))
			{
				ed->notification_debugueur_changement_memoire(_piege,distante,d);
			}
		}
	}
	else
	{
		for(size_t o = origine._adresse._adresse, d=destination._adresse._adresse ; o<origine.apres()._adresse ; ++o,++d)
		{
			ElementSP& eo = _elements[o];
			ElementSP& ed = distante->_elements[d];
			if(static_cast<bool>(distante->_piege))
			{
				distante->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(d),ed->_type,ed->_valeur,eo->_type,eo->_valeur,_piege);
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->enleve_debugueur_element(ed);
			}
			if(ed->_type == Type::TypeInterne::AUTOMATIQUE)
			{
				ed->_type = eo->_type;
			}
			if(not eo->_valeur)
			{
				ed->_valeur.reset();
			}
			else
			{
				ed->_valeur = eo->_valeur->copie();
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->ajoute_debugueur_element(ed);
			}
			auto vpo = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(eo->_valeur);
			auto vpd = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(ed->_valeur);
			if(static_cast<bool>(vpo) and static_cast<bool>(vpd))
			{
				vpd->decalage(vpo,origine,destination);
			}
			if(static_cast<bool>(distante->_piege))
			{
				ed->notification_debugueur_changement_memoire(_piege,distante,d);
			}
		}
	}
}

void Memoire::deplacement(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination)
{
	if(this==distante.get() and (origine==destination)) return;
	std::vector<SVM_Synchronisation::LecteurUP> lecteurs;
	std::vector<SVM_Synchronisation::EcrivainUP> ecrivains;
	compatible(origine,distante,destination,lecteurs,ecrivains);
	if(origine.adresse()<destination.adresse())
	{
		for(size_t o = origine.apres()._adresse, d=destination.apres()._adresse ; ; )
		{
			if(o<=origine.adresse()._adresse) break;
			--o;
			--d;
			ElementSP& eo = _elements[o];
			ElementSP& ed = distante->_elements[d];
			if(static_cast<bool>(_piege))
			{
				_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(o),eo->_type,eo->_valeur,eo->_type,ValeurSP(),_piege);
			}
			if(static_cast<bool>(distante->_piege))
			{
				distante->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(d),ed->_type,ed->_valeur,eo->_type,eo->_valeur,_piege);
			}
			if(static_cast<bool>(_piege) and static_cast<bool>(eo->_valeur))
			{
				eo->_valeur->enleve_debugueur_element(eo);
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->enleve_debugueur_element(ed);
			}
			if(ed->_type == Type::TypeInterne::AUTOMATIQUE)
			{
				ed->_type = eo->_type;
			}
			ed->_valeur = eo->_valeur;
			eo->_valeur.reset();
			auto vpd = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(ed->_valeur);
			if(static_cast<bool>(vpd))
			{
				vpd->decalage(vpd,origine,destination);
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->ajoute_debugueur_element(ed);
			}
			if(static_cast<bool>(_piege))
			{
				eo->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),o);
			}
			if(static_cast<bool>(distante->_piege))
			{
				ed->notification_debugueur_changement_memoire(_piege,distante,d);
			}
		}
	}
	else
	{
		for(size_t o = origine._adresse._adresse, d=destination._adresse._adresse ; o<origine.apres()._adresse ; ++o,++d)
		{
			ElementSP& eo = _elements[o];
			ElementSP& ed = distante->_elements[d];
			if(static_cast<bool>(_piege))
			{
				_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(o),eo->_type,eo->_valeur,eo->_type,ValeurSP(),_piege);
			}
			if(static_cast<bool>(distante->_piege))
			{
				distante->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(d),ed->_type,ed->_valeur,eo->_type,eo->_valeur,_piege);
			}
			if(static_cast<bool>(_piege) and static_cast<bool>(eo->_valeur))
			{
				eo->_valeur->enleve_debugueur_element(eo);
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->enleve_debugueur_element(ed);
			}
			if(ed->_type == Type::TypeInterne::AUTOMATIQUE)
			{
				ed->_type = eo->_type;
			}
			ed->_valeur = eo->_valeur;
			eo->_valeur.reset();
			auto vpd = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(ed->_valeur);
			if(static_cast<bool>(vpd))
			{
				vpd->decalage(vpd,origine,destination);
			}
			if(static_cast<bool>(distante->_piege) and static_cast<bool>(ed->_valeur))
			{
				ed->_valeur->ajoute_debugueur_element(ed);
			}
			if(static_cast<bool>(_piege))
			{
				eo->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),o);
			}
			if(static_cast<bool>(distante->_piege))
			{
				ed->notification_debugueur_changement_memoire(_piege,distante,d);
			}
		}
	}
}

void Memoire::partage(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination)
{
	if(this==distante.get() and (origine==destination)) return;
	std::vector<SVM_Synchronisation::LecteurUP> lecteurs;
	std::vector<SVM_Synchronisation::EcrivainUP> ecrivains;
	compatible(origine,distante,destination,lecteurs,ecrivains);
	if(origine.adresse()<destination.adresse())
	{
		for(size_t o = origine.apres()._adresse, d=destination.apres()._adresse ; ; )
		{
			if(o<=origine.adresse()._adresse) break;
			--o;
			--d;
			ElementSP& eo = _elements[o];
			ElementSP& ed = distante->_elements[d];
			if(static_cast<bool>(distante->_piege))
			{
				distante->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(d),ed->_type,ed->_valeur,eo->_type,eo->_valeur,_piege);
			}
			if(static_cast<bool>(_piege))
			{
				ed->enleve_reference_memoire(distante,d);
			}
			ed=eo;
			if(static_cast<bool>(_piege))
			{
				ed->ajoute_reference_memoire(distante,d);
			}
			if(static_cast<bool>(_piege))
			{
				ed->notification_debugueur_changement_memoire(_piege,distante,d);
			}
		}
	}
	else
	{
		for(size_t o = origine._adresse._adresse, d=destination._adresse._adresse ; o<origine.apres()._adresse ; ++o,++d)
		{
			ElementSP& eo = _elements[o];
			ElementSP& ed = distante->_elements[d];
			if(static_cast<bool>(distante->_piege))
			{
				distante->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(d),ed->_type,ed->_valeur,eo->_type,eo->_valeur,_piege);
			}
			if(static_cast<bool>(_piege))
			{
				ed->enleve_reference_memoire(distante,d);
			}
			ed=eo;
			if(static_cast<bool>(_piege))
			{
				ed->ajoute_reference_memoire(distante,d);
			}
			if(static_cast<bool>(_piege))
			{
				ed->notification_debugueur_changement_memoire(_piege,distante,d);
			}
		}
	}
}

void Memoire::traduit(const SVM_Valeur::Pointeur& zone, const SVM_Valeur::Pointeur& origine, const SVM_Valeur::Pointeur& destination)
{
	for(size_t a = zone._adresse._adresse ; a<zone.apres()._adresse ; ++a)
	{
		if(a>=_elements.size())
		{
			throw LectureAdresseNonDefinie(SVM_Valeur::AdresseMemoire(a));
		}
		if(not _elements[a])
		{
			throw LectureAdresseNonDefinie(SVM_Valeur::AdresseMemoire(a));
		}
		auto v = _elements[a]->_valeur;
		if(not v)
		{
			continue;
		}
		auto vp = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(v);
		if(not vp)
		{
			continue;
		}
		SVM_Valeur::Pointeur p = (*vp);
		SVM_Valeur::Pointeur d = p;
		d.decalage(destination.adresse()-origine.adresse());
		if((not origine.inclus(p)) or (not destination.inclus(d)))
		{
			continue;
		}
		if(static_cast<bool>(_piege))
		{
			_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(a),_elements[a]->_type,v,_elements[a]->_type,std::make_shared<Pointeur>(d),_piege);
		}
		vp->decalage(vp,origine,destination);
		if(static_cast<bool>(_piege))
		{
			_elements[a]->notification_debugueur_changement_memoire(_piege,this->shared_from_this(),a);
		}
	}
}


SVM::Machine::Debugueur::JSON_Interne::ObjetSP PiegeMemoire::mise_a_jour()
{
	auto m = SVM::Machine::Debugueur::JSON::objet();
	auto memoire = _memoire.lock();
	if(not memoire)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	auto c = SVM::Machine::Debugueur::JSON::tableau();
	size_t adresse = 0;
	for(auto e: memoire->_elements)
	{
		if(static_cast<bool>(e))
		{
			std::ostringstream osst;
			osst << e->_type;
			auto ee = SVM::Machine::Debugueur::JSON::objet({ { "address" , SVM::Machine::Debugueur::JSON::entier(adresse) } , { "type" , SVM::Machine::Debugueur::JSON::chaine(osst.str()) } });
			if(static_cast<bool>(e->_valeur))
			{
				ee->ajoute("value",SVM::Machine::Debugueur::JSON::chaine(e->_valeur->serialise()));
				auto p = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(e->_valeur);
				if(static_cast<bool>(p))
				{
					SVM_Valeur::Pointeur pp = *p;
					ee->ajoute("pointer",pp.debugueur());
				}
			}
			c->ajoute(ee);
		}
		++adresse;
	}
	m->ajoute("content",c);
	auto a = SVM::Machine::Debugueur::JSON::tableau();
	for(auto alias: memoire->_alias)
	{
		auto aa = SVM::Machine::Debugueur::JSON::objet({ { "alias" , SVM::Machine::Debugueur::JSON::chaine(alias.first) } , { "pointer" , alias.second.debugueur() } });
		a->ajoute(aa);
	}
	m->ajoute("alias",a);
	if(static_cast<bool>(memoire->_utilisation))
	{
		m->ajoute("quota",memoire->_utilisation->debugueur());
	}
	return m;
}

void PiegeMemoire::evenement(const SVM_Debugueur::JSON_Interne::ObjetSP& evenement)
{
	auto debug = debugueur();
	auto synchro = SVM_Debugueur::JSON::entier((*evenement)["sync"]);
	if(SVM_Debugueur::JSON::est(synchro))
	{
		auto memoire = _memoire.lock();
		auto noyau = memoire->_noyau.lock();
		if(noyau->investigation()) // pour eviter les lectures concurrentes a des ecritures si la valeur n'est pas synchronisee
		{
			if((*synchro)>=0)
			{
				size_t adresse = static_cast<size_t>(*synchro);
				if((adresse)<memoire->_elements.size())
				{
					auto element = memoire->_elements[adresse];
					if(static_cast<bool>(element))
					{
						SVM_Synchronisation::LecteurUP lecteur;
						if(static_cast<bool>(element->_acces))
						{
							lecteur = element->_acces->lecteur();
						}
						transformation(PiegeMemoireElement(adresse,std::make_shared<Type>(element->_type),element->_valeur));
					}
				}
			}
		}
	}
	auto courant = SVM_Debugueur::JSON::booleen((*evenement)["current"]);
	if(SVM_Debugueur::JSON::est(courant))
	{
		auto memoire = _memoire.lock();
		std::lock_guard<std::mutex> verrou(Memoire::protection());
		auto noyau = memoire->_noyau.lock();
		auto memoire_courante = noyau->_processeur->memoire_courante();
		auto e = SVM_Debugueur::JSON::objet({ { "current" , memoire_courante.debugueur() } });
		notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
	}
}

void PiegeMemoire::allocation(const size_t adresse_debut, const DescriptionBloc& bloc)
{
	auto e = SVM::Machine::Debugueur::JSON::objet();
	auto c = SVM::Machine::Debugueur::JSON::tableau();
	size_t adresse = adresse_debut;
	for(const auto& e: bloc._elements)
	{
		std::ostringstream oss;
		oss << e;
		auto ee = SVM::Machine::Debugueur::JSON::objet({ { "address" , SVM::Machine::Debugueur::JSON::entier(adresse) } , { "type" , SVM::Machine::Debugueur::JSON::chaine(oss.str()) } });
		c->ajoute(ee);
		++adresse;
	}
	e->ajoute("content",c);
	auto a = SVM::Machine::Debugueur::JSON::tableau();
	for(const auto& aa: bloc._alias)
	{
		auto aaa = SVM::Machine::Debugueur::JSON::objet({ { "alias" , SVM::Machine::Debugueur::JSON::chaine(aa.first) } , { "pointer" , SVM::Machine::Debugueur::JSON::objet({ { "address" , SVM::Machine::Debugueur::JSON::entier(adresse_debut+aa.second._debut) } , { "size" , SVM::Machine::Debugueur::JSON::entier(aa.second._taille) } }) } });
		a->ajoute(aaa);
	}
	e->ajoute("alias",a);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeMemoire::quota(const SVM_Noyau::LimiteUtilisationSP& utilisation)
{
	auto e = SVM::Machine::Debugueur::JSON::objet();
	e->ajoute("quota",utilisation->debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeMemoire::transformation(const PiegeMemoireElement& element)
{
	std::vector<PiegeMemoireElement> v;
	v.push_back(element);
	transformation(v,std::vector<PiegeMemoireAlias>());
}

void PiegeMemoire::transformation(const PiegeMemoireAlias& alias)
{
	std::vector<PiegeMemoireAlias> v;
	v.push_back(alias);
	transformation(std::vector<PiegeMemoireElement>(),v);
}

void PiegeMemoire::transformation(const std::vector<PiegeMemoireElement>& elements, const std::vector<PiegeMemoireAlias>& alias)
{
	auto e = SVM::Machine::Debugueur::JSON::objet();
	auto c = SVM::Machine::Debugueur::JSON::tableau();
	for(const auto& el: elements)
	{
		auto ee = SVM::Machine::Debugueur::JSON::objet({ { "address" , SVM::Machine::Debugueur::JSON::entier(el._adresse) } });
		if(static_cast<bool>(el._type))
		{
			std::ostringstream oss;
			oss << (*el._type);
			ee->ajoute("type", SVM::Machine::Debugueur::JSON::chaine(oss.str()));
		}
		if(static_cast<bool>(el._valeur))
		{
			ee->ajoute("value", SVM::Machine::Debugueur::JSON::chaine(el._valeur->serialise()));
			auto p = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(el._valeur);
			if(static_cast<bool>(p))
			{
				SVM_Valeur::Pointeur pp = *p;
				ee->ajoute("pointer",pp.debugueur());
			}
		}
		c->ajoute(ee);
	}
	e->ajoute("content",c);
	auto a = SVM::Machine::Debugueur::JSON::tableau();
	for(const auto& aa: alias)
	{
		auto aaa = SVM::Machine::Debugueur::JSON::objet({ { "alias" , SVM::Machine::Debugueur::JSON::chaine(aa._alias) } });
		if(static_cast<bool>(aa._pointeur))
		{
 			aaa->ajoute("pointer",aa._pointeur->debugueur());
		}
		a->ajoute(aaa);
	}
	e->ajoute("alias",a);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeMemoire::point_arret_lecture(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout)
{
	if(ajout)
	{
		_points_arret_lecture.insert(adresse);
		return;
	}
	auto it = _points_arret_lecture.find(adresse);
	if(it==_points_arret_lecture.end()) return;
	_points_arret_lecture.erase(it);
}

void PiegeMemoire::point_arret_ecriture(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout)
{
	if(ajout)
	{
		_points_arret_ecriture.insert(adresse);
		return;
	}
	auto it = _points_arret_ecriture.find(adresse);
	if(it==_points_arret_ecriture.end()) return;
	_points_arret_ecriture.erase(it);
}

void PiegeMemoire::point_arret_acces(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout)
{
	if(ajout)
	{
		_points_arret_acces.insert(adresse);
		return;
	}
	auto it = _points_arret_acces.find(adresse);
	if(it==_points_arret_acces.end()) return;
	_points_arret_acces.erase(it);
}

void PiegeMemoire::point_arret_suppression(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout)
{
	if(ajout)
	{
		_points_arret_suppression.insert(adresse);
		return;
	}
	auto it = _points_arret_suppression.find(adresse);
	if(it==_points_arret_suppression.end()) return;
	_points_arret_suppression.erase(it);
}

void PiegeMemoire::point_arret_lecture(const SVM_Valeur::AdresseMemoire& adresse, const Type& type, const SVM_Memoire::ValeurSP& valeur)
{
	auto it = _points_arret_lecture.find(adresse);
	if(it == _points_arret_lecture.end()) return;
	std::ostringstream oss;
	oss << "Read at " << adresse << ":\n  Type: " << type << "\n  Value: ";
	if(static_cast<bool>(valeur))
	{
		oss << SVM::Global::Chaine::echappe_html(valeur->serialise());
	}
	oss << "\n";
	_memoire.lock()->_noyau.lock()->_processeur->_piege->point_arret("Memory",false,oss.str());
}

void PiegeMemoire::point_arret_ecriture(const SVM_Valeur::AdresseMemoire& adresse, const Type& type_avant, const SVM_Memoire::ValeurSP& valeur_avant, const Type& type_apres, const SVM_Memoire::ValeurSP& valeur_apres, const PiegeMemoireSP& piege)
{
	auto it = _points_arret_ecriture.find(adresse);
	if(it == _points_arret_ecriture.end()) return;
	std::ostringstream oss;
	oss << "Write at " << adresse << ":\n  Type: " << type_avant << "\n  Value: ";
	if(static_cast<bool>(valeur_avant))
	{
		oss << SVM::Global::Chaine::echappe_html(valeur_avant->serialise());
	}
	oss << "\nReplaced by:\n  Type: " << type_apres << "\n  Value: ";
	if(static_cast<bool>(valeur_apres))
	{
		oss << SVM::Global::Chaine::echappe_html(valeur_apres->serialise());
	}
	oss << "\n";
	bool interne = false;
	if(static_cast<bool>(piege))
	{
		interne = piege.get()==this;
	}
	if(interne)
		_memoire.lock()->_noyau.lock()->_processeur->_piege->point_arret("Memory",false,oss.str());
	else
		_memoire.lock()->_noyau.lock()->_processeur->_piege->point_arret_externe("Memory",oss.str());
}

void PiegeMemoire::point_arret_changement(const SVM_Valeur::AdresseMemoire& adresse, const Type& type_apres, const SVM_Memoire::ValeurSP& valeur_apres, const PiegeMemoireSP& piege)
{
	auto it = _points_arret_acces.find(adresse);
	if(it == _points_arret_acces.end()) return;
	std::ostringstream oss;
	oss << "Access at " << adresse << ":\n  Type: " << type_apres << "\n  Value: ";
	if(static_cast<bool>(valeur_apres))
	{
		oss << SVM::Global::Chaine::echappe_html(valeur_apres->serialise());
	}
	oss << "\n";
	bool interne = false;
	if(static_cast<bool>(piege))
	{
		interne = piege.get()==this;
	}
	if(interne)
		_memoire.lock()->_noyau.lock()->_processeur->_piege->point_arret("Memory",false,oss.str());
	else
		_memoire.lock()->_noyau.lock()->_processeur->_piege->point_arret_externe("Memory",oss.str());
}

void PiegeMemoire::point_arret_suppression(const SVM_Valeur::AdresseMemoire& adresse, const Type& type_avant, const SVM_Memoire::ValeurSP& valeur_avant)
{
	auto it = _points_arret_suppression.find(adresse);
	if(it == _points_arret_suppression.end()) return;
	std::ostringstream oss;
	oss << "Free at " << adresse << ":\n  Type: " << type_avant << "\n  Value: ";
	if(static_cast<bool>(valeur_avant))
	{
		oss << SVM::Global::Chaine::echappe_html(valeur_avant->serialise());
	}
	oss << "\n";
	_memoire.lock()->_noyau.lock()->_processeur->_piege->point_arret("Memory",false,oss.str());
}
