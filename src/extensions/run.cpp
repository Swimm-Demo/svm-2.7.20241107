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
#include <string>
#include <memory>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

struct PileNoyaux
{
	std::list<SVM_Kernel> _noyaux;
};

struct Parallele
{
	Parallele()
	:_limite(0) {}
	std::list<SVM_Process> _processus;
	unsigned long int _limite;
};

struct RRpreempt
{
	RRpreempt(const unsigned long int periode)
	:_periode(periode) {}
	std::list<std::pair<SVM_Process,SVM_Process_State> > _processus;
	unsigned long int _periode;
};

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN run \n"
		"DEFINE \n"
		"  INSTRUCTION run.interrupt IRQ \n"
		"  INSTRUCTION run.interruption [ 'GLOBAL' 'CASCADE' ] ? [ STR SYM ] : target ? \n"
		"  SYSTEM INSTRUCTION run.coredump [ 'STDOUT' 'STDERR' 'TRACE' ] ? -> STR \n"
		"  SYSTEM INSTRUCTION run.trace 'LOCATION' ? VALUE + \n"
		"  WAITING INSTRUCTION run.sleep [ 'HARD' 'SOFT' ] INT:seconds \n"
		"  INSTRUCTION run.suspend \n"
		"  INSTRUCTION run.retry INT ? : number_instructions \n"
		"  INSTRUCTION run.loop MUTABLE INT:index ( { INT:step } ) ? [ < > ] INT:limit STR:label \n"
		"  INSTRUCTION run.protected_call SYM:function PTR:parameters ( 'NOIRQ' | 'CPU' = INT:max_instructions | 'RAM' = INT:max_memory | 'PARAMS' = [ 'COPY' 'MOVE' 'SHARE' ] 'CASCADE' ? ) * \n"
		"  WAITING INSTRUCTION run.parallel_call SYM:function PTR:parameters STR:name ( 'PM' = BLN:protected_mode | 'SEQ' = [ 'DEFAULT' PEP ]:sequencer | 'SCHED' = [ 'DEFAULT' PEP ]:scheduler | 'CPU' = INT:max_instructions | 'RAM' = INT:max_memory | 'PARAMS' = [ 'COPY' 'MOVE' 'SHARE' ] 'CASCADE' ?) * \n"
		"  INSTRUCTION run.parallel_limit [ 'NONE' INT ] \n"
		"  INSTRUCTION run.rrpreempt_timer INT \n"
		"  SEQUENCER run.stack \n "
		"  SCHEDULER run.parallel \n "
		"  SCHEDULER run.rrpreempt \n "
		"  OPTION run.parallel_limit -l INT \n "
		"  OPTION run.rrpreempt_timer -t INT \n "
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	SVM_Value instruction_interrupt(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		::svm_processor_add_interruption(svm,::svm_kernel_get_current(svm),::svm_parameter_value_get(svm,argv[0]));
		return nullptr;
	}

	SVM_Value instruction_interruption(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Object_Level niveau = LOCAL;
		SVM_Value cible = nullptr;
		SVM_Address adresse = 0;
		SVM_Value_Symbol symbole = nullptr;
		SVM_Kernel k = ::svm_kernel_get_current(svm);
		for(SVM_Index i = 0 ; i<argc ; ++i)
		{
			if(::svm_parameter_type_is_keyword(svm,argv[i]))
			{
				std::string mode(::svm_parameter_keyword_get(svm,argv[i]).string);
				niveau = (mode=="GLOBAL")?GLOBAL:CASCADE;
			}
			else
			{
				cible = ::svm_parameter_value_get(svm,argv[i]);
				if(::svm_value_type_is_string(svm,cible))
				{
					adresse = ::svm_code_label_get_address(svm,::svm_processor_get_currentcode(svm,k),cible);
				}
				else
				{
					symbole = cible;
				}
			}
		}
		for(SVM_Value_Interruption *t = ::svm_machine_list_interruption(svm) ; *t ; ++t)
		{
			if(cible)
			{
				if(symbole)
				{
					::svm_processor_interruptionhandler_set_global(svm,k,*t,symbole,niveau);
				}
				else
				{
					::svm_processor_interruptionhandler_set_local(svm,k,*t,adresse,niveau);
				}
			}
			else
			{
				::svm_processor_interruptionhandler_reset(svm,k,*t,niveau);
			}
		}
		return nullptr;
	}

	SVM_Value instruction_coredump(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String cadavre = ::svm_kernel_get_coredump(svm,::svm_kernel_get_current(svm));
		if(argc>0)
		{
			std::string str_cadavre(cadavre.string,cadavre.size);
			std::string sortie(::svm_parameter_keyword_get(svm,argv[0]).string);
			if(sortie=="STDOUT") std::cout << str_cadavre;
			if(sortie=="STDERR") std::cerr << str_cadavre;
			if(sortie=="TRACE") ::svm_machine_trace__string(svm,cadavre);
		}
		return ::svm_value_string_new(svm,cadavre);
	}

	SVM_Value instruction_trace(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		size_t debut = 0;
		std::ostringstream oss;
		if(::svm_parameter_type_is_keyword(svm,argv[0]))
		{
			auto p = ::svm_value_symbol_get_location(svm,::svm_processor_get_currentinstruction(svm,::svm_kernel_get_current(svm)));
			oss << std::string(p.string,p.size) << ": ";
			debut = 1;
		}
		for(size_t indice=debut ; indice<argc ; ++indice)
		{
			SVM_Value v = svm_parameter_value_get(svm,argv[indice]);
			SVM_String s = svm_value_print(svm,v);
			oss << std::string(s.string,s.size);
		}
		::svm_machine_trace__string(svm,::svm_string_new(svm,oss.str().c_str(),oss.str().size()));
		return nullptr;
	}

	SVM_Value instruction_sleep(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		std::string methode(::svm_parameter_keyword_get(svm,argv[0]).string);
		long long int secondes = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[1]));
		if(secondes<=0) return nullptr;
		::svm_processor_current_sleep__raw(svm,secondes,0,(methode=="SOFT")?TRUE:FALSE);
		return nullptr;
	}

	SVM_Value instruction_suspend(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		::svm_process_suspend(svm,::svm_process_get_current(svm));
		return nullptr;
	}

	SVM_Value instruction_retry(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		long long int d = 1;
		if(argc>1)
		{
			d = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
		}
		::svm_processor_return(svm,::svm_kernel_get_current(svm));
		SVM_Address a = ::svm_value_symbol_get_address(svm,::svm_processor_get_nextinstruction(svm,::svm_kernel_get_current(svm)));
		if(d<=0) return nullptr;
		if(static_cast<size_t>(d)>=a)
		{
			a = 0;
		}
		else
		{
			a -= d;
		}
		::svm_processor_jump_local(svm,::svm_kernel_get_current(svm),a);
		return nullptr;
	}

	SVM_Value instruction_loop(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Integer indice_val = ::svm_parameter_value_get(svm,argv[0]);
		long long int indice = ::svm_value_integer_get(svm,indice_val);
		size_t argv_sens = 1;
		size_t argv_limite = 2;
		size_t argv_etiquette = 3;
		SVM_String delimiteur = ::svm_parameter_marker_get(svm,argv[1]);
		long long int increment = 1;
		if(::svm_string_is_equal__raw(svm,delimiteur,"{"))
		{
			argv_sens += 3;
			argv_limite += 3;
			argv_etiquette += 3;
			SVM_Value_Integer increment_val = ::svm_parameter_value_get(svm,argv[2]);
			increment = ::svm_value_integer_get(svm,increment_val);
			if(increment<=0)
			{
				::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Increment can not be negative.");
			}
		}
		SVM_String sens = ::svm_parameter_marker_get(svm,argv[argv_sens]);
		if(::svm_string_is_equal__raw(svm,sens,">"))
		{
			increment *= -1;
		}
		SVM_Value_Integer limite_val = ::svm_parameter_value_get(svm,argv[argv_limite]);
		long long int limite = ::svm_value_integer_get(svm,limite_val);
		indice += increment;
		SVM_Kernel noyau_courant = ::svm_kernel_get_current(svm);
		SVM_Code code_courant = ::svm_processor_get_currentcode(svm,noyau_courant);
		SVM_Value_String etiquette = ::svm_parameter_value_get(svm,argv[argv_etiquette]);
		SVM_Address debut_boucle = ::svm_code_label_get_address(svm,code_courant,etiquette);
		if(((increment>0)&&(indice<limite))||((increment<0)&&(indice>limite)))
		{
			::svm_value_integer_set(svm,indice_val,indice);
			::svm_processor_jump_local(svm,noyau_courant,debut_boucle);
		}
		return nullptr;
	}

	enum class GestionParametres { COPIE, DEPLACEMENT, PARTAGE };
	void transfert_memoire(const void *svm, SVM_Kernel courant, SVM_Kernel noyau, SVM_Value_Pointer parametres, const bool cascade, const GestionParametres gestion_parametres)
	{
		SVM_Value_Pointer *transfert=nullptr;
		if(cascade)
		{
			transfert = ::svm_memory_pointer_list_accessible(svm,courant,parametres);
		}
		else
		{
			transfert = ::svm_value_array_new(svm,2);
			transfert[0] = parametres;
			transfert[1] = nullptr;
		}
		for(SVM_Value_Pointer *index=transfert ; *index ; ++index)
		{
			SVM_Memory_Zone zone = ::svm_memory_zone_new(svm);
			::svm_memory_zone_append_internal__raw(svm,zone,AUTOMATIC,::svm_value_pointer_get_size(svm,*index));
			::svm_memory_allocate_address(svm,noyau,zone,::svm_value_pointer_get_address(svm,*index));
			switch(gestion_parametres)
			{
				case GestionParametres::COPIE:
					::svm_memory_copy(svm,courant,*index,noyau,*index);
					break;
				case GestionParametres::DEPLACEMENT:
					::svm_memory_move(svm,courant,*index,noyau,*index);
					break;
				case GestionParametres::PARTAGE:
					::svm_memory_share(svm,courant,*index,noyau,*index);
					break;
			}
		}
		::svm_processor_set_currentpointer(svm,noyau,parametres);
	}

	SVM_Value instruction_protected_call(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Symbol fonction = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_Pointer parametres = ::svm_parameter_value_get(svm,argv[1]);
		SVM_Boolean interruptions = TRUE;
		std::shared_ptr<SVM_Size> max_instruction;
		std::shared_ptr<SVM_Size> max_memoire;
		GestionParametres gestion_parametres = GestionParametres::PARTAGE;
		bool cascade = false;
		SVM_Size indice = 2;
		while(indice<argc)
		{
			std::string mot_clef(::svm_parameter_keyword_get(svm,argv[indice]).string);
			if(mot_clef == "NOIRQ")
			{
				interruptions = FALSE;
				++indice;
			}
			else if(mot_clef == "CPU")
			{
				long long int max = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[indice+2]));
				if(max>=0)
				{
					max_instruction = std::make_shared<SVM_Size>(max);
				}
				indice += 3;
			}
			else if(mot_clef == "RAM")
			{
				long long int max = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[indice+2]));
				if(max>=0)
				{
					max_memoire = std::make_shared<SVM_Size>(max+::svm_value_pointer_get_size(svm,parametres));
				}
				indice += 3;
			}
			else if(mot_clef == "PARAMS")
			{
				indice += 2;
				SVM_String m = ::svm_parameter_keyword_get(svm,argv[indice]);
				std::string mode(m.string,m.size);
				if(mode=="COPY") gestion_parametres = GestionParametres::COPIE;
				if(mode=="MOVE") gestion_parametres = GestionParametres::DEPLACEMENT;
				if(mode=="SHARE") gestion_parametres = GestionParametres::PARTAGE;
				cascade = false;
				if((indice+1<argc) and (::svm_parameter_type_is_keyword(svm,argv[indice+1])))
				{
					SVM_String s = ::svm_parameter_keyword_get(svm,argv[indice+1]);
					if(std::string(s.string,s.size)=="CASCADE")
					{
						++indice;
						cascade = true;
					}
				}
				++indice;
			}
		}
		SVM_AccessControl controle_acces = ::svm_accesscontrol_new(svm,max_instruction.get(),max_memoire.get());
		SVM_Kernel noyau = ::svm_kernel_new_symbol(svm,interruptions,TRUE,TRUE,controle_acces,fonction);
		SVM_Kernel courant = ::svm_kernel_get_current(svm);
		::transfert_memoire(svm,courant,noyau,parametres,cascade,gestion_parametres);
		if(::svm_process_kernel_attach(svm,::svm_process_get_current(svm),noyau,0,NULL)==FALSE)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Unable to attach a protected kernel to the current process");
		}
		::svm_kernel_suspend(svm,svm_kernel_get_current(svm));
		return nullptr;
	}

	SVM_Value instruction_parallel_call(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Symbol fonction = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_Pointer parametres = ::svm_parameter_value_get(svm,argv[1]);
		SVM_Value_String nom = ::svm_parameter_value_get(svm,argv[2]);
		SVM_Boolean mode_protege = FALSE;
		SVM_Value_PluginEntryPoint sequenceur = ::svm_process_get_sequencer(svm,::svm_process_get_current(svm));
		SVM_Scheduler ordonnanceur = ::svm_process_get_scheduler(svm,::svm_process_get_current(svm));
		std::shared_ptr<SVM_Size> max_instruction;
		std::shared_ptr<SVM_Size> max_memoire;
		GestionParametres gestion_parametres = GestionParametres::PARTAGE;
		bool cascade = false;
		SVM_Size indice = 3;
		while(indice<argc)
		{
			std::string mot_clef(::svm_parameter_keyword_get(svm,argv[indice]).string);
			indice += 2;
			if(mot_clef == "PM")
			{
				mode_protege = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[indice]));
			}
			else if(mot_clef == "SEQ")
			{
				if(::svm_parameter_type_is_keyword(svm,argv[indice]))
				{
					sequenceur = nullptr;
				}
				else
				{
					sequenceur = ::svm_parameter_value_get(svm,argv[indice]);
				}
			}
			else if(mot_clef == "SCHED")
			{
				if(::svm_parameter_type_is_keyword(svm,argv[indice]))
				{
					ordonnanceur = ::svm_scheduler_get(svm,nullptr);
				}
				else
				{
					ordonnanceur = ::svm_scheduler_get(svm,::svm_parameter_value_get(svm,argv[indice]));
				}
			}
			else if(mot_clef == "CPU")
			{
				long long int max = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[indice]));
				if(max>=0)
				{
					max_instruction = std::make_shared<SVM_Size>(max);
				}
			}
			else if(mot_clef == "RAM")
			{
				long long int max = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[indice]));
				if(max>=0)
				{
					max_memoire = std::make_shared<SVM_Size>(max+::svm_value_pointer_get_size(svm,parametres));
				}
			}
			else if(mot_clef == "PARAMS")
			{
				SVM_String m = ::svm_parameter_keyword_get(svm,argv[indice]);
				std::string mode(m.string,m.size);
				if(mode=="COPY") gestion_parametres = GestionParametres::COPIE;
				if(mode=="MOVE") gestion_parametres = GestionParametres::DEPLACEMENT;
				if(mode=="SHARE") gestion_parametres = GestionParametres::PARTAGE;
				cascade = false;
				if((indice+1<argc) and (::svm_parameter_type_is_keyword(svm,argv[indice+1])))
				{
					SVM_String s = ::svm_parameter_keyword_get(svm,argv[indice+1]);
					if(std::string(s.string,s.size)=="CASCADE")
					{
						++indice;
						cascade = true;
					}
				}
			}
			++indice;
		}

		SVM_Kernel courant = ::svm_kernel_get_current(svm);
		SVM_AccessControl controle_acces = ::svm_accesscontrol_new(svm,max_instruction.get(),max_memoire.get());
		SVM_Kernel noyau = ::svm_kernel_new_symbol(svm,TRUE,TRUE,mode_protege,controle_acces,fonction);
		::transfert_memoire(svm,courant,noyau,parametres,cascade,gestion_parametres);
		SVM_Process processus = ::svm_process_new(svm,nom,sequenceur,FALSE,noyau);
		::svm_process_ownership_get_local(svm,processus);
		if(not ::svm_scheduler_process_attach__raw(svm,ordonnanceur,processus,0))
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Unable to attach process to scheduler");
		}
		return nullptr;
	}

	SVM_Value instruction_parallel_limit(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		size_t limite = 0;
		if(::svm_parameter_type_is_value(svm,argv[0]))
		{
			long long int l = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
			if(l<1)
			{
				::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid limit");
			}
			limite = static_cast<size_t>(l);
		}
		::svm_scheduler_notify__raw(svm,::svm_scheduler_get(svm,::svm_value_pluginentrypoint_new__raw(svm,"run","parallel")),limite);
		return nullptr;
	}

	SVM_Value instruction_rrpreempt_timer(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		size_t periode = 0;
		long long int l = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[0]));
		if(l<1)
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Invalid periode");
		}
		periode = static_cast<size_t>(l);
		::svm_scheduler_notify__raw(svm,::svm_scheduler_get(svm,::svm_value_pluginentrypoint_new__raw(svm,"run","rrpreempt")),periode);
		return nullptr;
	}

	void* sequencer_stack_create(const void *svm)
	{
		PileNoyaux *pile = new PileNoyaux;
		return pile;
	}

	void sequencer_stack_delete(const void *svm, void *sequencer)
	{
		PileNoyaux *pile = reinterpret_cast<PileNoyaux*>(sequencer);
		for(auto& n : pile->_noyaux)
		{
			::svm_variable_delete(svm,n);
		}
		delete pile;
	}

	SVM_Kernel sequencer_stack_current(const void *svm, void *sequencer)
	{
		PileNoyaux *pile = reinterpret_cast<PileNoyaux*>(sequencer);
		for(auto it=pile->_noyaux.begin() ; it!=pile->_noyaux.end() ; ++it)
		{
			if(::svm_kernel_is_runnable(svm,*it))
			{
				return *it;
			}
		}
		return nullptr;
	}

	SVM_Boolean sequencer_stack_attach(const void *svm, void *sequencer, SVM_Kernel kernel, size_t argc, void *argv[])
	{
		PileNoyaux *pile = reinterpret_cast<PileNoyaux*>(sequencer);
		::svm_variable_scope_set_global(svm,kernel);
		pile->_noyaux.push_front(kernel);
		return TRUE;
	}

	SVM_Boolean sequencer_stack_detach(const void *svm, void *sequencer, SVM_Kernel kernel, size_t argc, void *argv[])
	{
		PileNoyaux *pile = reinterpret_cast<PileNoyaux*>(sequencer);
		auto it = std::find(pile->_noyaux.begin(),pile->_noyaux.end(),kernel);
		if(it==pile->_noyaux.end())
			return FALSE;
		::svm_variable_delete(svm,kernel);
		pile->_noyaux.erase(it);
		return TRUE;
	}

	SVM_String sequencer_stack_print(const void *svm, const void *sequencer)
	{
		const PileNoyaux *pile = reinterpret_cast<const PileNoyaux*>(sequencer);
		std::ostringstream oss;
		for(auto& n : pile->_noyaux)
		{
			SVM_String s = ::svm_kernel_print(svm,n);
			oss << "  Kernel " << std::string(s.string,s.size) << std::endl;
		}
		return ::svm_string_new__raw(svm,oss.str().c_str());
	}

	void *scheduler_parallel_create(const void *svm)
	{
		Parallele *liste = new Parallele;
		return liste;
	}

	void scheduler_parallel_delete(const void *svm, void *scheduler)
	{
		Parallele *liste = reinterpret_cast<Parallele*>(scheduler);
		for(auto& p: liste->_processus)
		{
			::svm_variable_delete(svm,p);
		}
		delete liste;
	}

	SVM_Boolean scheduler_parallel_attach(const void *svm, void *scheduler, SVM_Process process, size_t parameter)
	{
		Parallele *liste = reinterpret_cast<Parallele*>(scheduler);
		if(liste->_limite>0)
		{
			if(liste->_processus.size()>=liste->_limite)
			{
				return FALSE;
			}
		}
		::svm_variable_scope_set_global(svm,process);
		liste->_processus.push_back(process);
		return TRUE;
	}

	SVM_Boolean scheduler_parallel_detach(const void *svm, void *scheduler, SVM_Process process, size_t parameter)
	{
		Parallele *liste = reinterpret_cast<Parallele*>(scheduler);
		auto it = std::find(liste->_processus.begin(),liste->_processus.end(),process);
		if(it==liste->_processus.end())
			return FALSE;
		::svm_variable_delete(svm,process);
		liste->_processus.erase(it);
		return TRUE;
	}

	unsigned long int scheduler_parallel_schedule(const void *svm, void *scheduler, SVM_Process process, const SVM_Process_State state)
	{
		if(state==SUSPENDED)
		{
			::svm_process_run__raw(svm,process,0);
		}
		return 0;
	}

	size_t scheduler_parallel_notification(const void *svm, void *scheduler, const SVM_Notification_Type type, const size_t parameter)
	{
		Parallele *liste = reinterpret_cast<Parallele*>(scheduler);
		if(type==NOTIFICATION)
		{
			if(parameter>=0) liste->_limite=parameter;
		}
		return 0;
	}

	SVM_String scheduler_parallel_print(const void *svm, const void *scheduler)
	{
		const Parallele *liste = reinterpret_cast<const Parallele*>(scheduler);
		std::ostringstream oss;
		if(liste->_limite>0)
		{
			oss << "Limit: " << liste->_limite << std::endl;
		}
		for(auto& p:liste->_processus)
		{
			SVM_String s = ::svm_process_print(svm,p);
			oss << "  " << std::string(s.string,s.size) << std::endl;
		}
		return ::svm_string_new__raw(svm,oss.str().c_str());
	}

	void *scheduler_rrpreempt_create(const void *svm)
	{
		RRpreempt *liste = new RRpreempt(50);
		return liste;
	}

	void scheduler_rrpreempt_delete(const void *svm, void *scheduler)
	{
		RRpreempt *liste = reinterpret_cast<RRpreempt*>(scheduler);
		for(auto& p: liste->_processus)
		{
			::svm_variable_delete(svm,p.first);
		}
		delete liste;
	}

	SVM_Boolean scheduler_rrpreempt_attach(const void *svm, void *scheduler, SVM_Process process, size_t parameter)
	{
		RRpreempt *liste = reinterpret_cast<RRpreempt*>(scheduler);
		::svm_variable_scope_set_global(svm,process);
		liste->_processus.push_back(std::make_pair(process,SUSPENDED));
		return TRUE;
	}

	SVM_Boolean scheduler_rrpreempt_detach(const void *svm, void *scheduler, SVM_Process process, size_t parameter)
	{
		RRpreempt *liste = reinterpret_cast<RRpreempt*>(scheduler);
		for(auto it=liste->_processus.begin() ; it!=liste->_processus.end() ; ++it)
		{
			if(it->first==process)
			{
				liste->_processus.erase(it);
				::svm_variable_delete(svm,process);
				return TRUE;
			}
		}
		return FALSE;
	}

	size_t scheduler_rrpreempt_schedule(const void *svm, void *scheduler, SVM_Process process, const SVM_Process_State state)
	{
		RRpreempt *liste = reinterpret_cast<RRpreempt*>(scheduler);
		SVM_Process_State ancien_etat;
		for(auto it=liste->_processus.begin() ; it!=liste->_processus.end() ; ++it)
		{
			if(it->first==process)
			{
				ancien_etat = it->second;
				it->second = state;
				if((ancien_etat==RUNNING) and (state==SUSPENDED))
				{
					auto p = *it;
					liste->_processus.erase(it);
					liste->_processus.push_back(p);
				}
				if((ancien_etat==WAITING) and (state==SUSPENDED))
				{
					auto p = *it;
					liste->_processus.erase(it);
					liste->_processus.push_front(p);
				}
				break;
			}
		}
		bool en_cours = false;
		for(auto it=liste->_processus.begin() ; it!=liste->_processus.end() ; ++it)
		{
			if(it->second==RUNNING)
				en_cours = true;
		}
		if(not en_cours)
		{
			for(auto it=liste->_processus.begin() ; it!=liste->_processus.end() ; ++it)
			{
				if(it->second==SUSPENDED)
				{
					::svm_process_run__raw(svm,it->first,0);
					break;
				}

			}
		}
		return liste->_periode;
	}

	size_t scheduler_rrpreempt_notification(const void *svm, void *scheduler, const SVM_Notification_Type type, const size_t parameter)
	{
		RRpreempt *liste = reinterpret_cast<RRpreempt*>(scheduler);
		if(type==TIMER)
		{
			for(auto& p:liste->_processus)
			{
				if((p.second == RUNNING) or (p.second == WAITING))
				{
					::svm_process_suspend(svm,p.first);
					break;
				}
			}
		}
		if(type==NOTIFICATION)
		{
			if(parameter>0) liste->_periode = parameter;
		}
		return liste->_periode;
	}

	SVM_String scheduler_rrpreempt_print(const void *svm, const void *scheduler)
	{
		const RRpreempt *liste = reinterpret_cast<const RRpreempt*>(scheduler);
		std::ostringstream oss;
		oss << "Timer " << liste->_periode << "ms" << std::endl;
		for(auto& p:liste->_processus)
		{
			SVM_String s = ::svm_process_print(svm,p.first);
			oss << "  " << std::string(s.string,s.size) << std::endl;
		}
		return ::svm_string_new__raw(svm,oss.str().c_str());
	}

	void plugin_startup(const void *svm)
	{
		SVM_Value_Integer limite = ::svm_plugin_get_option(svm,::svm_value_pluginentrypoint_new__raw(svm,"run","parallel_limit"));
		if(not ::svm_value_state_is_null(svm,limite))
		{
			::svm_scheduler_notify(svm,::svm_scheduler_get(svm,::svm_value_pluginentrypoint_new__raw(svm,"run","parallel")),limite);
		}
		SVM_Value_Integer periode = ::svm_plugin_get_option(svm,::svm_value_pluginentrypoint_new__raw(svm,"run","rrpreempt_timer"));
		if(not ::svm_value_state_is_null(svm,periode))
		{
			::svm_scheduler_notify(svm,::svm_scheduler_get(svm,::svm_value_pluginentrypoint_new__raw(svm,"run","rrpreempt")),periode);
		}
	}
}

