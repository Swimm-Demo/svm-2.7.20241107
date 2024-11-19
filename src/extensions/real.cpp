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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

#define TOLERANCE 10*std::numeric_limits<double>::epsilon()

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN real \n"
		"DEFINE \n"
		"  TYPE real.number \n"
		"  FUNCTION real.parse STR -> real.number \n"
		"  INSTRUCTION real.add ( [ INT real.number ] [ INT real.number ] + | PTR ) -> real.number \n"
		"  INSTRUCTION real.sub [ INT real.number ] [ INT real.number ] -> real.number \n"
		"  INSTRUCTION real.mul ( [ INT real.number ] [ INT real.number ] + | PTR ) -> real.number \n"
		"  INSTRUCTION real.div [ INT real.number ] [ INT real.number ] -> real.number \n"
		"  INSTRUCTION real.cmp [ INT real.number ] [ < > = <> <= => ] [ INT real.number ] -> BLN \n"
		"  INSTRUCTION real.parse STR -> real.number \n"
		"  INSTRUCTION real.print real.number ( INT INT ) ? -> STR \n"
		"  FUNCTION real.format real.number INT:size INT:mantiss ? -> STR \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	void type_number_delete(const void *svm, void *objet)
	{
		double *nombre = reinterpret_cast<double*>(objet);
		delete nombre;
	}

	void* type_number_copy(const void *svm, void *objet)
	{
		double *nombre = reinterpret_cast<double*>(objet);
		double *copie = new double;
		*copie = *nombre;
		return copie;
	}

	unsigned char type_number_compare(const void *svm, const void *gauche, const void *droit)
	{
		const double *g = reinterpret_cast<const double*>(gauche);
		const double *d = reinterpret_cast<const double*>(droit);
		if(*g<*d) return ORDER_INFERIOR;
		if(*g>*d) return ORDER_SUPERIOR;
		return ORDER_EQUAL;
	}

	void *type_number_constant(const void *svm, const SVM_String constante)
	{
		std::string chaine(constante.string,constante.size);
		size_t fin;
		try
		{
			double analyse = std::stod(chaine,&fin);
			if(fin!=chaine.size())
			{
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Input is not a real number");
			}
			double *nombre = new double;
			*nombre = analyse;
			return nombre;
		}
		catch(...)
		{
			::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Input is not a real number");
			return nullptr;
		}
	}

	SVM_String type_number_print(const void *svm, void *objet)
	{
		double *nombre = reinterpret_cast<double*>(objet);
		std::ostringstream oss;
		oss << std::setprecision(std::numeric_limits<double>::digits10) << (*nombre);
		return ::svm_string_new__raw(svm,oss.str().c_str());
	}
	
	SVM_Value instruction_add(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double somme = 0;
		if(argc==1)
		{
			SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address debut = ::svm_value_pointer_get_address(svm,p);
			SVM_Address fin = debut + ::svm_value_pointer_get_size(svm,p);
			SVM_Value_PluginEntryPoint pep = ::svm_value_pluginentrypoint_new__raw(svm,"real","number");
			for(SVM_Address a = debut ; a<fin ; ++a)
			{
				SVM_Value v = ::svm_memory_read_address(svm,::svm_kernel_get_current(svm),a);
				if(::svm_value_type_is_integer(svm,v))
				{
					somme += ::svm_value_integer_get(svm,v);
				}
				else if(::svm_value_type_is_plugin(svm,v,pep))
				{
					double *vv = reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,v));
					somme += *vv;
				}
				else
				{
					::svm_processor_current_raise_error_internal__raw(svm,MEMORY,"Invalid type");
				}
			}
		}
		else
		{
			for(SVM_Index i = 0 ; i<argc ; ++i)
			{
				SVM_Value v = ::svm_parameter_value_get(svm,argv[i]);
				if(::svm_value_type_is_integer(svm,v))
				{
					somme += ::svm_value_integer_get(svm,v);
				}
				else
				{
					double *vv = reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,v));
					somme += *vv;
				}
			}
		}
		double *resultat = new double;
		*resultat = somme;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_sub(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double difference;
		SVM_Value gauche = ::svm_parameter_value_get(svm,argv[0]);
		if(::svm_value_type_is_integer(svm,gauche))
		{
			difference = ::svm_value_integer_get(svm,gauche);
		}
		else
		{
			difference = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,gauche)));
		}
		SVM_Value droit = ::svm_parameter_value_get(svm,argv[1]);
		if(::svm_value_type_is_integer(svm,droit))
		{
			difference -= ::svm_value_integer_get(svm,droit);
		}
		else
		{
			difference -= *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,droit)));
		}
		double *resultat = new double;
		*resultat = difference;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_mul(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double produit = 1;
		if(argc==1)
		{
			SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address debut = ::svm_value_pointer_get_address(svm,p);
			SVM_Address fin = debut + ::svm_value_pointer_get_size(svm,p);
			SVM_Value_PluginEntryPoint pep = ::svm_value_pluginentrypoint_new__raw(svm,"real","number");
			for(SVM_Address a = debut ; a<fin ; ++a)
			{
				SVM_Value v = ::svm_memory_read_address(svm,::svm_kernel_get_current(svm),a);
				if(::svm_value_type_is_integer(svm,v))
				{
					produit *= ::svm_value_integer_get(svm,v);
				}
				else if(::svm_value_type_is_plugin(svm,v,pep))
				{
					double *vv = reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,v));
					produit *= *vv;
				}
				else
				{
					::svm_processor_current_raise_error_internal__raw(svm,MEMORY,"Invalid type");
				}
			}
		}
		else
		{
			for(SVM_Index i = 0 ; i<argc ; ++i)
			{
				SVM_Value v = ::svm_parameter_value_get(svm,argv[i]);
				if(::svm_value_type_is_integer(svm,v))
				{
					produit *= ::svm_value_integer_get(svm,v);
				}
				else
				{
					double *vv = reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,v));
					produit *= *vv;
				}
			}
		}
		double *resultat = new double;
		*resultat = produit;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_div(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double quotient;
		SVM_Value gauche = ::svm_parameter_value_get(svm,argv[0]);
		if(::svm_value_type_is_integer(svm,gauche))
		{
			quotient = ::svm_value_integer_get(svm,gauche);
		}
		else
		{
			quotient = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,gauche)));
		}
		SVM_Value droit = ::svm_parameter_value_get(svm,argv[1]);
		if(::svm_value_type_is_integer(svm,droit))
		{
			long long int diviseur = ::svm_value_integer_get(svm,droit);
			if(diviseur==0)
			{
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Division by zero.");
			}
			quotient /= diviseur;
		}
		else
		{
			double diviseur = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,droit)));
			if((diviseur>-TOLERANCE) and (diviseur<TOLERANCE))
			{
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Division by zero.");
			}
			quotient /= diviseur;
		}
		double *resultat = new double;
		*resultat = quotient;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_cmp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		bool resultat = false;
		SVM_Value valeur_gauche = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value valeur_droite = ::svm_parameter_value_get(svm,argv[2]);
		double gauche;
		double droite;
		if(::svm_value_type_is_integer(svm,valeur_gauche))
		{
			gauche = ::svm_value_integer_get(svm,valeur_gauche);
		}
		else
		{
			gauche = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,valeur_gauche)));
		}
		if(::svm_value_type_is_integer(svm,valeur_droite))
		{
			droite = ::svm_value_integer_get(svm,valeur_droite);
		}
		else
		{
			droite = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,valeur_droite)));
		}
		SVM_String cmp = ::svm_parameter_marker_get(svm,argv[1]);
		std::string comparateur(cmp.string,cmp.size);
		if(comparateur == "=")
		{
			resultat = ((gauche-droite)>-TOLERANCE) and ((gauche-droite)<TOLERANCE);
		}
		else if(comparateur == "<>")
		{
			resultat = not (((gauche-droite)>-TOLERANCE) and ((gauche-droite)<TOLERANCE));
		}
		else if(comparateur == "<")
		{
			resultat = gauche<droite;
		}
		else if(comparateur == "<=")
		{
			resultat = gauche<=droite;
		}
		else if(comparateur == ">")
		{
			resultat = gauche>droite;
		}
		else if(comparateur == "=>")
		{
			resultat = gauche>=droite;
		}
		return ::svm_value_boolean_new(svm,resultat?TRUE:FALSE);
	}

	SVM_Value instruction_parse(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String valeur_chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),::type_number_constant(svm,valeur_chaine));
	}

	SVM_Value instruction_print(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double reel = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		std::ostringstream oss;
		long long int longueur = -1;
		if(argc>1)
		{
			longueur = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
			if(longueur<=0)
			{
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Size is not scrictly positive.");
			}
			oss << std::setw(longueur);
		}
		if(argc>2)
		{
			long long int precision = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
			if(precision<0)
			{
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Precision is not positive.");
			}
			oss << std::setprecision(precision);
		}
		oss << reel;
		auto resultat = oss.str();
		if((longueur>0) and (resultat.size()>static_cast<size_t>(longueur)))
		{
			resultat = resultat.substr(resultat.size()-longueur,longueur);
		}
		return ::svm_value_string_new__buffer(svm,resultat.c_str(),resultat.size());
	}

	SVM_Value function_format(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_print(svm,argc,argv);
	}

	SVM_Value function_parse(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_parse(svm,argc,argv);
	}
}
