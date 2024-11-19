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
#include <string>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN type \n"
		"DEFINE \n"
		"  TYPE type.type \n"
		"  INSTRUCTION type.new ( [ 'AUTO' 'INT' 'STR' 'BLN' 'PTR' 'LIB' 'SYM' 'PEP' 'IRQ' ] | PEP ) -> type.type \n"
		"  INSTRUCTION type.cmp type.type [ = <> < > <= => ] type.type -> BLN \n"
		"  INSTRUCTION type.extract PTR -> PTR \n"
		"  INSTRUCTION type.memory 'GLOBAL' ? ( ( type.type INT ? ) + | PTR ) -> PTR \n"
		"  FUNCTION type.new VARIABLE -> type.type \n"
		"  FUNCTION type.get type.type -> VARIABLE \n"
		"  FUNCTION type.cmp type.type [ = <> < > <= => ] type.type -> BLN \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	struct type_type
	{
		SVM_Type _type;
	};

	void type_type_delete(const void *svm, void *objet)
	{
		type_type *t = reinterpret_cast<type_type*>(objet);
		::svm_variable_scope_set_local(svm,t->_type);
		delete t;
	}

	void* type_type_copy(const void *svm, void *objet)
	{
		type_type *t = reinterpret_cast<type_type*>(objet);
		type_type *c = new type_type;
		c->_type = ::svm_type_copy(svm,t->_type);
		::svm_variable_scope_set_global(svm,c->_type);
		return c;
	}

	unsigned char type_type_compare(const void *svm, const void *gauche, const void *droit)
	{
		const type_type *g = reinterpret_cast<const type_type*>(gauche);
		const type_type *d = reinterpret_cast<const type_type*>(droit);
		return ::svm_value_plugin_compare_convert(svm,::svm_type_compare(svm,g->_type,d->_type));
	}

	void *type_type_constant(const void *svm, const SVM_String constante)
	{
		std::string c(constante.string,constante.size);
		SVM_Type t;
		if(c=="AUTO")
		{
			t = ::svm_type_new_internal(svm,AUTOMATIC);
		}
		else if(c=="INT")
		{
			t = ::svm_type_new_internal(svm,INTEGER);
		}
		else if(c=="STR")
		{
			t = ::svm_type_new_internal(svm,STRING);
		}
		else if(c=="BLN")
		{
			t = ::svm_type_new_internal(svm,BOOLEAN);
		}
		else if(c=="PTR")
		{
			t = ::svm_type_new_internal(svm,POINTER);
		}
		else if(c=="LIB")
		{
			t = ::svm_type_new_internal(svm,LIBRARY);
		}
		else if(c=="SYM")
		{
			t = ::svm_type_new_internal(svm,SYMBOL);
		}
		else if(c=="PEP")
		{
			t = ::svm_type_new_internal(svm,PLUGIN_ENTRY_POINT);
		}
		else if(c=="IRQ")
		{
			t = ::svm_type_new_internal(svm,INTERRUPTION);
		}
		else
		{
			auto it = c.find('.');
			if(it==std::string::npos)
			{
				::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid external type.");
			}
			std::string p = c.substr(0,it);
			std::string e = c.substr(it+1);
			t=::svm_type_new_external(svm,::svm_value_pluginentrypoint_new__string(svm,::svm_string_new(svm,p.c_str(),p.size()),::svm_string_new(svm,e.c_str(),e.size())));
		}
		type_type *ti = new type_type;
		ti->_type = t;
		::svm_variable_scope_set_global(svm,t);
		return ti;
	}

	SVM_String type_type_print(const void *svm, void *objet)
	{
		type_type *t = reinterpret_cast<type_type*>(objet);
		return ::svm_type_print(svm,t->_type);
	}

	SVM_Value instruction_new(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		if(::svm_parameter_type_is_keyword(svm,argv[0]))
		{
			auto t = type_type_constant(svm,::svm_parameter_keyword_get(svm,argv[0]));
			return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"type","type"),t);
		}
		auto pep = ::svm_parameter_value_get(svm,argv[0]);
		auto st = ::svm_type_new_external(svm,pep);
		::svm_variable_scope_set_global(svm,st);
		auto t = new type_type;
		t->_type = st;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"type","type"),t);
	}

	SVM_Value function_new(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Type type = ::svm_parameter_variable_get(svm,argv[0]);
		::svm_variable_scope_set_shared(svm,type);
		::svm_variable_scope_set_global(svm,type);
		auto t = new type_type;
		t->_type = type;
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"type","type"),t);
	}

	SVM_Value function_get(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Plugin pt = ::svm_parameter_value_get(svm,argv[0]);
		type_type *t=reinterpret_cast<type_type*>(::svm_value_plugin_get_internal(svm,pt));
		return t->_type;
	}

	SVM_Value instruction_cmp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		type_type* gauche = reinterpret_cast<type_type*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0])));
		SVM_String cmp = ::svm_parameter_marker_get(svm,argv[1]);
		type_type* droite = reinterpret_cast<type_type*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[2])));
		std::string comparateur(cmp.string,cmp.size);
		SVM_Comparison_Result c = ::svm_type_compare(svm,gauche->_type,droite->_type);
		bool resultat = false;
		if(comparateur == "=")
		{
			resultat = c.equal;
		}
		else if(comparateur == "<>")
		{
			resultat = c.different;
		}
		else if(comparateur == "<")
		{
			resultat = c.inferior;
		}
		else if(comparateur == "<=")
		{
			resultat = c.inferior_or_equal;
		}
		else if(comparateur == ">")
		{
			resultat = c.superior;
		}
		else if(comparateur == "=>")
		{
			resultat = c.superior_or_equal;
		}
		return ::svm_value_boolean_new(svm,resultat?TRUE:FALSE);
	}

	SVM_Value function_cmp(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		return instruction_cmp(svm,argc,argv);
	}

	SVM_Value instruction_extract(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Memory_Zone zone = ::svm_memory_pointer_get_zone(svm,::svm_kernel_get_current(svm),p);
		SVM_Size s = ::svm_memory_zone_get_size(svm,zone);
		SVM_Value_Plugin *r = ::svm_value_array_new(svm,s);
		SVM_Value_PluginEntryPoint pep = ::svm_value_pluginentrypoint_new__raw(svm,"type", "type");
		for(SVM_Index i=0 ; i<s ; ++i)
		{
			SVM_Type t = ::svm_memory_zone_get_type(svm,zone,i);
			::svm_variable_scope_set_global(svm,t);
			type_type *tt = new type_type;
			tt->_type = t;
			r[i] = ::svm_value_plugin_new(svm,pep,tt);
		}
		SVM_Memory_Zone z = ::svm_memory_zone_new(svm);
		::svm_memory_zone_append_external__raw(svm,z,pep,s);
		SVM_Value_Pointer pp = ::svm_memory_allocate(svm,::svm_kernel_get_current(svm),z);
		::svm_memory_write_pointer(svm,::svm_kernel_get_current(svm),pp,r);
		return pp;
	}

	SVM_Value instruction_memory(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Index index = 0;
		bool global = false;
		if(::svm_parameter_type_is_keyword(svm,argv[0]))
		{
			global = true;
			++index;
		}
		SVM_Memory_Zone zone = ::svm_memory_zone_new(svm);
		SVM_Value vp = ::svm_parameter_value_get(svm,argv[index]);
		if(::svm_type_equal_internal(svm,::svm_value_type_get(svm,vp),POINTER))
		{
			SVM_Value_Plugin *vt = ::svm_memory_read_pointer_type_external(svm,::svm_kernel_get_current(svm),vp,::svm_value_pluginentrypoint_new__raw(svm,"type","type"));
			for(SVM_Value_Plugin *it=vt ; *it ; ++it)
			{
				type_type *t=reinterpret_cast<type_type*>(::svm_value_plugin_get_internal(svm,*it));
				::svm_memory_zone_append__raw(svm,zone,t->_type,1);
			}

		}
		else
		{
			for( ; ; )
			{
				type_type* type = reinterpret_cast<type_type*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[index])));
				++index;
				SVM_Value_Integer i = nullptr;
				if(index<argc)
				{
					SVM_Value v = ::svm_parameter_value_get(svm,argv[index]);
					if(::svm_type_equal_internal(svm,::svm_value_type_get(svm,v),INTEGER))
					{
						i = v;
					}
				}
				if(i)
				{
					::svm_memory_zone_append(svm,zone,type->_type,i);
				}
				else
				{
					::svm_memory_zone_append__raw(svm,zone,type->_type,1);
				}
				++index;
				if(index>=argc)
					break;
			}
		}
		SVM_Value_Pointer p = ::svm_memory_allocate(svm,::svm_kernel_get_current(svm),zone);
		if(global)
		{
			::svm_memory_scope_set_global(svm,::svm_kernel_get_current(svm),p);
		}
		return p;
	}

}
