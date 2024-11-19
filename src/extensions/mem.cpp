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

#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>
#include <sstream>

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN mem \n"
		"DEFINE \n"
		"  INSTRUCTION mem.copy PTR => PTR \n"
		"  INSTRUCTION mem.move PTR => PTR \n"
		"  INSTRUCTION mem.share PTR => PTR \n"
		"  INSTRUCTION mem.global PTR \n"
		"  INSTRUCTION mem.free PTR \n"
		"  INSTRUCTION mem.aggregate 'GLOBAL' ? MUTABLE PTR -> PTR \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	SVM_Value instruction_copy(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		::svm_memory_copy(svm,::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[0]),::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[2]));
		return nullptr;
	}

	SVM_Value instruction_move(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		::svm_memory_move(svm,::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[0]),::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[2]));
		return nullptr;
	}

	SVM_Value instruction_share(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		::svm_memory_share(svm,::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[0]),::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[2]));
		return nullptr;
	}

	SVM_Value instruction_global(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_String *a = ::svm_memory_pointer_expand_alias(svm,::svm_kernel_get_current(svm),p);
		if(*a)
		{
			std::ostringstream oss;
			SVM_String sa = ::svm_value_string_get(svm,*a);
			SVM_String sp = ::svm_value_print(svm,p);
			oss << "Pointer " << std::string(sp.string,sp.size) << " touches definition of alias " << std::string(sa.string,sa.size);
			::svm_processor_current_raise_error_internal__raw(svm,MEMORY,oss.str().c_str());
		}
		::svm_memory_scope_set_global(svm,::svm_kernel_get_current(svm),p);
		return nullptr;
	}

	SVM_Value instruction_free(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_String *a = ::svm_memory_pointer_expand_alias(svm,::svm_kernel_get_current(svm),p);
		if(*a)
		{
			std::ostringstream oss;
			SVM_String sa = ::svm_value_string_get(svm,*a);
			SVM_String sp = ::svm_value_print(svm,p);
			oss << "Pointer " << std::string(sp.string,sp.size) << " touches definition of alias " << std::string(sa.string,sa.size);
			::svm_processor_current_raise_error_internal__raw(svm,MEMORY,oss.str().c_str());
		}
		::svm_memory_free(svm,::svm_kernel_get_current(svm),p);
		::svm_memory_scope_set_global(svm,::svm_kernel_get_current(svm),p);
		return nullptr;
	}

	SVM_Value instruction_aggregate(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		bool global = ::svm_parameter_type_is_marker(svm,argv[0])==TRUE;
		SVM_Value_Pointer p = ::svm_parameter_value_get(svm,argv[argc-1]);
		SVM_Value_Pointer *ap = ::svm_memory_pointer_list_accessible(svm,::svm_kernel_get_current(svm),p);
		SVM_Size tap = 0;
		SVM_Size tz = 0;
		for(SVM_Value_Pointer *iap=ap ; *iap ; ++iap)
		{
			++tap;
			tz += ::svm_value_pointer_get_size(svm,*iap);
			SVM_Value_String *la = ::svm_memory_pointer_expand_alias(svm,::svm_kernel_get_current(svm),*iap);
			if(*la)
			{
				std::ostringstream oss;
				SVM_String sa = ::svm_value_string_get(svm,*la);
				SVM_String siap = ::svm_value_print(svm,*iap);
				SVM_String sp = ::svm_value_print(svm,p);
				oss << "Pointer " << std::string(siap.string,siap.size) << " accessible from " << std::string(sp.string,sp.size) << " touches definition of alias " << std::string(sa.string,sa.size);
				::svm_processor_current_raise_error_internal__raw(svm,MEMORY,oss.str().c_str());
			}
			::svm_variable_delete(svm,la);
		}
		SVM_Memory_Zone z = ::svm_memory_zone_new(svm);
		::svm_memory_zone_append_internal__raw(svm,z,AUTOMATIC,tz);
		SVM_Value_Pointer r = ::svm_memory_allocate(svm,::svm_kernel_get_current(svm),z);
		if(global)
		{
			::svm_memory_scope_set_global(svm,::svm_kernel_get_current(svm),r);
		}
		SVM_Address a = ::svm_value_pointer_get_address(svm,r);
		SVM_Value_Pointer *rp = ::svm_value_array_new(svm,tap);
		for(SVM_Index i=0 ; i<tap ; ++i)
		{
			SVM_Size sap = ::svm_value_pointer_get_size(svm,ap[i]);
			rp[i] = ::svm_value_pointer_new__raw(svm,a,sap);
			a += sap;
			::svm_memory_move(svm,::svm_kernel_get_current(svm),ap[i],::svm_kernel_get_current(svm),rp[i]);
			::svm_memory_free(svm,::svm_kernel_get_current(svm),ap[i]);
			::svm_memory_scope_set_global(svm,::svm_kernel_get_current(svm),ap[i]);
		}
		for(SVM_Index i=0 ; i<tap ; ++i)
		{
			for(SVM_Index j=0 ; j<tap ; ++j)
			{
				::svm_memory_translate(svm,::svm_kernel_get_current(svm),rp[i],ap[j],rp[j]);
			}
		}
		::svm_value_pointer_set(svm,p,rp[0]);
		return r;
	}
}
