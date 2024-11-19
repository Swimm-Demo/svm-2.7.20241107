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
		"PLUGIN plugin \n"
		"DEFINE \n"
		"  SYSTEM INSTRUCTION plugin.dump [ 'STDOUT' 'STDERR' ] ? -> STR \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	SVM_Value instruction_dump(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String cadavre = ::svm_plugin_print(svm);
		if(argc>0)
		{
			std::string str_cadavre(cadavre.string,cadavre.size);
			std::string sortie(::svm_parameter_keyword_get(svm,argv[0]).string);
			if(sortie=="STDOUT") std::cout << str_cadavre;
			if(sortie=="STDERR") std::cerr << str_cadavre;
		}
		return ::svm_value_string_new(svm,cadavre);
	}

}
