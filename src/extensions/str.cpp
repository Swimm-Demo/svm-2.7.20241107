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

#include <sstream>
#include <string>
#include <regex>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN str \n"
		"DEFINE \n"
		"  TYPE str.pattern \n"
		"  INSTRUCTION str.pattern STR:pattern -> str.pattern \n"
		"  FUNCTION str.pattern STR:pattern -> str.pattern \n"
		"  INSTRUCTION str.size STR -> INT \n"
		"  INSTRUCTION str.char INT:ascii_code -> STR \n"
		"  INSTRUCTION str.index STR INT:index -> INT:ascii_code \n"
		"  INSTRUCTION str.change MUTABLE STR INT:index INT:ascii_code \n"
		"  INSTRUCTION str.prepend MUTABLE STR INT:ascii_code \n"
		"  INSTRUCTION str.append MUTABLE STR INT:ascii_code \n"
		"  INSTRUCTION str.remove MUTABLE STR INT:index \n"
		"  INSTRUCTION str.sub STR INT:offset INT:size 'END':origin_at_end ? -> STR \n"
		"  INSTRUCTION str.cmp STR [ < > = <> <= => ] STR -> BLN \n"
		"  INSTRUCTION str.find STR [ STR str.pattern ]:pattern MUTABLE INT:position MUTABLE INT:size ? -> BLN \n"
		"  INSTRUCTION str.replace MUTABLE STR [ INT 'ALL' ]:occurence [ STR str.pattern ]:pattern_old => STR:new \n"
		"  INSTRUCTION str.join ( PTR:array_of_strings | STR + | STR:separator { ( PTR:array_of_strings | STR + ) } ) -> STR \n"
		"  INSTRUCTION str.split STR [ STR str.pattern ]:separator_pattern -> PTR \n"
		"  INSTRUCTION str.regex STR [ STR str.pattern ]:pattern -> PTR \n"
		"  INSTRUCTION str.format ( VALUE | { PEP . * } ) + -> STR \n"
		"  FUNCTION str.join ( PTR:array_of_strings | STR + | STR:separator { ( PTR:array_of_strings | STR + ) } ) -> STR \n"
		"  FUNCTION str.sub STR INT:offset INT:size 'END' ? -> STR \n"
		"  FUNCTION str.size STR -> INT \n"
		"  FUNCTION str.cmp STR [ < > = <> <= => ] STR -> BLN \n"
		"  FUNCTION str.format STR INT:size [ 'LEFT' 'CENTER' 'RIGHT' ] ? -> STR \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}
	
	struct Regex
	{
		std::string _pattern;
		std::regex _regex;
	};

	void type_pattern_delete(const void *svm, void *objet)
	{
		Regex *regex = reinterpret_cast<Regex*>(objet);
		delete regex;
	}

	void* type_pattern_copy(const void *svm, void *objet)
	{
		Regex *regex = reinterpret_cast<Regex*>(objet);
		Regex *copie = new Regex;
		*copie = *regex;
		return copie;
	}

	unsigned char type_pattern_compare(const void *svm, const void *gauche, const void *droit)
	{
		const Regex *g = reinterpret_cast<const Regex*>(gauche);
		const Regex *d = reinterpret_cast<const Regex*>(droit);
		if(g->_pattern<d->_pattern) return ORDER_INFERIOR|COMPARISON_WEAK;
		if(g->_pattern>d->_pattern) return ORDER_SUPERIOR|COMPARISON_WEAK;
		return ORDER_EQUAL|COMPARISON_WEAK;
	}
	void type_pattern_constant_locale(const void *svm, const SVM_String valeur, Regex& locale, Regex *pointeur=nullptr)
	{
		locale._pattern = std::string(valeur.string,valeur.size);
		try
		{
			locale._regex = std::regex(locale._pattern);
		}
		catch(const std::regex_error& e)
		{
			if(pointeur) delete pointeur;
			std::ostringstream oss;
			oss << "Invalid pattern: " << e.what() ;
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,oss.str().c_str());
		}
	}
	void *type_pattern_constant(const void *svm, const SVM_String constante)
	{
		Regex *regex = new Regex;
		type_pattern_constant_locale(svm,constante,*regex,regex);
		return regex;
	}

	SVM_String type_pattern_print(const void *svm, const void *objet)
	{
		const Regex *regex = reinterpret_cast<const Regex*>(objet);
		return ::svm_string_new__raw(svm,regex->_pattern.c_str());
	}

	SVM_Value instruction_pattern(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String s = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		void *objet = ::type_pattern_constant(svm,s);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"str","pattern"),objet);
	}

	SVM_Value function_pattern(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_pattern(svm,argc,argv);
	}

	SVM_Value instruction_size(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::svm_value_integer_new(svm,::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0])).size);
	}

	SVM_Variable function_size(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_size(svm,argc,argv);
	}

	SVM_Value instruction_char(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int ascii = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
		if((ascii<0) or (ascii>255))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid ASCII code");
		}
		std::string chaine(1,static_cast<unsigned char>(ascii));
		return ::svm_value_string_new__buffer(svm,chaine.c_str(),1);
	}

	SVM_Value instruction_index(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		long long int indice = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if((indice<0) or(indice>=static_cast<ssize_t>(chaine.size)))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid index");
		}
		unsigned char c = std::string(chaine.string,chaine.size).at(indice);
		return ::svm_value_integer_new(svm,c);
	}
	
	SVM_Value instruction_change(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_String chaine = ::svm_parameter_value_get(svm,argv[0]);
		SVM_String chaine_brute = ::svm_value_string_get(svm,chaine);
		long long int indice = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		long long int caractere = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
		if((indice<0) or(indice>=static_cast<ssize_t>(chaine_brute.size)))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid index");
		}
		if((caractere<0) or(caractere>=256))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid character");
		}
		const_cast<char&>(chaine_brute.string[indice])=static_cast<char>(caractere);
		::svm_value_string_set(svm,chaine,chaine_brute);
		return nullptr;
	}

	SVM_Value instruction_prepend(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_String chaine = ::svm_parameter_value_get(svm,argv[0]);
		SVM_String chaine_brute = ::svm_value_string_get(svm,chaine);
		long long int caractere = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if((caractere<0) or(caractere>=256))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid character");
		}
		std::string nouvelle_brute(chaine_brute.string,chaine_brute.size);
		nouvelle_brute.insert(nouvelle_brute.begin(),static_cast<char>(caractere));
		::svm_value_string_set(svm,chaine,::svm_string_new(svm,nouvelle_brute.c_str(),nouvelle_brute.size()));
		return nullptr;
	}

	SVM_Value instruction_append(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_String chaine = ::svm_parameter_value_get(svm,argv[0]);
		SVM_String chaine_brute = ::svm_value_string_get(svm,chaine);
		long long int caractere = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if((caractere<0) or(caractere>=256))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid character");
		}
		std::string nouvelle_brute(chaine_brute.string,chaine_brute.size);
		nouvelle_brute += static_cast<char>(caractere);
		::svm_value_string_set(svm,chaine,::svm_string_new(svm,nouvelle_brute.c_str(),nouvelle_brute.size()));
		return nullptr;
	}

	SVM_Value instruction_remove(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_String chaine = ::svm_parameter_value_get(svm,argv[0]);
		SVM_String chaine_brute = ::svm_value_string_get(svm,chaine);
		long long int indice = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if((indice<0) or(indice>=static_cast<ssize_t>(chaine_brute.size)))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid index");
		}
		std::string nouvelle_brute(chaine_brute.string,chaine_brute.size);
		nouvelle_brute.erase(nouvelle_brute.begin()+indice);
		::svm_value_string_set(svm,chaine,::svm_string_new(svm,nouvelle_brute.c_str(),nouvelle_brute.size()));
		return nullptr;
	}


	SVM_Value instruction_sub(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		long long int indice = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		long long int taille = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
		if(argc>3) { indice += chaine.size; }
		if(taille<0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid size");
		}
		long long int debut = indice;
		long long int fin = indice+taille;
		if(debut<0) { debut = 0; }
		if(fin<0) { fin = 0; }
		if(debut>static_cast<ssize_t>(chaine.size)) { debut=chaine.size; }
		if(fin>static_cast<ssize_t>(chaine.size)) { fin=chaine.size; }
		std::string c = std::string(chaine.string,chaine.size).substr(debut,fin-debut);
		return ::svm_value_string_new__buffer(svm,c.c_str(),c.size());
	}

	SVM_Variable function_sub(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_sub(svm,argc,argv);
	}

	SVM_Value instruction_cmp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		bool resultat = false;
		SVM_String gauche_brut = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		SVM_String droit_brut = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[2]));
		std::string gauche(gauche_brut.string,gauche_brut.size);
		std::string droit(droit_brut.string,droit_brut.size);
		SVM_String cmp = ::svm_parameter_marker_get(svm,argv[1]);
		std::string comparateur(cmp.string,cmp.size);
		if(comparateur == "=")
		{
			resultat = gauche==droit;
		}
		else if(comparateur == "<>")
		{
			resultat = gauche!=droit;
		}
		else if(comparateur == "<")
		{
			resultat = gauche<droit;
		}
		else if(comparateur == "<=")
		{
			resultat = gauche<=droit;
		}
		else if(comparateur == ">")
		{
			resultat = gauche>droit;
		}
		else if(comparateur == "=>")
		{
			resultat = gauche>=droit;
		}
		return ::svm_value_boolean_new(svm,resultat?TRUE:FALSE);
	}

	SVM_Variable function_cmp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_cmp(svm,argc,argv);
	}

	Regex* determine_regex(const void *svm, const SVM_Value valeur, Regex& locale)
	{
		if(::svm_value_type_is_string(svm,valeur))
		{
			SVM_String chaine = ::svm_value_string_get(svm,valeur);
			type_pattern_constant_locale(svm,chaine,locale);
			return &locale;
		}
		else
		{
			return reinterpret_cast<Regex*>(::svm_value_plugin_get_internal(svm,valeur));
		}
	}

	SVM_Value instruction_find(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String svm_chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		std::string chaine(svm_chaine.string,svm_chaine.size);
		Regex locale;
		Regex *regex = ::determine_regex(svm,::svm_parameter_value_get(svm,argv[1]),locale);
		long long int depart = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
		if((depart<0) or (depart>static_cast<ssize_t>(chaine.size())))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid start position");
		}
		auto debut = std::sregex_iterator(chaine.begin()+depart,chaine.end(),regex->_regex);
		auto fin = std::sregex_iterator();
		if(debut==fin)
		{
			return ::svm_value_boolean_new(svm,FALSE);
		}
		std::smatch m = *debut;
		::svm_value_integer_set(svm,::svm_parameter_value_get(svm,argv[2]),depart+m.position());
		if(argc>3)
		{
			::svm_value_integer_set(svm,::svm_parameter_value_get(svm,argv[3]),m.length());
		}
		return ::svm_value_boolean_new(svm,TRUE);
	}

	SVM_Value instruction_replace(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String svm_chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		Regex locale;
		Regex *regex = ::determine_regex(svm,::svm_parameter_value_get(svm,argv[2]),locale);
		SVM_String svm_nouveau = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[4]));
		std::string chaine(svm_chaine.string,svm_chaine.size);
		std::string nouveau(svm_nouveau.string,svm_nouveau.size);
		std::string resultat;
		SVM_Boolean tous = ::svm_parameter_type_is_keyword(svm,argv[1]);
		long long int occurence=0;
		if(tous==FALSE)
		{
			occurence = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
			if(occurence<1)
			{
				::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid replace index");
			}
		}
		long long int occ = 0;
		auto debut = std::sregex_iterator(chaine.begin(),chaine.end(),regex->_regex);
		auto fin = std::sregex_iterator();
		long long int curseur = 0;
		for(std::sregex_iterator i = debut ; i!=fin ; ++i)
		{
			std::smatch m = *i;
			resultat += chaine.substr(curseur,m.position()-curseur);
			++occ;
			if(tous or occ==occurence)
			{
				resultat += nouveau;
			}
			else
			{
				resultat += m.str();
			}
			curseur = m.position()+m.length();
		}
		resultat += chaine.substr(curseur);
		::svm_value_string_set__buffer(svm,::svm_parameter_value_get(svm,argv[0]),resultat.c_str(),resultat.size());
		return nullptr;
	}

	static SVM_Value sous_instruction_join_pointeur(const void *svm, SVM_Value_Pointer pointeur, const std::string& separateur="")
	{
		SVM_Address debut = ::svm_value_pointer_get_address(svm,pointeur);
		SVM_Size taille = ::svm_value_pointer_get_size(svm,pointeur);
		std::string resultat;
		for(SVM_Address i=debut ; i<(debut+taille) ; ++i)
		{
			if(i!=debut)
			{
				resultat += separateur;
			}
			SVM_Value_String ss = ::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),i,STRING);
			SVM_String s = ::svm_value_string_get(svm,ss);
			resultat += std::string(s.string,s.size);
		}
		return ::svm_value_string_new__buffer(svm,resultat.c_str(),resultat.size()); 
	}

	static SVM_Value sous_instruction_join_liste(const void *svm, SVM_Size debut, SVM_Size fin, SVM_Parameter argv[], const std::string& separateur="")
	{
		std::string resultat;
		for(SVM_Index i=debut ; i<fin ; ++i)
		{
			if(i!=debut)
			{
				resultat += separateur;
			}
			SVM_String s = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[i]));
			resultat += std::string(s.string,s.size);
		}
		return ::svm_value_string_new__buffer(svm,resultat.c_str(),resultat.size()); 
	}

	SVM_Value instruction_join(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		if((argc==1) and ::svm_value_type_is_pointer(svm,::svm_parameter_value_get(svm,argv[0])))
		{
			return sous_instruction_join_pointeur(svm,::svm_parameter_value_get(svm,argv[0]));
		}
		if((argc>1) and ::svm_parameter_type_is_marker(svm,argv[1]))
		{
			SVM_String svm_separateur = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
			std::string separateur(svm_separateur.string,svm_separateur.size);
			if(::svm_value_type_is_pointer(svm,::svm_parameter_value_get(svm,argv[2])))
			{
				return sous_instruction_join_pointeur(svm,::svm_parameter_value_get(svm,argv[2]),separateur);
			}
			return sous_instruction_join_liste(svm,2,argc-1,argv,separateur);
		}
		return sous_instruction_join_liste(svm,0,argc,argv);
	}

	SVM_Variable function_join(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_join(svm,argc,argv);
	}

	SVM_Value instruction_split(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String svm_chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		std::string chaine(svm_chaine.string,svm_chaine.size);
		Regex locale;
		Regex *regex = ::determine_regex(svm,::svm_parameter_value_get(svm,argv[1]),locale);
		auto debut = std::sregex_token_iterator(chaine.begin(),chaine.end(),regex->_regex,-1);
		auto fin = std::sregex_token_iterator();
		std::vector<std::string> jetons;
		for(std::sregex_token_iterator i = debut ; i!=fin ; ++i)
		{
			std::ssub_match m = *i;
			jetons.push_back(m);
		}
		SVM_Memory_Zone zone = ::svm_memory_zone_new(svm);
		::svm_memory_zone_append_internal__raw(svm,zone,STRING,jetons.size());
		SVM_Value_Pointer resultat = ::svm_memory_allocate(svm,::svm_kernel_get_current(svm),zone);
		SVM_Address adresse = ::svm_value_pointer_get_address(svm,resultat);
		for(auto& j: jetons)
		{
			::svm_memory_write_address(svm,::svm_kernel_get_current(svm),adresse++,::svm_value_string_new__buffer(svm,j.c_str(),j.size()));
		}
		return resultat;
	}

	SVM_Value instruction_regex(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String svm_chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		std::string chaine(svm_chaine.string,svm_chaine.size);
		Regex locale;
		Regex *regex = ::determine_regex(svm,::svm_parameter_value_get(svm,argv[1]),locale);
		auto debut = std::sregex_token_iterator(chaine.begin(),chaine.end(),regex->_regex,0);
		auto fin = std::sregex_token_iterator();
		std::vector<std::string> jetons;
		for(std::sregex_token_iterator i = debut ; i!=fin ; ++i)
		{
			std::ssub_match m = *i;
			jetons.push_back(m);
		}
		SVM_Memory_Zone zone = ::svm_memory_zone_new(svm);
		::svm_memory_zone_append_internal__raw(svm,zone,STRING,jetons.size());
		SVM_Value_Pointer resultat = ::svm_memory_allocate(svm,::svm_kernel_get_current(svm),zone);
		SVM_Address adresse = ::svm_value_pointer_get_address(svm,resultat);
		for(auto& j: jetons)
		{
			::svm_memory_write_address(svm,::svm_kernel_get_current(svm),adresse++,::svm_value_string_new__buffer(svm,j.c_str(),j.size()));
		}
		return resultat;
	}

	SVM_Value instruction_format(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		std::ostringstream oss;
		for(SVM_Index indice=0 ; indice<argc ; )
		{
			if(::svm_parameter_type_is_value(svm,argv[indice]))
			{
				SVM_String s = ::svm_value_print(svm,::svm_parameter_value_get(svm,argv[indice]));
				oss << std::string(s.string,s.size);
				++indice;
			}
			else
			{
				SVM_Index fin = ::svm_parameter_marker_find_closing(svm,argc,argv,indice);
				++indice;
				SVM_Value_PluginEntryPoint fonction = ::svm_parameter_value_get(svm,argv[indice]);
				++indice;
				SVM_Value vs = ::svm_function_call(svm,fonction,fin-indice,argv+indice);
				SVM_String s = ::svm_value_print(svm,vs);
				oss << std::string(s.string,s.size);
				indice=fin+1;
			}
		}
		return ::svm_value_string_new__buffer(svm,oss.str().c_str(),oss.str().size());
	}

	SVM_Value function_format(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_String s = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_Integer t = ::svm_parameter_value_get(svm,argv[1]);
		std::string a = "LEFT";
		if(argc>2)
		{
			SVM_String as = ::svm_parameter_keyword_get(svm,argv[2]);
			a = std::string(as.string,as.size);
		}
		SVM_String rs = ::svm_value_string_get(svm,s);
		long long int rt = ::svm_value_integer_get(svm,t);
		if(rt<0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Negative size requested");
		}
		SVM_Size rrt = static_cast<SVM_Size>(rt);
		std::istringstream iss;
		std::ostringstream oss;
		iss.str(std::string(rs.string,rs.size));
		while(iss.good())
		{
			std::string line;
			std::getline(iss,line);
			if(line.size()<rrt)
			{
				SVM_Size reste = rrt-line.size();
				if(a=="LEFT")
				{
					line += std::string(reste,' ');
				}
				else if(a=="CENTER")
				{
					SVM_Size avant = reste/2;
					SVM_Size apres = reste-avant;
					line = std::string(avant,' ')+line+std::string(apres,' ');
				}
				else
				{
					line = std::string(reste,' ')+line;
				}
			}
			else if(line.size()>rrt)
			{
				if(a=="RIGHT")
				{
					line = line.substr(line.size()-rrt,rrt);
				}
				else
				{
					line = line.substr(0,rrt);
				}
			}
			oss << line << '\n';
		}
		return ::svm_value_string_new__buffer(svm,oss.str().c_str(),oss.str().size()-1);
	}
}
