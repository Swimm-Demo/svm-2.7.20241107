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

#include <src/machine/extensions/description.h>
#include <src/machine/extensions/automate_non_deterministe.h>
#include <src/machine/extensions/patron_objet_extension.h>
#include <src/outils/outils.h>

using namespace SVM::Machine::Extension;

RegexParametresSequence::operator AutomateNonDeterministeSP () const 
{
	AutomateNonDeterministeSP a = std::make_shared<AutomateNonDeterministe>();
	if(_elements.empty())
	{
		a->ajout_vide();
	}
	else
	{
		for(auto& e:_elements)
		{
			AutomateNonDeterministeSP aa = *e;
			a->ajout_automate_serie(*aa);
		}
	}
	return a;
}

/*
RegexParametresAutre::operator AutomateNonDeterministeSP () const 
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}
*/
RegexParametresJokerTotal::operator AutomateNonDeterministeSP () const 
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}


RegexParametresJokerPartielValeur::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresValeur::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresJokerPartielStructure::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresStructure::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresJokerPartielMarqueur::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresMarqueur::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresJokerPartielMotClef::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresJokerPartielVariable::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresMotClef::operator AutomateNonDeterministeSP () const
{
	return std::make_shared<AutomateNonDeterministe>(this->shared_from_this());
}

RegexParametresAlternativeSimple::operator AutomateNonDeterministeSP () const
{
	AutomateNonDeterministeSP a = std::make_shared<AutomateNonDeterministe>();
	EtatNonDeterministeSP i = EtatNonDeterministe::nouvel_etat();
	EtatNonDeterministeSP f = EtatNonDeterministe::nouvel_etat();
	a->ajout_initial(i);
	a->ajout_final(f);
	/*
	if(_inverse)
	{
		EtatNonDeterministeSP p = EtatNonDeterministe::nouvel_etat();
		a->ajout_transition(i,std::make_shared<RegexParametresAutre>(RegexParametresAutre::EnsembleExclus(_elements.begin(),_elements.end())),f);
		for(auto& e:_elements)
		{
			a->ajout_transition(i,e,p);
		}
	}
	else
	{
	*/
		for(auto& e:_elements)
		{
			a->ajout_transition(i,e,f);
		}
	/*
	}
	*/
	return a;
}

RegexParametresAlternative::operator AutomateNonDeterministeSP () const
{
	AutomateNonDeterministeSP a = std::make_shared<AutomateNonDeterministe>();
	for(auto& alter:_alternatives)
	{
		AutomateNonDeterministeSP aa = *alter;
		a->ajout_automate_parallele(*aa);
	}
	return a;
}

RegexParametresOptionnel::operator AutomateNonDeterministeSP () const
{
	AutomateNonDeterministeSP a = *_regex;
	a->ajout_final_depuis_initial();
	return a;
}

RegexParametresRepetitionOptionnel::operator AutomateNonDeterministeSP () const
{
	AutomateNonDeterministeSP a = *_regex;
	a->ajout_boucle_instantanee();
	a->ajout_final_depuis_initial();
	return a;
}

RegexParametresRepetitionObligatoire::operator AutomateNonDeterministeSP () const
{
	AutomateNonDeterministeSP a = *_regex;
	a->ajout_boucle_instantanee();
	return a;
}

RegexParametresRepetitionFixe::operator AutomateNonDeterministeSP () const
{
	AutomateNonDeterministeSP a = std::make_shared<AutomateNonDeterministe>();
	if(_repetitions==0)
	{
		a->ajout_vide();
		return a;
	}
	for(size_t i = 0 ; i<_repetitions ; ++i)
	{
		AutomateNonDeterministeSP motif = *_regex;
		a->ajout_automate_serie(*motif);
	}
	return a;
}

PatronObjetExtensionSP Type::patron(const std::string& nom, void *fichier) const
{
	std::ostringstream oss;
	oss << "type_" << _type.nom_entree() << "_delete";
	PatronTypeExtension::Destructeur d = reinterpret_cast<PatronTypeExtension::Destructeur>(::dlsym(fichier,oss.str().c_str()));
	if(not d) { throw ExtensionFonctionNonDefinie(nom,"void",oss.str(),"const void *svm, void *handler"); }
	oss.str("");;
	oss << "type_" << _type.nom_entree() << "_copy";
	PatronTypeExtension::Copie c = reinterpret_cast<PatronTypeExtension::Copie>(::dlsym(fichier,oss.str().c_str()));
	oss.str("");;
	oss << "type_" << _type.nom_entree() << "_compare";
	PatronTypeExtension::Comparaison cc = reinterpret_cast<PatronTypeExtension::Comparaison>(::dlsym(fichier,oss.str().c_str()));
	oss.str("");;
	oss << "type_" << _type.nom_entree() << "_constant";
	PatronTypeExtension::Constante ccc = reinterpret_cast<PatronTypeExtension::Constante>(::dlsym(fichier,oss.str().c_str()));
	oss.str("");;
	oss << "type_" << _type.nom_entree() << "_print";
	PatronTypeExtension::Affichage a = reinterpret_cast<PatronTypeExtension::Affichage>(::dlsym(fichier,oss.str().c_str()));
	return std::make_shared<PatronTypeExtension>(_type,d,c,cc,ccc,a);
}

PatronObjetExtensionSP Type::dependance(const std::string& nom) const 
{
	return std::make_shared<PatronTypeExtension>(_type);
}

PatronObjetExtensionSP Structure::patron(const std::string& nom, void *fichier) const
{
	std::ostringstream oss;
	oss << "struct_" << _structure.nom_entree() << "_delete";
	PatronStructureExtension::Destructeur d = reinterpret_cast<PatronStructureExtension::Destructeur>(::dlsym(fichier,oss.str().c_str()));
	if(not d) { throw ExtensionFonctionNonDefinie(nom,"void",oss.str(),"const void *svm, void *handler"); }
	oss.str("");;
	oss << "struct_" << _structure.nom_entree() << "_copy";
	PatronStructureExtension::Copie c = reinterpret_cast<PatronStructureExtension::Copie>(::dlsym(fichier,oss.str().c_str()));
	return std::make_shared<PatronStructureExtension>(_structure,d,c);
}

PatronObjetExtensionSP Structure::dependance(const std::string& nom) const
{
	return std::make_shared<PatronStructureExtension>(_structure);
}

PatronObjetExtensionSP Interruption::patron(const std::string& nom, void *fichier) const
{
	return std::make_shared<PatronInterruptionExtension>(_interruption);
}

PatronObjetExtensionSP Interruption::dependance(const std::string& nom) const 
{
	return std::make_shared<PatronInterruptionExtension>(_interruption);
}

PatronObjetExtensionSP Ordonnanceur::patron(const std::string& nom, void *fichier) const
{
	std::ostringstream oss;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_create";
	PatronOrdonnanceurExtension::Creation c = reinterpret_cast<PatronOrdonnanceurExtension::Creation>(::dlsym(fichier,oss.str().c_str()));
	if(not c) { throw ExtensionFonctionNonDefinie(nom,"void*",oss.str(),"const void *svm"); }
	oss.str("");;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_delete";
	PatronOrdonnanceurExtension::Destruction d = reinterpret_cast<PatronOrdonnanceurExtension::Destruction>(::dlsym(fichier,oss.str().c_str()));
	if(not d) { throw ExtensionFonctionNonDefinie(nom,"void",oss.str(),"const void *svm, void *scheduler"); }
	oss.str("");;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_attach";
	PatronOrdonnanceurExtension::Attache at = reinterpret_cast<PatronOrdonnanceurExtension::Attache>(::dlsym(fichier,oss.str().c_str()));
	if(not at) { throw ExtensionFonctionNonDefinie(nom,"SVM_Boolean",oss.str(),"const void *svm, void *scheduler, SVM_Process process, const unsigned long int parameter"); }
	oss.str("");;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_detach";
	PatronOrdonnanceurExtension::Detache dt = reinterpret_cast<PatronOrdonnanceurExtension::Detache>(::dlsym(fichier,oss.str().c_str()));
	if(not dt) { throw ExtensionFonctionNonDefinie(nom,"SVM_Boolean",oss.str(),"const void *svm, void *scheduler, SVM_Process process, const unsigned long int parameter"); }
	oss.str("");;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_schedule";
	PatronOrdonnanceurExtension::Ordonnance o = reinterpret_cast<PatronOrdonnanceurExtension::Ordonnance>(::dlsym(fichier,oss.str().c_str()));
	if(not o) { throw ExtensionFonctionNonDefinie(nom,"unsigned long int",oss.str(),"const void *svm, void *scheduler, SVM_Process process, const SVM_Process_State state"); }
	oss.str("");;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_notification";
	PatronOrdonnanceurExtension::Notification n = reinterpret_cast<PatronOrdonnanceurExtension::Notification>(::dlsym(fichier,oss.str().c_str()));
	oss.str("");;
	oss << "scheduler_" << _ordonnanceur.nom_entree() << "_print";
	PatronOrdonnanceurExtension::Affiche a = reinterpret_cast<PatronOrdonnanceurExtension::Affiche>(::dlsym(fichier,oss.str().c_str()));
	if(not a) { throw ExtensionFonctionNonDefinie(nom,"SVM_String",oss.str(),"const void *svm, void *scheduler"); }
	return std::make_shared<PatronOrdonnanceurExtension>(_ordonnanceur,c,d,at,dt,o,n,a);
}

PatronObjetExtensionSP Ordonnanceur::dependance(const std::string& nom) const
{
	return std::make_shared<PatronOrdonnanceurExtension>(_ordonnanceur);
}

PatronObjetExtensionSP Sequenceur::patron(const std::string& nom, void *fichier) const 
{
	std::ostringstream oss;
	oss << "sequencer_" << _sequenceur.nom_entree() << "_create";
	PatronSequenceurExtension::Creation c = reinterpret_cast<PatronSequenceurExtension::Creation>(::dlsym(fichier,oss.str().c_str()));
	if(not c) { throw ExtensionFonctionNonDefinie(nom,"void*",oss.str(),"const void *svm"); }
	oss.str("");;
	oss << "sequencer_" << _sequenceur.nom_entree() << "_delete";
	PatronSequenceurExtension::Destruction d = reinterpret_cast<PatronSequenceurExtension::Destruction>(::dlsym(fichier,oss.str().c_str()));
	if(not d) { throw ExtensionFonctionNonDefinie(nom,"void",oss.str(),"const void *svm, void *sequencer"); }
	oss.str("");;
	oss << "sequencer_" << _sequenceur.nom_entree() << "_current";
	PatronSequenceurExtension::Courant cc = reinterpret_cast<PatronSequenceurExtension::Courant>(::dlsym(fichier,oss.str().c_str()));
	if(not cc) { throw ExtensionFonctionNonDefinie(nom,"SVM_Kernel",oss.str(),"const void *svm, void *sequencer"); }
	oss.str("");;
	oss << "sequencer_" << _sequenceur.nom_entree() << "_attach";
	PatronSequenceurExtension::Attache at = reinterpret_cast<PatronSequenceurExtension::Attache>(::dlsym(fichier,oss.str().c_str()));
	if(not at) { throw ExtensionFonctionNonDefinie(nom,"SVM_Boolean",oss.str(),"const void *svm, void *sequencer, SVM_Kernel kernel, SVM_Size argc, SVM_Parameter argv[]"); }
	oss.str("");;
	oss << "sequencer_" << _sequenceur.nom_entree() << "_detach";
	PatronSequenceurExtension::Detache dt = reinterpret_cast<PatronSequenceurExtension::Detache>(::dlsym(fichier,oss.str().c_str()));
	if(not dt) { throw ExtensionFonctionNonDefinie(nom,"SVM_Boolean",oss.str(),"const void *svm, void *sequencer, SVM_Kernel kernel, SVM_Size argc, SVM_Parameter argv[]"); }
	oss.str("");;
	oss << "sequencer_" << _sequenceur.nom_entree() << "_print";
	PatronSequenceurExtension::Affiche a = reinterpret_cast<PatronSequenceurExtension::Affiche>(::dlsym(fichier,oss.str().c_str()));
	if(not a) { throw ExtensionFonctionNonDefinie(nom,"SVM_String",oss.str(),"const void *svm, void *sequencer"); }
	return std::make_shared<PatronSequenceurExtension>(_sequenceur,c,d,cc,at,dt,a);
}

PatronObjetExtensionSP Sequenceur::dependance(const std::string& nom) const
{
	return std::make_shared<PatronSequenceurExtension>(_sequenceur);
}

PatronObjetExtensionSP Instruction::patron(const std::string& nom, void *fichier) const
{
	std::ostringstream oss;
	oss << "instruction_" << _instruction.nom_entree();
	PatronInstructionExtension::Instruction i = reinterpret_cast<PatronInstructionExtension::Instruction>(::dlsym(fichier,oss.str().c_str()));
	if(not i) { throw ExtensionFonctionNonDefinie(nom,"SVM_Value",oss.str(),"const void *svm, const SVM_Size argc, SVM_Parameter argv[]"); }
	oss.str("");
	oss << *_parametres;
	AutomateNonDeterministeSP parametre = *_parametres;
	AutomateDeterministeSP automate_parametres = parametre->determinise(oss.str());
	AutomateDeterministeSP automate_retour;
	if(_retour)
	{
		oss.str("");
		oss << *_retour;
		AutomateNonDeterministeSP retour = *_retour;
		automate_retour = retour->determinise(oss.str());
	}
	return std::make_shared<PatronInstructionExtension>(_instruction,_systeme,_attente,_remplacement,i,automate_parametres,automate_retour);
}

PatronObjetExtensionSP Fonction::patron(const std::string& nom, void *fichier) const
{
	std::ostringstream oss;
	oss << "function_" << _fonction.nom_entree();
	PatronFonctionExtension::Fonction f = reinterpret_cast<PatronFonctionExtension::Fonction>(::dlsym(fichier,oss.str().c_str()));
	if(not f) { throw ExtensionFonctionNonDefinie(nom,"SVM_Variable",oss.str(),"const void *svm, const SVM_Size argc, SVM_Parameter argv[]"); }
	oss.str("");
	oss << *_parametres;
	AutomateNonDeterministeSP parametre = *_parametres;
	AutomateDeterministeSP automate_parametres = parametre->determinise(oss.str());
	AutomateDeterministeSP automate_retour;
	if(_retour)
	{
		oss.str("");
		oss << *_retour;
		AutomateNonDeterministeSP retour = *_retour;
		automate_retour = retour->determinise(oss.str());
	}
	return std::make_shared<PatronFonctionExtension>(_fonction,f,automate_parametres,automate_retour);
}

PatronObjetExtensionSP Instruction::dependance(const std::string& nom) const
{
	std::ostringstream oss;
	oss << *_parametres;
	AutomateNonDeterministeSP parametre = *_parametres;
	AutomateDeterministeSP automate_parametres = parametre->determinise(oss.str());
	AutomateDeterministeSP automate_retour;
	if(_retour)
	{
		oss.str("");
		oss << *_retour;
		AutomateNonDeterministeSP retour = *_retour;
		automate_retour = retour->determinise(oss.str());
	}
	return std::make_shared<PatronInstructionExtension>(_instruction,_systeme,_attente,_remplacement,automate_parametres,automate_retour);
}

PatronObjetExtensionSP Fonction::dependance(const std::string& nom) const
{
	std::ostringstream oss;
	oss << *_parametres;
	AutomateNonDeterministeSP parametre = *_parametres;
	AutomateDeterministeSP automate_parametres = parametre->determinise(oss.str());
	AutomateDeterministeSP automate_retour;
	if(_retour)
	{
		oss.str("");
		oss << *_retour;
		AutomateNonDeterministeSP retour = *_retour;
		automate_retour = retour->determinise(oss.str());
	}
	return std::make_shared<PatronFonctionExtension>(_fonction,automate_parametres,automate_retour);
}

PatronObjetExtensionSP Option::patron(const std::string& nom, void *fichier) const
{
	return std::make_shared<PatronOptionExtension>(_option,_clef,_type);
}

PatronObjetExtensionSP Option::dependance(const std::string& nom) const 
{
	return std::make_shared<PatronOptionExtension>(_option,_clef,_type);
}

PatronObjetExtensionSP Argument::patron(const std::string& nom, void *fichier) const
{
	return std::make_shared<PatronArgumentExtension>(_argument,_indice,_type);
}

PatronObjetExtensionSP Argument::dependance(const std::string& nom) const 
{
	return std::make_shared<PatronArgumentExtension>(_argument,_indice,_type);
}

PatronObjetExtensionSP Arguments::patron(const std::string& nom, void *fichier) const
{
	return std::make_shared<PatronArgumentsExtension>(_arguments);
}

PatronObjetExtensionSP Arguments::dependance(const std::string& nom) const 
{
	return std::make_shared<PatronArgumentsExtension>(_arguments);
}

void Type::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
	std::string nom = _type.nom_entree_pur();
	os	<< (langage?"":"typedef ") << "struct type_" << nom << std::endl
		<< "{" << std::endl;
	definition->corps(os);
	os	<< "}";
	if(not langage)
	{
		os << " type_" << nom << "_t";
	}
	os	<< ";" << std::endl
		<< std::endl;
	{
		SVM::Outils::FonctionSP suppression = definition->fonction("delete");
		os	<< "void type_" << nom << "_delete(const void *svm, void *handler)" << std::endl
			<< "{" << std::endl;
		if(suppression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os << "\ttype_" << nom << " * const object = reinterpret_cast<type_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os << "\ttype_" << nom << "_t * const object = (type_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		suppression->corps(os,definition->_fichier);
		if(suppression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tdelete object;" << std::endl;
			}
			else
			{
				os	<< "\t::free(object);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	auto copie = definition->fonction("copy");
	if(static_cast<bool>(copie))
	{
		os	<< "void* type_" << nom << "_copy(const void *svm, const void *handler)" << std::endl
			<< "{" << std::endl;
		if(copie->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tconst type_" << nom << " *object = reinterpret_cast<const type_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tconst type_" << nom << "_t *object = (const type_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		if(copie->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\ttype_" << nom << " *copy = new type_" << nom << "(*object);" << std::endl;
			}
			else
			{
				os	<< "\ttype_" << nom << "_t *copy = malloc(sizeof(type_" << nom << "_t));" << std::endl
					<< "\t*copy = *object;" << std::endl;
			}
		}
		copie->corps(os,definition->_fichier);
		if(copie->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn copy;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	auto comparaison = definition->fonction("compare");
	if(static_cast<bool>(comparaison))
	{
		os	<< "unsigned char type_" << nom << "_compare(const void *svm, const void *handler_left, const void *handler_right)" << std::endl
			<< "{" << std::endl;
		if(comparaison->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			for(const auto& t : { "left", "right"})
			{
				if(langage)
				{
					os	<< "\tconst type_" << nom << " *object_" << t << " = reinterpret_cast<const type_" << nom << "*>(handler_" << t << ");" << std::endl;
				}
				else
				{
					os	<< "\tconst type_" << nom << "_t *object_" << t << " = (const type_" << nom << "_t*)(handler_" << t << ");" << std::endl;
				}
			}
		}
		if(comparaison->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tbool equal = (*object_left)==(*object_right);" << std::endl;
			}
			else
			{
				os	<< "\tint equal = 0;" << std::endl;
			}
		}
		comparaison->corps(os,definition->_fichier);
		if(comparaison->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn (equal?EQUIVALENCE_EQUAL:EQUIVALENCE_DIFFERENT)|COMPARISON_STRONG;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	auto constante = definition->fonction("constant");
	if(static_cast<bool>(constante))
	{
		os	<< "void* type_" << nom << "_constant(const void *svm, const SVM_String value)" << std::endl
			<< "{" << std::endl;
		if(constante->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tstd::string string(value.string,value.size);" << std::endl;
			}
		}
		if(constante->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\ttype_" << nom << " *object = new type_" << nom << "(string);" << std::endl;
			}
			else
			{
				os	<< "\ttype_" << nom << "_t *object = malloc(sizeof(type_" << nom << "_t));" << std::endl;
			}
		}
		constante->corps(os,definition->_fichier);
		if(constante->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn object;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	auto impression = definition->fonction("print");
	if(static_cast<bool>(impression))
	{
		os	<< "SVM_String type_" << nom << "_print(const void *svm, const void *handler)" << std::endl
			<< "{" << std::endl;
		if(impression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tconst type_" << nom << " *object = reinterpret_cast<const type_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tconst type_" << nom << "_t *object = (const type_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		if(impression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tstd::string string = static_cast<std::string>(*object);" << std::endl;
			}
			else
			{
				os	<< "\tchar *string = " << langage.null() << ";" << std::endl;
			}
		}
		impression->corps(os,definition->_fichier);
		if(impression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\treturn ::svm_string_new(svm,string.c_str(),string.size());" << std::endl;
			}
			else
			{
				os	<< "\treturn ::svm_string_new_raw(svm,string);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
}

void Structure::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
	std::string nom = _structure.nom_entree_pur();
	os	<< (langage?"":"typedef ") << "struct struct_" << nom << std::endl
		<< "{" << std::endl;
	definition->corps(os);
	os	<< "}";
	if(not langage)
	{
		os << " struct_" << nom << "_t";
	}
	os	<< ";" << std::endl
		<< std::endl;
	{
		SVM::Outils::FonctionSP suppression = definition->fonction("delete");
		os	<< "void struct_" << nom << "_delete(const void *svm, void *handler)" << std::endl
			<< "{" << std::endl;
		if(suppression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os << "\tstruct_" << nom << " * const object = reinterpret_cast<struct_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os << "\tstruct_" << nom << "_t * const object = (struct_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		suppression->corps(os,definition->_fichier);
		if(suppression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tdelete object;" << std::endl;
			}
			else
			{
				os	<< "\t::free(object);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	auto copie = definition->fonction("copy");
	if(static_cast<bool>(copie))
	{
		os	<< "void* struct_" << nom << "_copy(const void *svm, const void *handler)" << std::endl
			<< "{" << std::endl;
		if(copie->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tconst struct_" << nom << " *object = reinterpret_cast<const struct_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tconst struct_" << nom << "_t *object = (const struct_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		if(copie->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tstruct_" << nom << " *copy = new struct_" << nom << "(*object);" << std::endl;
			}
			else
			{
				os	<< "\tstruct_" << nom << "_t *copy = malloc(sizeof(struct_" << nom << "_t));" << std::endl
					<< "\t*copy = *object;" << std::endl;
			}
		}
		copie->corps(os,definition->_fichier);
		if(copie->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn copy;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
}

void Instruction::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
	std::string nom = _instruction.nom_entree_pur();
	os	<< "SVM_Value instruction_" << nom << "(const void *svm, SVM_Size argc, SVM_Parameter argv[])" << std::endl
		<< "{" << std::endl;
	definition->corps(os);
	if(not _retour)
	{
		os	<< "\treturn " << langage.null() << ";" << std::endl;
	}
	os	<< "}" << std::endl
		<< std::endl;
}

void Fonction::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
	std::string nom = _fonction.nom_entree_pur();
	os	<< "SVM_Variable function_" << nom << "(const void *svm, SVM_Size argc, SVM_Parameter argv[])" << std::endl
		<< "{" << std::endl;
	definition->corps(os);
	if(not _retour)
	{
		os	<< "\treturn " << langage.null() << ";" << std::endl;
	}
	os	<< "}" << std::endl
		<< std::endl;
}

void Interruption::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
}

void Ordonnanceur::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
	std::string nom = _ordonnanceur.nom_entree_pur();
	os	<< (langage?"":"typedef ") << "struct scheduler_" << nom << std::endl
		<< "{" << std::endl;
	definition->corps(os);
	os	<< "}";
	if(not langage)
	{
		os << " scheduler_" << nom << "_t";
	}
	os	<< ";" << std::endl
		<< std::endl;
	{
		auto creation = definition->fonction("create");
		os	<< "void* scheduler_" << nom << "_create(const void *svm)" << std::endl
			<< "{" << std::endl;
		if(creation->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tscheduler_" << nom << " *object = new scheduler_" << nom << "();" << std::endl;
			}
			else
			{
				os	<< "\tscheduler_" << nom << "_t *object = malloc(sizeof(scheduler_" << nom << "_t));" << std::endl;
			}
		}
		creation->corps(os,definition->_fichier);
		if(creation->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn object;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		SVM::Outils::FonctionSP suppression = definition->fonction("delete");
		os	<< "void scheduler_" << nom << "_delete(const void *svm, void *handler)" << std::endl
			<< "{" << std::endl;
		if(suppression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os << "\tscheduler_" << nom << " * const object = reinterpret_cast<scheduler_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os << "\tscheduler_" << nom << "_t * const object = (scheduler_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		suppression->corps(os,definition->_fichier);
		if(suppression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tdelete object;" << std::endl;
			}
			else
			{
				os	<< "\t::free(object);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto ordonnance = definition->fonction("schedule");
		os	<< "unsigned long int scheduler_" << nom << "_schedule(const void *svm, void *handler, const SVM_Process process, const SVM_Process_State state)" << std::endl
			<< "{" << std::endl;
		if(ordonnance->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tscheduler_" << nom << " *object = reinterpret_cast<scheduler_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tscheduler_" << nom << "_t *object = (scheduler_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		ordonnance->corps(os,definition->_fichier);
		if(ordonnance->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn 0;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	auto notification = definition->fonction("notification");
	if(static_cast<bool>(notification))
	{
		os	<< "unsigned long int scheduler_" << nom << "_notification(const void *svm, void *handler, const SVM_Notification_Type type, unsigned long int parameter)" << std::endl
			<< "{" << std::endl;
		if(notification->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tscheduler_" << nom << " *object = reinterpret_cast<scheduler_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tscheduler_" << nom << "_t *object = (scheduler_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		notification->corps(os,definition->_fichier);
		if(notification->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn parameter;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto attache = definition->fonction("attach");
		os	<< "SVM_Boolean scheduler_" << nom << "_attach(const void *svm, void *handler, const SVM_Process process, unsigned long int parameter)" << std::endl
			<< "{" << std::endl;
		if(attache->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tscheduler_" << nom << " *object = reinterpret_cast<scheduler_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tscheduler_" << nom << "_t *object = (scheduler_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		attache->corps(os,definition->_fichier);
		if(attache->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn TRUE;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto detache = definition->fonction("detach");
		os	<< "SVM_Boolean scheduler_" << nom << "_detach(const void *svm, void *handler, const SVM_Process process, unsigned long int parameter)" << std::endl
			<< "{" << std::endl;
		if(detache->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tscheduler_" << nom << " *object = reinterpret_cast<scheduler_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tscheduler_" << nom << "_t *object = (scheduler_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		detache->corps(os,definition->_fichier);
		if(detache->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn TRUE;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto impression = definition->fonction("print");
		os	<< "SVM_String scheduler_" << nom << "_print(const void *svm, const void *handler)" << std::endl
			<< "{" << std::endl;
		if(impression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tconst scheduler_" << nom << " *object = reinterpret_cast<const scheduler_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tconst scheduler_" << nom << "_t *object = (const scheduler_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		if(impression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tstd::string string = static_cast<std::string>(*object);" << std::endl;
			}
			else
			{
				os	<< "\tchar *string = " << langage.null() << ";" << std::endl;
			}
		}
		impression->corps(os,definition->_fichier);
		if(impression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\treturn ::svm_string_new(svm,string.c_str(),string.size());" << std::endl;
			}
			else
			{
				os	<< "\treturn ::svm_string_new_raw(svm,string);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
}

void Sequenceur::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
	std::string nom = _sequenceur.nom_entree_pur();
	os	<< (langage?"":"typedef ") << "struct sequencer_" << nom << std::endl
		<< "{" << std::endl;
	definition->corps(os);
	os	<< "}";
	if(not langage)
	{
		os << " sequencer_" << nom << "_t";
	}
	os	<< ";" << std::endl
		<< std::endl;
	{
		auto creation = definition->fonction("create");
		os	<< "void* sequencer_" << nom << "_create(const void *svm)" << std::endl
			<< "{" << std::endl;
		if(creation->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tsequencer_" << nom << " *object = new sequencer_" << nom << "();" << std::endl;
			}
			else
			{
				os	<< "\tsequencer_" << nom << "_t *object = malloc(sizeof(sequencer_" << nom << "_t));" << std::endl;
			}
		}
		creation->corps(os,definition->_fichier);
		if(creation->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn object;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		SVM::Outils::FonctionSP suppression = definition->fonction("delete");
		os	<< "void sequencer_" << nom << "_delete(const void *svm, void *handler)" << std::endl
			<< "{" << std::endl;
		if(suppression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os << "\tsequencer_" << nom << " * const object = reinterpret_cast<sequencer_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os << "\tsequencer_" << nom << "_t * const object = (sequencer_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		suppression->corps(os,definition->_fichier);
		if(suppression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tdelete object;" << std::endl;
			}
			else
			{
				os	<< "\t::free(object);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto courant = definition->fonction("current");
		os	<< "SVM_Kernel sequencer_" << nom << "_current(const void *svm, void *handler)" << std::endl
			<< "{" << std::endl;
		if(courant->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tsequencer_" << nom << " *object = reinterpret_cast<sequencer_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tsequencer_" << nom << "_t *object = (sequencer_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		courant->corps(os,definition->_fichier);
		if(courant->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn " << langage.null() << ";" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto attache = definition->fonction("attach");
		os	<< "SVM_Boolean sequencer_" << nom << "_attach(const void *svm, void *handler, const SVM_Kernel kernel, const unsigned long int argc, const SVM_Parameter argv[])" << std::endl
			<< "{" << std::endl;
		if(attache->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tsequencer_" << nom << " *object = reinterpret_cast<sequencer_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tsequencer_" << nom << "_t *object = (sequencer_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		attache->corps(os,definition->_fichier);
		if(attache->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn TRUE;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto detache = definition->fonction("detach");
		os	<< "SVM_Boolean sequencer_" << nom << "_detach(const void *svm, void *handler, const SVM_Kernel kernel, const unsigned long int argc, const SVM_Parameter argv[])" << std::endl
			<< "{" << std::endl;
		if(detache->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tsequencer_" << nom << " *object = reinterpret_cast<sequencer_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tsequencer_" << nom << "_t *object = (sequencer_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		detache->corps(os,definition->_fichier);
		if(detache->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			os	<< "\treturn TRUE;" << std::endl;
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
	{
		auto impression = definition->fonction("print");
		os	<< "SVM_String sequencer_" << nom << "_print(const void *svm, const void *handler)" << std::endl
			<< "{" << std::endl;
		if(impression->_option!=SVM::Outils::Fonction::Option::AUCUNE)
		{
			if(langage)
			{
				os	<< "\tconst sequencer_" << nom << " *object = reinterpret_cast<const sequencer_" << nom << "*>(handler);" << std::endl;
			}
			else
			{
				os	<< "\tconst sequencer_" << nom << "_t *object = (const sequencer_" << nom << "_t*)(handler);" << std::endl;
			}
		}
		if(impression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\tstd::string string = static_cast<std::string>(*object);" << std::endl;
			}
			else
			{
				os	<< "\tchar *string = " << langage.null() << ";" << std::endl;
			}
		}
		impression->corps(os,definition->_fichier);
		if(impression->_option==SVM::Outils::Fonction::Option::DEFAUT)
		{
			if(langage)
			{
				os	<< "\treturn ::svm_string_new(svm,string.c_str(),string.size());" << std::endl;
			}
			else
			{
				os	<< "\treturn ::svm_string_new_raw(svm,string);" << std::endl;
			}
		}
		os	<< "}" << std::endl
			<< std::endl;
	}
}

void Option::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
}

void Argument::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
}

void Arguments::generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const 
{
}
