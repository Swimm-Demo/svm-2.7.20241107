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

#include <src/machine/machine/demarrage.h>
#include <src/machine/machine/historique.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/valeur/adresse_instruction.h>

namespace SVM
{
namespace Machine
{
namespace Performance
{
namespace Tableau
{
	enum class Alignement { GAUCHE, DROITE };

	template<Alignement alignement, size_t taille_max=0, char separateur = ' ', char ligne = ' ', char croisement = ' ' >
	struct Colonne
	{
		Colonne()
		:_taille(0),_entete(false) {};
		void ajout(const std::string& texte)
		{
			_valeurs.push_back(texte);
			if(texte.size()>_taille)
			{
				_taille = texte.size();
				if((taille_max>0) and (_taille>taille_max))
				{
					_taille = taille_max;
				}
			}
		};
		size_t _taille;
		std::string format(const size_t indice) const
		{
			std::string format;
			bool depasse=false;
			if(indice<_valeurs.size())
			{
				if(_valeurs[indice].size()>_taille)
				{
					if(alignement==Alignement::GAUCHE)
					{
						format=_valeurs[indice].substr(0,_taille);
					}
					else
					{
						format=_valeurs[indice].substr(_valeurs[indice].size()-_taille,_taille);
					}
					depasse=true;
				}
				else
				{
					std::string blanc(_taille-_valeurs[indice].size(),' ');
					if(alignement==Alignement::GAUCHE)
					{
						format = _valeurs[indice]+blanc;
					}
					else
					{
						format = blanc+_valeurs[indice];
					}
				}
			}
			else
			{
				format = std::string(_taille,' ');
			}
			std::ostringstream formatss;
			formatss << ((depasse and alignement==Alignement::DROITE)?'<':' ')
				<< format
				<< ((depasse and alignement==Alignement::GAUCHE)?'>':' ')
				<< separateur;
			return formatss.str();
		};
		std::string separation() const
		{
			std::string uneligne(_taille+2,ligne);
			uneligne += croisement;
			return uneligne;
		}

		void titre(const std::string& titre)
		{
			_entete = true;
			_titre = titre;
			_taille = titre.size();
			if((taille_max>0) and (_taille>taille_max))
			{
				_taille = taille_max;
			}
		}

		std::string titre() const
		{
			if(not _entete)
				return "";
			std::string titre(" ");
			titre+=_titre.substr(0,_taille);
			std::string blanc(_taille-_titre.size(),' ');
			titre+=blanc;
			titre+=' ';
			titre+=separateur;
			return titre;
		}
		size_t taille() const
		{
			return _valeurs.size();
		};
		private:
			std::string _titre;
			std::vector<std::string> _valeurs;
			bool _entete;
	};

	struct Incremente
	{
		void operator() (std::vector<size_t>& vecteur, const std::string& texte)
		{
			vecteur.push_back(vecteur.size()+1);
		};
	};

	struct Decremente
	{
		void operator() (std::vector<size_t>& vecteur, const std::string& texte)
		{
			vecteur.insert(vecteur.begin(),vecteur.size()+1);
		};
	};
	template<typename Ajout = Incremente, char separateur = ' ', char ligne = ' ', char croisement=' '>
	struct Numeros
	{
		Numeros()
		:_entete(false) {};
		void ajout(const std::string& texte)
		{
			Ajout ajout;
			ajout(_valeurs,texte);
		}
		std::string format(const size_t indice) const
		{
			std::string format;
			if(indice<_valeurs.size())
			{
				std::ostringstream oss;
				oss << std::setw(5) << _valeurs[indice];
				std::string valeur = oss.str();
				std::string blanc(5-valeur.size(),' ');
				format = blanc+valeur;
			}
			else
			{
				format = std::string(5,' ');
			}
			format += separateur;
			return format;
		};
		std::string separation() const
		{
			std::string uneligne(5,ligne);
			uneligne += croisement;
			return uneligne;
		}

		std::string titre() const
		{
			if(not _entete)
				return "";
			return std::string("Index")+separateur;
		}

		void titre(const std::string& titre)
		{
			_entete=true;
		}
		size_t taille() const
		{
			return _valeurs.size();
		};
		protected:
			std::vector<size_t> _valeurs;
			bool _entete;
	};

	template<Alignement alignement, size_t taille_max = 0>
	struct ColonneCadre : public Colonne<alignement,taille_max,'|','-','+'>
	{
	};
	
	struct NumerosCadre : public Numeros<Incremente,'|','-','+'>
	{
	};

	template<char separateur = ' ', char ligne = ' ', char croisement=' '>
	struct NumerosInverses : public Numeros<Decremente,separateur,ligne,croisement>
	{
	};

	struct NumerosInversesCadre : public NumerosInverses<'|','-','+'>
	{
	};

	template<typename... Colonnes>
	class Resultat
	{
		public:
			std::string separation() const
			{
				return "";
			}
			std::string titre() const
			{
				return "";
			}
			std::string format(const size_t indice) const
			{
				return "";
			}
	};

	struct Pourcentage
	{
		explicit Pourcentage(const unsigned char v)
		:_v(v) {}
		operator std::string () const
		{
			std::ostringstream oss;
			oss << static_cast<unsigned int>(_v) << "\%";
			return oss.str();
		}
		unsigned char _v;
	};

	template<typename T>
	struct Texte
	{
		explicit Texte(const T& t)
		:_t(t) {}

		operator std::string () const
		{
			std::ostringstream oss;
			oss << _t;
			return oss.str();
		}

		const T& _t;
	};

	template<typename Colonne, typename... ColonnesSuivantes>
	class Resultat<Colonne,ColonnesSuivantes...>
	{
		public:
			Resultat<Colonne,ColonnesSuivantes...>() {};
			friend Resultat<ColonnesSuivantes...>& operator&(Resultat<Colonne,ColonnesSuivantes...>& resultat, const std::string& texte) 
			{
				resultat._colonne.titre(texte);
				return resultat._suivantes;
			};
			friend Resultat<ColonnesSuivantes...>& operator|(Resultat<Colonne,ColonnesSuivantes...>& resultat, const size_t nombre) 
			{
				std::stringstream oss;
				oss << nombre;
				resultat._colonne.ajout(oss.str());
				return resultat._suivantes;
			};
			friend Resultat<ColonnesSuivantes...>& operator|(Resultat<Colonne,ColonnesSuivantes...>& resultat, const std::string& texte) 
			{
				resultat._colonne.ajout(texte);
				return resultat._suivantes;
			};
			friend Resultat<ColonnesSuivantes...>& operator|(Resultat<Colonne,ColonnesSuivantes...>& resultat, const std::chrono::microseconds& temps) 
			{
				std::stringstream oss;
				oss << temps.count() << "us";
				resultat._colonne.ajout(oss.str());
				return resultat._suivantes;
			};
			template<typename oStream>
			friend oStream& operator<<(oStream& os, const Resultat<Colonne,ColonnesSuivantes...>& resultat)
			{
				if(not resultat._colonne.titre().empty())
				{
					os << resultat._colonne.separation();
					os << resultat._suivantes.separation();
					os << std::endl;
					os << resultat._colonne.titre();
					os << resultat._suivantes.titre();
					os << std::endl;
				}
				os << resultat._colonne.separation();
				os << resultat._suivantes.separation();
				os << std::endl;
				for(size_t indice=0 ; indice<resultat._colonne.taille() ; ++indice)
				{
					os << resultat._colonne.format(indice);
					os << resultat._suivantes.format(indice);
					os << std::endl;
				}
				os << resultat._colonne.separation();
				os << resultat._suivantes.separation();
				os << std::endl;
				return os;
			};
			std::string format(const size_t indice) const
			{
				return _colonne.format(indice)+_suivantes.format(indice);
			}

			std::string separation() const
			{
				return _colonne.separation()+_suivantes.separation();
			}

			std::string titre() const
			{
				return _colonne.titre()+_suivantes.titre();
			}
		private:
			Colonne _colonne;
			Resultat<ColonnesSuivantes...> _suivantes;
	};
}

struct Statistiques
{
	virtual ~Statistiques() {}
	bool _attente = false;
	unsigned long long int _occurences = 1;
	unsigned long long int _occurences_remplacee = 0;
	std::chrono::microseconds _minimum {0}; 
	std::chrono::microseconds _maximum {0}; 
	std::chrono::microseconds _cumule {0}; 
};

struct Calculs : public Statistiques
{
	Calculs(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const Statistiques& s)
	:Statistiques(s), _moyen(_cumule/_occurences), _vitesse((_maximum-_minimum).count()?(100*(_maximum-_moyen).count())/(_maximum-_minimum).count():100), _fiabilite(100-100*_occurences_remplacee/_occurences), _stabilite(_maximum-_minimum), _stabilite_relative(_maximum.count()?(100-100*(_maximum-_minimum)/_maximum):100),  _adresse(adresse)
	{
		std::ostringstream adr;
		adr << adresse;
		_texte_adresse = adr.str();
		std::ostringstream instr;
		instr << (*_adresse.instruction());
		size_t blanc=0;
		for(const auto& c: instr.str())
		{
			unsigned char n = c;
			if((n=='\n') or (n=='\t')) n=' ';
			if(n==' ')
			{
				if(blanc++)
					continue;
			}
			else
			{
				blanc=0;
			}
			_texte_instruction += n;
		}
		std::ostringstream caracteristiques;
		_adresse.instruction()->extra(caracteristiques);
		_texte_extra = caracteristiques.str().substr(1);
	}
	virtual ~Calculs() {}
	std::chrono::microseconds _moyen {0};
	unsigned char _vitesse = 0 ;
	unsigned char _fiabilite = 0;
	std::chrono::microseconds _stabilite {0};
	unsigned char _stabilite_relative = 0 ;
	SVM::Machine::Element::Valeur::AdresseInstruction _adresse;
	std::string _texte_adresse;
	std::string _texte_instruction;
	std::string _texte_extra;
};

DECL_SHARED_PTR(Performance);

struct Performance
{
	explicit Performance(const SVM::Machine::HistoriqueSP& historique, const bool brut)
	:_historique(historique), _brut(brut) {}
	virtual ~Performance() {}
	static PerformanceSP creation(const SVM::Machine::PerfSP& perf, const SVM::Machine::HistoriqueSP& historique);
	Performance(const bool brut, const size_t details, const SVM::Machine::HistoriqueSP& historique);
	static void rapport(const PerformanceSP& performance)
	{
		if(not performance) return;
		performance->rapport();
	}
	template<typename Flux>
	void nom_flux(Flux& f) const
	{
		f << "PERFORMANCE"; 
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Performance& perf)
	{
		f << "Performance";
		return f;
	}
	virtual void evenement(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const bool remplacee, const std::chrono::time_point<std::chrono::steady_clock>& debut, const std::chrono::time_point<std::chrono::steady_clock>& fin, const SVM::Machine::Element::Noyau::NoyauSP& noyau, const size_t taille_pile) = 0;
	virtual void rapport() = 0;
	protected:
	mutable std::mutex _protection;
	SVM::Machine::HistoriqueSP _historique;
	bool _brut;
};

struct PerformanceRapport : public Performance
{
	PerformanceRapport(const SVM::Machine::HistoriqueSP& historique, const size_t details, const bool brut)
	:Performance(historique,brut), _details(details) {}
	virtual ~PerformanceRapport() {}
	virtual void evenement(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const bool remplacee, const std::chrono::time_point<std::chrono::steady_clock>& debut, const std::chrono::time_point<std::chrono::steady_clock>& fin, const SVM::Machine::Element::Noyau::NoyauSP& noyau, const size_t taille_pile) override;
	virtual void rapport() override;
	private:
		size_t _details;
		std::map<SVM::Machine::Element::Valeur::AdresseInstruction,Statistiques> _statistiques;
};

struct PerformanceBrute : public Performance
{
	PerformanceBrute(const SVM::Machine::HistoriqueSP& historique, const bool brut)
	:Performance(historique,brut), _identifiant_machine(::getpid()), _reference(std::chrono::steady_clock::now()) { }
	virtual ~PerformanceBrute() {}
	virtual void evenement(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const bool remplacee, const std::chrono::time_point<std::chrono::steady_clock>& debut, const std::chrono::time_point<std::chrono::steady_clock>& fin, const SVM::Machine::Element::Noyau::NoyauSP& noyau, const size_t taille_pile) override;
	virtual void rapport() override;
	void creation_adresse(const SVM::Machine::Element::Valeur::AdresseInstruction& adresse, const size_t id_adresse);
	private:
		std::map<SVM::Machine::Element::Valeur::AdresseInstruction,size_t> _identifiants_adresse;
		pid_t _identifiant_machine;
		std::chrono::time_point<std::chrono::steady_clock> _reference;
};

struct ParIdentifiant
{
	bool operator() (const SVM::Machine::Element::Valeur::CodeSP& g, const SVM::Machine::Element::Valeur::CodeSP& d) const
	{
		return g->_identifiant<d->_identifiant;
	}
};
}
}
}
