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
#include <stdio.h>
#include <time.h>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN int \n"
		"DEFINE \n"
		"  INSTRUCTION int.rand 'DICE' ? INT:limit -> INT \n"
		"  INSTRUCTION int.add ( INT INT + | PTR ) -> INT \n"
		"  INSTRUCTION int.sub INT INT -> INT \n"
		"  INSTRUCTION int.mul ( INT INT + | PTR ) -> INT \n"
		"  INSTRUCTION int.div INT INT -> INT \n"
		"  INSTRUCTION int.mod INT INT -> INT \n"
		"  INSTRUCTION int.gcd ( INT INT + | PTR ) -> INT \n"
		"  INSTRUCTION int.lcm ( INT INT + | PTR ) -> INT \n"
		"  INSTRUCTION int.cmp INT [ < > = <> <= => ] INT -> BLN \n"
		"  INSTRUCTION int.parse STR -> INT \n"
		"  INSTRUCTION int.print INT ( [ 'OCT' 'DEC' 'HEX' ] INT ) ? -> STR \n"
		"  FUNCTION int.add ( INT INT + | PTR ) -> INT \n"
		"  FUNCTION int.sub INT INT -> INT \n"
		"  FUNCTION int.mul ( INT INT + | PTR ) -> INT \n"
		"  FUNCTION int.div INT INT -> INT \n"
		"  FUNCTION int.mod INT INT -> INT \n"
		"  FUNCTION int.gcd ( INT INT + | PTR ) -> INT \n"
		"  FUNCTION int.lcm ( INT INT + | PTR ) -> INT \n"
		"  FUNCTION int.cmp INT [ < > = <> <= => ] INT -> BLN \n"
		"  FUNCTION int.parse STR -> INT \n"
		"  FUNCTION int.print INT ( [ 'OCT' 'DEC' 'HEX' ] INT ) ? -> STR \n"
		"  FUNCTION int.format INT INT:size STR:padding [ 'OCT' 'DEC' 'HEX' ] ? -> STR \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
		::srand(::time(nullptr));
	}

	SVM_Value instruction_rand(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int depart = 0;
		if(::svm_parameter_type_is_keyword(svm,argv[0]))
		{
			++depart;
		}
		long long int limite = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[depart]));
		return ::svm_value_integer_new(svm,depart+::rand()%limite);
	}

	SVM_Value instruction_add(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int somme = 0;
		if(argc==1)
		{
			SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address debut = ::svm_value_pointer_get_address(svm,p);
			SVM_Address fin = debut + ::svm_value_pointer_get_size(svm,p);
			for(SVM_Address a = debut ; a<fin ; ++a)
			{
				SVM_Value_Integer i = ::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),a,INTEGER);
				somme += ::svm_value_integer_get(svm,i);
			}
			
		}
		else
		{
			for(SVM_Index i = 0 ; i<argc ; ++i)
			{
				SVM_Value_Integer vi = ::svm_parameter_value_get(svm,argv[i]);
				somme += ::svm_value_integer_get(svm,vi);
			}
		}
		return ::svm_value_integer_new(svm,somme);
	}

	SVM_Variable function_add(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_add(svm,argc,argv);
	}

	SVM_Value instruction_sub(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Integer gauche = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_Integer droit = ::svm_parameter_value_get(svm,argv[1]);
		return ::svm_value_integer_new(svm,::svm_value_integer_get(svm,gauche)-::svm_value_integer_get(svm,droit));
	}

	SVM_Variable function_sub(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_sub(svm,argc,argv);
	}

	SVM_Value instruction_mul(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int produit = 1;
		if(argc==1)
		{
			SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address debut = ::svm_value_pointer_get_address(svm,p);
			SVM_Address fin = debut + ::svm_value_pointer_get_size(svm,p);
			for(SVM_Address a = debut ; a<fin ; ++a)
			{
				SVM_Value_Integer i = ::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),a,INTEGER);
				produit *= ::svm_value_integer_get(svm,i);
			}
			
		}
		else
		{
			for(SVM_Index i = 0 ; i<argc ; ++i)
			{
				SVM_Value_Integer vi = ::svm_parameter_value_get(svm,argv[i]);
				produit *= ::svm_value_integer_get(svm,vi);
			}
		}
		return ::svm_value_integer_new(svm,produit);
	}

	SVM_Variable function_mul(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_mul(svm,argc,argv);
	}

	SVM_Value instruction_div(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Integer gauche = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_Integer droit = ::svm_parameter_value_get(svm,argv[1]);
		if(::svm_value_integer_get(svm,droit)==0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Division by zero.");
		}
		return ::svm_value_integer_new(svm,::svm_value_integer_get(svm,gauche)/::svm_value_integer_get(svm,droit));
	}

	SVM_Variable function_div(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_div(svm,argc,argv);
	}

	SVM_Value instruction_mod(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Integer gauche = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_Integer droit = ::svm_parameter_value_get(svm,argv[1]);
		if(::svm_value_integer_get(svm,droit)==0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Modulus by zero.");
		}
		return ::svm_value_integer_new(svm,::svm_value_integer_get(svm,gauche) % ::svm_value_integer_get(svm,droit));
	}

	SVM_Variable function_mod(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_mod(svm,argc,argv);
	}

	long long int calcul_pgcd(long long int a, long long int b)
	{
		if(a<0) a = -a;
		if(b<0) b = -b;
		if(a==0) return b;
		if(b==0) return a;
		for( ; ; )
		{
			if(a==b) return a;
			if(a>b)
				a=a-b;
			else
				b=b-a;
		}
	}

	SVM_Value instruction_gcd(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int pgcd = 0;
		if(argc==1)
		{
			SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address debut = ::svm_value_pointer_get_address(svm,p);
			SVM_Address fin = debut + ::svm_value_pointer_get_size(svm,p);
			for(SVM_Address a = debut ; a<fin ; ++a)
			{
				SVM_Value_Integer i = ::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),a,INTEGER);
				pgcd = calcul_pgcd(pgcd,::svm_value_integer_get(svm,i));
			}
			
		}
		else
		{
			for(SVM_Index i = 0 ; i<argc ; ++i)
			{
				SVM_Value_Integer vi = ::svm_parameter_value_get(svm,argv[i]);
				pgcd = calcul_pgcd(pgcd,::svm_value_integer_get(svm,vi));
			}
		}
		return ::svm_value_integer_new(svm,pgcd);
	}

	SVM_Variable function_gcd(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_gcd(svm,argc,argv);
	}

	SVM_Value instruction_lcm(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int ppcm = 1;
		if(argc==1)
		{
			SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
			SVM_Address debut = ::svm_value_pointer_get_address(svm,p);
			SVM_Address fin = debut + ::svm_value_pointer_get_size(svm,p);
			for(SVM_Address a = debut ; a<fin ; ++a)
			{
				SVM_Value_Integer i = ::svm_memory_read_address_type_internal(svm,::svm_kernel_get_current(svm),a,INTEGER);
				auto ri =::svm_value_integer_get(svm,i); 
				if(ri==0)
				{
					ppcm = 0;
					break;
				}
				ppcm *= ri/calcul_pgcd(ppcm,ri);
			}
			
		}
		else
		{
			for(SVM_Index i = 0 ; i<argc ; ++i)
			{
				SVM_Value_Integer vi = ::svm_parameter_value_get(svm,argv[i]);
				auto ri =::svm_value_integer_get(svm,vi); 
				if(ri==0)
				{
					ppcm = 0;
					break;
				}
				if(ri<0) ri=-ri;
				ppcm *= ri/calcul_pgcd(ppcm,ri);
			}
		}
		return ::svm_value_integer_new(svm,ppcm);
	}

	SVM_Variable function_lcm(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_lcm(svm,argc,argv);
	}

	SVM_Value instruction_cmp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		bool resultat = false;
		long long int gauche = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
		long long int droit = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
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

	SVM_Value instruction_parse(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String chaine = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		char *fin;
		long int entier = ::strtol(chaine.string,&fin,0);
		if((static_cast<size_t>(fin-chaine.string)!=chaine.size))
		{
			::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Input is not an integer number");
		}
		return ::svm_value_integer_new(svm,entier);
	}

	SVM_Variable function_parse(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_parse(svm,argc,argv);
	}

	SVM_Value instruction_print(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int entier = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
		std::ostringstream oss;
		if(argc>1)
		{
			SVM_String valeur_base = ::svm_parameter_keyword_get(svm,argv[1]);
			std::string base(valeur_base.string);
			if(base == "OCT")
			{
				oss << "0" << std::oct;
			}
			if(base == "HEX")
			{
				oss << "0x" << std::hex;
			}
			long long int longueur = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
			if(longueur<=0)
			{
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Size is not scrictly positive.");
			}
			oss << std::setw(longueur) << std::setfill('0');
		}
		oss << entier;
		return ::svm_value_string_new__raw(svm,oss.str().c_str());
	}

	SVM_Variable function_print(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return ::instruction_print(svm,argc,argv);
	}

	SVM_Variable function_format(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		std::ostringstream oss;
		long long int entier = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
		long long int taille = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if(taille<0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Negative size requested");
		}
		SVM_Size t = static_cast<SVM_Size>(taille);
		SVM_String remplissage = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[2]));
		if(remplissage.size==0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Empty padding");
		}
		std::string base = "DEC";
		if(argc>3)
		{
			SVM_String b = ::svm_parameter_keyword_get(svm,argv[3]);
			base = std::string(b.string,b.size);
		}
		if(base == "OCT")
		{
			oss << std::oct;
		}
		if(base == "HEX")
		{
			oss << std::hex;
		}
		oss << entier;
		std::string resultat = oss.str();
		if(resultat.size()<t)
		{
			SVM_Size fois = (t-resultat.size())/remplissage.size;
			std::string r;
			for(SVM_Index i=0 ; i<fois ; ++i)
			{
				r += std::string(remplissage.string,remplissage.size);
			}
			SVM_Size reste = (t-resultat.size())%remplissage.size;
			r += std::string(remplissage.string,reste);
			resultat = r+resultat;
		}
		else if(resultat.size()>t)
		{
			resultat = resultat.substr(resultat.size()-t,t);
		}
		return ::svm_value_string_new__buffer(svm,resultat.c_str(),resultat.size());
	}
}
