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

#include <string>
#include <list>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN bool \n"
		"DEFINE \n"
		"  INSTRUCTION bool.not BLN -> BLN \n"
		"  INSTRUCTION bool.and ( BLN * | PTR ) -> BLN \n"
		"  INSTRUCTION bool.or ( BLN * | PTR ) -> BLN \n"
		"  INSTRUCTION bool.xor BLN BLN -> BLN \n"
		"  INSTRUCTION bool.nand ( BLN * | PTR ) -> BLN \n"
		"  INSTRUCTION bool.nor ( BLN * | PTR ) -> BLN \n"
		"  INSTRUCTION bool.equal BLN BLN -> BLN \n"
		"  INSTRUCTION bool.rpn ( [ BLN 'NOT' 'AND' 'OR' 'XOR' 'NAND' 'NOR' 'EQUAL' ] | { PEP INT } ) + -> BLN \n"
		"  FUNCTION bool.not BLN -> BLN \n"
		"  FUNCTION bool.and ( BLN * | PTR ) -> BLN \n"
		"  FUNCTION bool.or ( BLN * | PTR ) -> BLN \n"
		"  FUNCTION bool.xor BLN BLN -> BLN \n"
		"  FUNCTION bool.nand ( BLN * | PTR ) -> BLN \n"
		"  FUNCTION bool.nor ( BLN * | PTR ) -> BLN \n"
		"  FUNCTION bool.equal BLN BLN -> BLN \n"
		"  FUNCTION bool.format BLN INT:size [ 'LEFT' 'CENTER' 'RIGHT' ] ? ( [ 'UPPER' 'LOWER' 'CAMEL' 'LETTER' 'NUM' ] | STR:true STR:false ) -> STR \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	SVM_Value instruction_not(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Boolean b = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[0]));
		return ::svm_value_boolean_new__raw(svm,b!=TRUE);
	}

	SVM_Variable function_not(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_not(svm,argc,argv);
	}

	SVM_Value instruction_and(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		bool resultat = true;
		if(::svm_value_type_is_pointer(svm,::svm_parameter_value_get(svm,argv[0])))
		{
			SVM_Value_Pointer pointeur = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address adresse = ::svm_value_pointer_get_address(svm,pointeur);
			SVM_Size taille = ::svm_value_pointer_get_size(svm,pointeur);
			for(SVM_Address a = adresse ; a<(adresse+taille) ; ++a)
			{
				SVM_Boolean b = ::svm_value_boolean_get(svm,::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),a,BOOLEAN));
				if(b==FALSE)
				{
					resultat = false;
					break;
				}

			}
		}
		else
		{
			for(SVM_Index i=0 ; i<argc ; ++i)
			{
				SVM_Boolean b = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[i]));
				if(b==FALSE)
				{
					resultat = false;
					break;
				}
			}
		}
		return ::svm_value_boolean_new__raw(svm,resultat);
	}

	SVM_Variable function_and(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_and(svm,argc,argv);
	}

	SVM_Value instruction_or(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		bool resultat = false;
		if(::svm_value_type_is_pointer(svm,::svm_parameter_value_get(svm,argv[0])))
		{
			SVM_Value_Pointer pointeur = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address adresse = ::svm_value_pointer_get_address(svm,pointeur);
			SVM_Size taille = ::svm_value_pointer_get_size(svm,pointeur);
			for(SVM_Address a = adresse ; a<(adresse+taille) ; ++a)
			{
				SVM_Boolean b = ::svm_value_boolean_get(svm,::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),a,BOOLEAN));
				if(b==TRUE)
				{
					resultat = true;
					break;
				}
			}
		}
		else
		{
			for(SVM_Index i=0 ; i<argc ; ++i)
			{
				SVM_Boolean b = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[i]));
				if(b==TRUE)
				{
					resultat = true;
					break;
				}
			}
		}
		return ::svm_value_boolean_new__raw(svm,resultat);
	}

	SVM_Variable function_or(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_or(svm,argc,argv);
	}

	SVM_Value instruction_xor(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Boolean gauche = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[0]));
		SVM_Boolean droit = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[1]));
		return ::svm_value_boolean_new__raw(svm, (gauche==TRUE) xor (droit==TRUE));
	}

	SVM_Variable function_xor(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_xor(svm,argc,argv);
	}

	SVM_Value instruction_nand(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Boolean b = ::instruction_and(svm,argc,argv);
		return ::svm_value_boolean_new__raw(svm,::svm_value_boolean_get(svm,b)!=TRUE);
	}

	SVM_Variable function_nand(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_nand(svm,argc,argv);
	}

	SVM_Value instruction_nor(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Boolean b = ::instruction_or(svm,argc,argv);
		return ::svm_value_boolean_new__raw(svm,::svm_value_boolean_get(svm,b)!=TRUE);
	}

	SVM_Variable function_nor(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_nor(svm,argc,argv);
	}

	SVM_Value instruction_equal(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Boolean gauche = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[0]));
		SVM_Boolean droit = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[1]));
		return ::svm_value_boolean_new__raw(svm, (gauche==TRUE) == (droit==TRUE));
	}

	SVM_Variable function_equal(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_equal(svm,argc,argv);
	}

	SVM_Value instruction_rpn(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		std::list<SVM_Value> pile;
		for(SVM_Index indice = 0 ; indice<argc ; ++indice)
		{
			if(::svm_parameter_type_is_value(svm,argv[indice]))
			{
				SVM_Value v = ::svm_parameter_value_get(svm,argv[indice]);
				if(::svm_value_state_is_null(svm,v))
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Null rpn value");
				}
				if(::svm_value_type_is_boolean(svm,v))
				{
					pile.push_front(v);
					continue;
				}
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn value");
			}
			if(::svm_parameter_type_is_keyword(svm,argv[indice]))
			{
				SVM_String operation = ::svm_parameter_keyword_get(svm,argv[indice]);
				std::string op(operation.string,operation.size);
				if(pile.size()<1)
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn expression");
				}
				SVM_Value droite = pile.front();
				pile.pop_front();
				if(op == "NOT")
				{
					pile.push_front(::svm_value_boolean_new__raw(svm,::svm_value_boolean_get(svm,droite)!=TRUE));
					continue;
				}
				if(pile.size()<1)
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn expression");
				}
				SVM_Value gauche = pile.front();
				pile.pop_front();
				bool v_gauche = ::svm_value_boolean_get(svm,gauche)==TRUE;
				bool v_droite = ::svm_value_boolean_get(svm,droite)==TRUE;
				bool v_resultat = false;
				if(op=="AND")
				{
					v_resultat = v_gauche and v_droite;
				}
				else if(op=="OR")
				{
					v_resultat = v_gauche or v_droite;
				}
				else if(op=="XOR")
				{
					v_resultat = v_gauche xor v_droite;
				}
				else if(op=="NAND")
				{
					v_resultat = not (v_gauche and v_droite);
				}
				else if(op=="NOR")
				{
					v_resultat = not (v_gauche or v_droite);
				}
				else if(op=="EQUAL")
				{
					v_resultat = (v_gauche == v_droite);
				}
				pile.push_front(::svm_value_boolean_new__raw(svm,v_resultat));
				continue;
			}
			if(::svm_parameter_type_is_marker(svm,argv[indice]))
			{
				++indice;
				SVM_Value_PluginEntryPoint v_fonction = ::svm_parameter_value_get(svm,argv[indice]);
				++indice;
				SVM_Value_Integer v_nb_params = ::svm_parameter_value_get(svm,argv[indice]);
				++indice;
				long long int nb_params = ::svm_value_integer_get(svm,v_nb_params);
				if(nb_params<0)
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Negative number of parameters");
				}
				if(static_cast<ssize_t>(pile.size())<nb_params)
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid number of parameters");
				}
				SVM_Parameter *parametres = ::svm_parameter_array_new(svm,nb_params);
				for(ssize_t i=0 ; i<nb_params  ; ++i)
				{
					parametres[nb_params-1-i] = ::svm_parameter_value_new(svm,pile.front());
					pile.pop_front();
				}
				SVM_Variable resultat = ::svm_function_call(svm,::svm_value_pluginentrypoint_new_prefix(svm,v_fonction,"bool"),nb_params,parametres);
				if(::svm_variable_type_is_value(svm,resultat))
				{
					if(::svm_value_state_is_null(svm,resultat))
					{
						::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Null rpn value");
					}
					if(::svm_value_type_is_boolean(svm,resultat))
					{
						pile.push_front(resultat);
						continue;
					}
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn value");
				}
				else
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid return from function");
				}
			}
		}
		if(pile.size()!=1)
		{
			::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn expression");
		}
		return pile.front();
	}
	
	SVM_Value function_format(const void *svm, const SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Boolean b = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[0]));
		long long int taille = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if(taille<0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Negative size requested");
		}
		SVM_Size t = static_cast<SVM_Size>(taille);
		std::string a = "LEFT";
		SVM_Index indice=2;
		if(::svm_parameter_type_is_keyword(svm,argv[2]))
		{
			SVM_String s = ::svm_parameter_keyword_get(svm,argv[2]);
			std::string sa(s.string,s.size);
			if(sa=="LEFT" or sa=="CENTER" or sa=="RIGHT")
			{
				a=sa;
				++indice;
			}
		}
		std::string vrai;
		std::string faux;
		if(::svm_parameter_type_is_keyword(svm,argv[indice]))
		{
			SVM_String s = ::svm_parameter_keyword_get(svm,argv[indice]);
			std::string sf(s.string,s.size);
			if(sf=="UPPER") { vrai = "TRUE"; faux = "FALSE"; }
			else if(sf=="LOWER") { vrai = "true"; faux = "false"; }
			else if(sf=="CAMEL") { vrai = "True"; faux = "False"; }
			else if(sf=="LETTER") { vrai = "T"; faux = "F"; }
			else { vrai = "1"; faux = "0"; }
		}
		else
		{
			SVM_String v = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[indice]));
			SVM_String f = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[indice+1]));
			vrai = std::string(v.string,v.size);
			faux = std::string(f.string,f.size);
		}
		std::string resultat = b?vrai:faux;
		if(resultat.size()<t)
		{
			SVM_Size reste = t-resultat.size();
			if(a=="LEFT")
			{
				resultat += std::string(reste,' ');
			}
			else if(a=="CENTER")
			{
				SVM_Size avant = reste/2;
				SVM_Size apres = reste-avant;
				resultat = std::string(avant,' ')+resultat+std::string(apres,' ');
			}
			else
			{
				resultat = std::string(reste,' ')+resultat;
			}
		}
		else if(resultat.size()>t)
		{
			if(a=="RIGHT")
			{
				resultat = resultat.substr(t-resultat.size(),t);
			}
			else
			{
				resultat = resultat.substr(0,t);
			}
		}
		return ::svm_value_string_new__buffer(svm,resultat.c_str(),resultat.size());
	}
}
