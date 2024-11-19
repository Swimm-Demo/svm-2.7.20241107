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
#include <math.h>
#include <list>
#include <string>
#include <memory>
#include <limits>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

#define TOLERANCE 10*std::numeric_limits<double>::epsilon() 

struct ValeurRPN
{
	ValeurRPN(const long long int entier)
	:_est_entier(true), _entier(entier), _reel(0) {}
	ValeurRPN(const double reel)
	:_est_entier(false), _entier(0), _reel(reel) {}
	double en_double() const { if(_est_entier) return _entier; else return _reel; }
	bool _est_entier;
	long long int _entier;
	double _reel;
};

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN math \n"
		"USE \n"
		"  TYPE real.number\n"
		"DEFINE \n"
		"  INSTRUCTION math.pi -> real.number \n"
		"  INSTRUCTION math.cos real.number -> real.number \n"
		"  INSTRUCTION math.sin real.number -> real.number \n"
		"  INSTRUCTION math.tan real.number -> real.number \n"
		"  INSTRUCTION math.acos real.number -> real.number \n"
		"  INSTRUCTION math.asin real.number -> real.number \n"
		"  INSTRUCTION math.atan real.number -> real.number \n"
		"  INSTRUCTION math.exp real.number -> real.number \n"
		"  INSTRUCTION math.ln real.number -> real.number \n"
		"  INSTRUCTION math.rpn ( [ INT real.number 'ADD' 'SUB' 'MUL' 'DIV' PEP ] | { PEP INT } ) + -> [ INT real.number ] \n"
		"  FUNCTION math.math_pi -> real.number \n"
		"  FUNCTION math.math_cos real.number -> real.number \n"
		"  FUNCTION math.math_sin real.number -> real.number \n"
		"  FUNCTION math.math_tan real.number -> real.number \n"
		"  FUNCTION math.math_acos real.number -> real.number \n"
		"  FUNCTION math.math_asin real.number -> real.number \n"
		"  FUNCTION math.math_atan real.number -> real.number \n"
		"  FUNCTION math.math_exp real.number -> real.number \n"
		"  FUNCTION math.math_ln real.number -> real.number \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	SVM_Value instruction_pi(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double *nombre = new double;
		*nombre = M_PI;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),nombre);
	}

	SVM_Value instruction_cos(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::cos(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_sin(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::sin(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_tan(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::tan(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_acos(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::acos(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_asin(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::asin(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_atan(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::atan(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_exp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::exp(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_ln(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		double nombre = *(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0]))));
		double *resultat = new double;
		*resultat = ::log(nombre);
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat);
	}

	SVM_Value instruction_rpn(const void *svm, SVM_Size argc, SVM_Parameter argv[])
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
				if(::svm_value_type_is_integer(svm,v) or ::svm_value_type_is_plugin(svm,v,::svm_value_pluginentrypoint_new__raw(svm,"real","number")))
				{
					pile.push_front(v);
					continue;
				}
				if(::svm_value_type_is_pluginentrypoint(svm,v))
				{
					SVM_Parameter *p = ::svm_parameter_array_new(svm,0);
					SVM_Variable resultat = ::svm_function_call(svm,::svm_value_pluginentrypoint_new_prefix(svm,v,"math"),0,p);
					if(::svm_variable_type_is_value(svm,resultat))
					{
						if(::svm_value_state_is_null(svm,resultat))
						{
							::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Null rpn value");
						}
						if(::svm_value_type_is_integer(svm,resultat) or ::svm_value_type_is_plugin(svm,resultat,::svm_value_pluginentrypoint_new__raw(svm,"real","number")))
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
				::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn value");
			}
			if(::svm_parameter_type_is_keyword(svm,argv[indice]))
			{
				if(pile.size()<2)
				{
					::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Invalid rpn expression");
				}
				SVM_Value droite = pile.front();
				pile.pop_front();
				SVM_Value gauche = pile.front();
				pile.pop_front();
				std::shared_ptr<ValeurRPN> v_gauche;
				std::shared_ptr<ValeurRPN> v_droite;
				std::shared_ptr<ValeurRPN> v_resultat;
				if(::svm_value_type_is_integer(svm,gauche))
				{
					v_gauche = std::make_shared<ValeurRPN>(::svm_value_integer_get(svm,gauche));
				}
				else
				{
					v_gauche = std::make_shared<ValeurRPN>(*(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,gauche))));
				}
				if(::svm_value_type_is_integer(svm,droite))
				{
					v_droite = std::make_shared<ValeurRPN>(::svm_value_integer_get(svm,droite));
				}
				else
				{
					v_droite = std::make_shared<ValeurRPN>(*(reinterpret_cast<double*>(::svm_value_plugin_get_internal(svm,droite))));
				}
				SVM_String operation = ::svm_parameter_keyword_get(svm,argv[indice]);
				std::string op(operation.string,operation.size);
				if(op=="ADD")
				{
					if(v_gauche->_est_entier and v_droite->_est_entier)
					{
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->_entier+v_droite->_entier);
					}
					else
					{
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->en_double()+v_droite->en_double());
					}
				}
				else if(op=="SUB")
				{
					if(v_gauche->_est_entier and v_droite->_est_entier)
					{
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->_entier-v_droite->_entier);
					}
					else
					{
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->en_double()-v_droite->en_double());
					}
				}
				else if(op=="MUL")
				{
					if(v_gauche->_est_entier and v_droite->_est_entier)
					{
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->_entier*v_droite->_entier);
					}
					else
					{
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->en_double()*v_droite->en_double());
					}
				}
				else if(op=="DIV")
				{
					if(v_gauche->_est_entier and v_droite->_est_entier)
					{
						if(v_droite->_entier==0)
						{
							::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Division by zero");
						}
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->_entier/v_droite->_entier);
					}
					else
					{
						if((v_droite->en_double()>-TOLERANCE) and (v_droite->en_double()<TOLERANCE))
						{
							::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Division by zero");
						}
						v_resultat = std::make_shared<ValeurRPN>(v_gauche->en_double()/v_droite->en_double());
					}
				}
				if(v_resultat->_est_entier)
				{
					pile.push_front(::svm_value_integer_new(svm,v_resultat->_entier));
				}
				else
				{
					double *resultat = new double;
					*resultat = v_resultat->_reel;
					pile.push_front(::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"real","number"),resultat));
				}
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
				SVM_Variable resultat = ::svm_function_call(svm,::svm_value_pluginentrypoint_new_prefix(svm,v_fonction,"math"),nb_params,parametres);
				if(::svm_variable_type_is_value(svm,resultat))
				{
					if(::svm_value_state_is_null(svm,resultat))
					{
						::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Null rpn value");
					}
					if(::svm_value_type_is_integer(svm,resultat) or ::svm_value_type_is_plugin(svm,resultat,::svm_value_pluginentrypoint_new__raw(svm,"real","number")))
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

	SVM_Value function_math_pi(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_pi(svm,argc,argv);
	}

	SVM_Value function_math_cos(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_cos(svm,argc,argv);
	}

	SVM_Value function_math_sin(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_sin(svm,argc,argv);
	}

	SVM_Value function_math_tan(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_tan(svm,argc,argv);
	}

	SVM_Value function_math_acos(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_acos(svm,argc,argv);
	}

	SVM_Value function_math_asin(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_asin(svm,argc,argv);
	}

	SVM_Value function_math_atan(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_atan(svm,argc,argv);
	}

	SVM_Value function_math_exp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_exp(svm,argc,argv);
	}

	SVM_Value function_math_ln(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_ln(svm,argc,argv);
	}
}
