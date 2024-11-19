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
#include <mutex>
#include <memory>
#include <map>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <string.h>
#include <termios.h>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/svm_compatibility.h>

#define TAILLE_DONNEES_FLUX 4096

extern "C"
{
	std::string strerror_local(int e)
	{
		return ::strerror(e);
	}
	std::string gai_strerror_local(int e)
	{
		return ::gai_strerror(e);
	}
	static int tcp_file = 20;
}

struct Peripherique
{
	Peripherique(SVM_Structure canal)
	:_canal(canal), _ouverture(nullptr), _affichage(nullptr), _lecture(nullptr), _ecriture(nullptr), _attente(nullptr), _commande(nullptr), _fermeture(nullptr) {}
	SVM_Structure _canal;
	std::mutex _acces_tampon;
	std::stringstream _tampon;
	SVM_Value_PluginEntryPoint _ouverture;
	SVM_Value_PluginEntryPoint _affichage;
	SVM_Value_PluginEntryPoint _lecture;
	SVM_Value_PluginEntryPoint _ecriture;
	SVM_Value_PluginEntryPoint _attente;
	SVM_Value_PluginEntryPoint _commande;
	SVM_Value_PluginEntryPoint _fermeture;
};

struct Terminal
{
	typedef std::map<std::string,std::shared_ptr<Peripherique> > Inventaire;
	static Inventaire& inventaire()
	{
		static Inventaire inventaire;
		return inventaire;
	}
	Terminal(const size_t id, FILE* file)
	:_id(id), _file(file), _ferme(false), _bloquant(true) {}
	size_t _id;
	FILE *_file;
	bool _ferme;
	bool _bloquant;
};

struct Fichier
{
	enum class Mode
	{
		LECTURE,
		ECRITURE,
		LECTURE_ECRITURE,
		AJOUT
	};
	Fichier(const int id, const std::string& fichier, const Mode mode)
	:_id(id), _fichier(fichier), _mode(mode) {}
	int _id;
	std::string _fichier;
	Mode _mode;
};

struct Memoire
{
	Memoire()
	{
		res0 = nullptr;
	}

	~Memoire()
	{
		if(res0)
			::freeaddrinfo(res0);
	}
	struct addrinfo *res0;
};

struct TCP
{
	TCP(const bool serveur, const std::string& ip, const std::string& port, const int id)
	:_ip_distante(ip), _port_distant(port), _id(id), _serveur(serveur), _auto(false), _bloquant(false), _fermeture_envoi(false), _fermeture_reception(false)
	{
		if(_serveur)
		{
			_ip_locale.swap(_ip_distante);
			_port_local.swap(_port_distant);
		}
	}
	std::string _ip_locale;
	std::string _port_local;
	std::string _ip_distante;
	std::string _port_distant;
	int _id;
	bool _serveur;
	bool _auto;
	bool _bloquant;
	bool _fermeture_envoi;
	bool _fermeture_reception;

	static bool resolution_ip_port(const struct sockaddr* sock, const socklen_t longueur, std::string& ip, std::string& port)
	{
		char host[TAILLE_DONNEES_FLUX+1];
		char serv[TAILLE_DONNEES_FLUX+1];
		bool resultat=::getnameinfo(sock,longueur,host,TAILLE_DONNEES_FLUX,serv,TAILLE_DONNEES_FLUX,NI_NUMERICHOST)==0;
		if(resultat)
		{
			ip = host;
			port = serv;
		}
		return resultat;
	}

	static void log_erreur(struct addrinfo *res, std::ostringstream& os, const std::string& fonction, int erreur)
	{
		std::string ip;
		std::string port;
		os << std::endl << fonction << " ";
		if(TCP::resolution_ip_port(res->ai_addr,res->ai_addrlen,ip,port))
		{
			os << "(" << ip << " : " << port << ")";
		}
		os << ": " << ::strerror_local(erreur);
	}

	template<bool appelle_bind, bool appelle_listen, bool appelle_connect>
		static int ouvrir(const void *svm, const int type, const std::string& ip, const std::string& port)
		{
			int fd;
			struct addrinfo hints, *res;
			int error;
			Memoire memoire;
			std::ostringstream details;
			details << "Unable to open TCP socket.";

			::memset(&hints, 0, sizeof(hints));
			hints.ai_family = PF_UNSPEC;
			hints.ai_socktype = type;
			error = ::getaddrinfo(ip.c_str(), port.c_str(), &hints, &memoire.res0);
			if (error)
			{
				details << std::endl << "getaddrinfo: " << ::gai_strerror_local(error);
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,details.str().c_str());
			}
			fd = -1;
			for (res = memoire.res0 ; res ; res = res->ai_next)
			{
				fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
				if (fd < 0)
				{
					TCP::log_erreur(res,details,"socket",errno);
					continue;
				}

				if(appelle_bind)
				{
					if (::bind(fd, res->ai_addr, res->ai_addrlen) < 0)
					{
						TCP::log_erreur(res,details,"bind",errno);
						::close(fd);
						fd = -1;
						continue;
					}
				}

				if(appelle_listen)
				{
					if(::listen(fd,tcp_file)<0)
					{
						TCP::log_erreur(res,details,"listen",errno);
						::close(fd);
						fd = -1;
						continue;
					}
				}

				if(appelle_connect)
				{
					if (::connect(fd, res->ai_addr, res->ai_addrlen) < 0)
					{
						TCP::log_erreur(res,details,"connect",errno);
						::close(fd);
						fd = -1;
						continue;
					}
				}
				break;
			}
			if (fd < 0)
			{
				details << std::endl;
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,details.str().c_str());
			}
			return fd;
		}
};

struct Attente
{
	enum class Mode
	{
		LECTURE,
		ECRITURE
	};
	Attente(const int id, Peripherique* peripherique, SVM_Value_PluginEntryPoint protocole, SVM_Size taille_parametres, SVM_Parameter *parametres, SVM_Value representant)
	:_mode(Mode::LECTURE), _id(id), _peripherique(peripherique), _protocole(protocole), _taille_parametres(taille_parametres), _parametres(parametres), _representant(representant), _eof(false) {}
	Attente(const int id, Peripherique* peripherique, SVM_Value representant)
	:_mode(Mode::ECRITURE), _id(id), _peripherique(peripherique), _protocole(nullptr), _taille_parametres(0), _parametres(nullptr), _representant(representant), _eof(false) {}
	Mode _mode;
	int _id;
	Peripherique *_peripherique;
	SVM_Value_PluginEntryPoint _protocole;
	SVM_Size _taille_parametres;
	SVM_Parameter *_parametres;
	SVM_Value _representant; // PTR ou com.device
	bool _eof;
};

extern "C"
{
	SVM_Value_PluginEntryPoint fonction_peripherique(const void *svm, SVM_Value_PluginEntryPoint type, const std::string& fonction)
	{
		SVM_String extension = ::svm_value_pluginentrypoint_get_plugin(svm, type);
		SVM_String entree = ::svm_value_pluginentrypoint_get_entry(svm, type);
		std::string f("device_");
		f += entree.string;
		f += "_";
		f += fonction;
		return ::svm_value_pluginentrypoint_new__raw(svm,extension.string,f.c_str());
	}

	SVM_Value fonctions_facultatives_peripherique(const void* svm, const SVM_Value_PluginEntryPoint type_peripherique, Peripherique* peripherique, SVM_Parameter *parametres)
	{
		SVM_Value_PluginEntryPoint fonction_affichage = ::fonction_peripherique(svm,type_peripherique,"print");
		if(::svm_plugin_has_function(svm,fonction_affichage,1,parametres,::svm_parameter_value_new(svm,::svm_value_string_new__raw(svm,""))))
		{
			peripherique->_affichage=fonction_affichage;
			::svm_variable_scope_set_global(svm,fonction_affichage);
		}
		SVM_Value_PluginEntryPoint fonction_lecture = ::fonction_peripherique(svm,type_peripherique,"read");
		if(::svm_plugin_has_function(svm,fonction_lecture,1,parametres,::svm_parameter_value_new(svm,::svm_value_string_new__raw(svm,""))))
		{
			peripherique->_lecture=fonction_lecture;
			::svm_variable_scope_set_global(svm,fonction_lecture);
		}
		parametres[1] = ::svm_parameter_value_new(svm,::svm_value_string_new__raw(svm,""));
		SVM_Value_PluginEntryPoint fonction_ecriture = ::fonction_peripherique(svm,type_peripherique,"write");
		if(::svm_plugin_has_function(svm,fonction_ecriture,2,parametres,nullptr))
		{
			peripherique->_ecriture=fonction_ecriture;
			::svm_variable_scope_set_global(svm,fonction_ecriture);
		}
		parametres[1] = ::svm_parameter_value_new(svm,::svm_value_integer_new(svm,0));
		parametres[2] = ::svm_parameter_value_new(svm,::svm_value_integer_new(svm,0));
		parametres[3] = ::svm_parameter_value_new(svm,::svm_value_integer_new(svm,0));
		SVM_Value_PluginEntryPoint fonction_attente = ::fonction_peripherique(svm,type_peripherique,"idle");
		if(::svm_plugin_has_function(svm,fonction_attente,4,parametres,nullptr))
		{
			peripherique->_attente=fonction_attente;
			::svm_variable_scope_set_global(svm,fonction_attente);
		}
		SVM_Value_PluginEntryPoint fonction_commande = ::fonction_peripherique(svm,type_peripherique,"command");
		if(::svm_plugin_has_function(svm,fonction_commande,1,parametres,::svm_parameter_value_new(svm,::svm_value_boolean_new_null(svm))))
		{
			peripherique->_commande=fonction_commande;
			::svm_variable_scope_set_global(svm,fonction_commande);
		}
		return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","device"),peripherique);
	}
}

extern "C"
{
	void plugin_configure(void *plugin)
	{
		::svm_plugin_configure(plugin,""
		"PLUGIN com \n"
		"DEFINE \n"
		"  INSTRUCTION com.message [ 'STDOUT' 'STDERR' ] ? VALUE + \n"
		"  WAITING INSTRUCTION com.prompt ( [ 'STDOUT' 'STDERR' ] ? STR ): prompt ? [ 'STR' 'INT' ]:output_type -> [ STR INT ] \n"
		"  TYPE com.device \n"
		"  SYSTEM WAITING INSTRUCTION com.open PEP :device_type . * :parameters -> com.device \n"
		"  WAITING INSTRUCTION com.read com.device PEP :protocol [ VALUE KEYWORD ] * :parameters -> STR ? \n"
		"  WAITING INSTRUCTION com.write com.device VALUE * \n"
		"  WAITING INSTRUCTION com.idle ( ( > | < ( PEP | { PEP [ VALUE KEYWORD ] * } ) ? :protocol ):mode PTR + :device_arrays ) + -> PTR :active_device \n"
		"  SYSTEM WAITING INSTRUCTION com.command com.device . * -> VALUE ? \n"
		"  INSTRUCTION com.equal com.device 2 -> BLN \n"
		"  FUNCTION com.device STRUCT -> com.device ? \n"
		"  INTERRUPTION com.interrupted \n"
		"  STRUCT com.terminal \n"
		//"  STRUCT com.buffer \n"
		"  STRUCT com.file \n"
		"  STRUCT com.tcp \n"
		"  FUNCTION com.device_terminal_open [ 'STDIN' 'STDOUT' 'STDERR' ] -> $com.terminal \n"
		"  FUNCTION com.device_terminal_print $com.terminal -> STR \n"
		"  FUNCTION com.device_terminal_read $com.terminal -> STR ? \n"
		"  FUNCTION com.device_terminal_write $com.terminal STR \n"
		"  FUNCTION com.device_terminal_idle $com.terminal MUTABLE INT MUTABLE INT MUTABLE INT \n"
		"  FUNCTION com.device_terminal_command $com.terminal . * -> VALUE ?\n"
		"  FUNCTION com.device_terminal_close $com.terminal -> BLN \n"
		//"  FUNCTION com.device_buffer_open -> com.buffer \n"
		//"  FUNCTION com.device_buffer_print com.buffer -> STR \n"
		//"  FUNCTION com.device_buffer_read com.buffer -> STR \n"
		//"  FUNCTION com.device_buffer_write com.buffer STR \n"
		//"  FUNCTION com.device_buffer_command com.buffer . * -> VALUE ?\n"
		//"  FUNCTION com.device_buffer_close com.buffer \n"
		"  FUNCTION com.device_file_open [ < > <> >> ] STR [ 'EXEC' 'PRIV' ] * -> $com.file \n"
		"  FUNCTION com.device_file_print $com.file -> STR \n"
		"  FUNCTION com.device_file_read $com.file -> STR ? \n"
		"  FUNCTION com.device_file_write $com.file STR \n"
		"  FUNCTION com.device_file_command $com.file . * -> VALUE ?\n"
		"  FUNCTION com.device_file_close $com.file -> BLN \n"
		"  FUNCTION com.device_tcp_open [ < > ] STR [ STR INT ] -> $com.tcp \n"
		"  FUNCTION com.device_tcp_print $com.tcp -> STR \n"
		"  FUNCTION com.device_tcp_read $com.tcp -> STR ? \n"
		"  FUNCTION com.device_tcp_write $com.tcp STR \n"
		"  FUNCTION com.device_tcp_idle $com.tcp MUTABLE INT MUTABLE INT MUTABLE INT \n"
		"  FUNCTION com.device_tcp_command $com.tcp . * -> VALUE ?\n"
		"  FUNCTION com.device_tcp_close $com.tcp -> BLN \n"
		"  FUNCTION com.protocol_available STR BLN:eof -> INT ? \n"
		"  FUNCTION com.protocol_line STR BLN:eof STR ? -> INT ? \n"
		"  FUNCTION com.protocol_size STR BLN:eof INT -> INT ? \n"
		"  FUNCTION com.protocol_all STR BLN:eof -> INT ? \n"
		"  OPTION com.tcp_queue -t INT \n"
		"",
		SVM_API_SIGNATURE,
		SVM_VERSION);
	}

	void pause_processus_courant(const void *svm)
	{
		SVM_Process processus_courant = ::svm_process_get_current(svm);
		::svm_process_pause(svm,processus_courant);
		::svm_process_interruptionnotification_enable(svm,processus_courant);
	}

	void reprise_processus_courant(const void *svm)
	{
		SVM_Process processus_courant = ::svm_process_get_current(svm);
		::svm_process_interruptionnotification_disable(svm,processus_courant);
		::svm_process_resume(svm,processus_courant);
	}

	std::mutex& verrou_terminal()
	{
		static std::mutex terminal;
		return terminal;
	}

	SVM_Value instruction_message(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		size_t indice = 0;
		bool err = false;
		if(::svm_parameter_type_is_keyword(svm,argv[0]))
		{
			err = std::string(::svm_parameter_keyword_get(svm,argv[0]).string)=="STDERR";
			++indice;
		}
		std::lock_guard<std::mutex> protection(::verrou_terminal());
		for( ; indice<argc ; ++indice)
		{
			SVM_Value v = svm_parameter_value_get(svm,argv[indice]);
			SVM_String s = svm_value_print(svm,v);
			if(err)
			{
				std::cerr << std::string(s.string,s.size);
			}
			else
			{
				std::cout << std::string(s.string,s.size);
			}
		}
		if(err)
		{
			std::cerr << std::endl;
		}
		else
		{
			std::cout << std::endl;
		}
		return nullptr;
	}

	SVM_Value instruction_prompt(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		std::shared_ptr<std::string> prompt;
		bool sortie_erreur = false;
		std::string type;
		size_t indice = 0;
		if(::svm_parameter_type_is_value(svm,argv[0]))
		{
			SVM_String s = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
			prompt = std::make_shared<std::string>(s.string,s.size);
			++indice;
		}
		else
		{
			SVM_String s = ::svm_parameter_keyword_get(svm,argv[0]);
			std::string sortie(s.string,s.size);
			if((sortie=="STDERR") or (sortie=="STDOUT"))
			{
				sortie_erreur = (sortie=="STDERR");
				indice += 2;
				s = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[1]));
				prompt = std::make_shared<std::string>(s.string,s.size);
			}
		}
		SVM_String s2 = ::svm_parameter_keyword_get(svm,argv[indice]);
		type = std::string(s2.string,s2.size);
		{
			std::lock_guard<std::mutex> protection(::verrou_terminal());
			if(static_cast<bool>(prompt))
			{
				if(sortie_erreur)
				{
					std::cerr << (*prompt) << ": ";
				}
				else
				{
					std::cout << (*prompt) << ": ";
				}
			}
		}
		std::string ligne;
		SVM_Process processus_courant = ::svm_process_get_current(svm);
		::svm_process_pause(svm,processus_courant);
		::svm_process_interruptionnotification_enable(svm,processus_courant);
		std::getline(std::cin,ligne);
		::svm_process_interruptionnotification_disable(svm,processus_courant);
		::svm_process_resume(svm,processus_courant);
		if(type=="STR")
		{
			return ::svm_value_string_new__buffer(svm,ligne.c_str(),ligne.size());
		}
		char *fin;
		long int entier = ::strtol(ligne.c_str(),&fin,0);
		if((static_cast<size_t>(fin-ligne.c_str())!=ligne.size()))
		{
			::svm_processor_current_raise_error_internal__raw(svm,NUMERIC,"Input is not an integer number");
		}
		return ::svm_value_integer_new(svm,entier);
	}

	void type_device_delete(const void *svm, void *objet)
	{
		Peripherique *peripherique = reinterpret_cast<Peripherique*>(objet);
		SVM_Parameter* parametres = ::svm_parameter_array_new(svm,1);
		parametres[0] = ::svm_parameter_structure_new(svm,peripherique->_canal);
		SVM_Value_Boolean reussite = ::svm_function_call(svm,peripherique->_fermeture,1,parametres);
		if(::svm_value_boolean_get(svm,reussite))
		{
			::svm_variable_delete(svm,peripherique->_canal);
		}
		::svm_variable_delete(svm,peripherique->_ouverture);
		if(peripherique->_affichage) { ::svm_variable_delete(svm,peripherique->_affichage); }
		if(peripherique->_lecture) { ::svm_variable_delete(svm,peripherique->_lecture); }
		if(peripherique->_ecriture) { ::svm_variable_delete(svm,peripherique->_ecriture); }
		if(peripherique->_attente) { ::svm_variable_delete(svm,peripherique->_attente); }
		if(peripherique->_commande) { ::svm_variable_delete(svm,peripherique->_commande); }
		::svm_variable_delete(svm,peripherique->_fermeture);
		delete peripherique;
	}

	/*
	unsigned char type_device_compare(const void *svm, const void *gauche, const void *droit)
	{
		const Peripherique *g = reinterpret_cast<const Peripherique*>(gauche);
		const Peripherique *d = reinterpret_cast<const Peripherique*>(droit);
		return ((g->_canal==d->_canal)?EQUIVALENCE_EQUAL:EQUIVALENCE_DIFFERENT)|COMPARISON_WEAK;
	}
	*/

	SVM_String type_device_print(const void *svm, void *objet)
	{
		Peripherique *peripherique = reinterpret_cast<Peripherique*>(objet);
		if(peripherique->_affichage)
		{
			SVM_Parameter* parametres = ::svm_parameter_array_new(svm,1);
			parametres[0] = ::svm_parameter_structure_new(svm,peripherique->_canal);
			SVM_Value_String representation = ::svm_function_call(svm,peripherique->_affichage,1,parametres);
			return ::svm_value_string_get(svm,representation);
		}
		else
		{
			return ::svm_string_new__raw(svm,"Shy device");
		}
	}

	void struct_terminal_delete(const void *svm, void *objet)
	{
		Terminal *terminal = reinterpret_cast<Terminal*>(objet);
		delete terminal;
	}

	void struct_file_delete(const void *svm, void *objet)
	{
		Fichier *fichier = reinterpret_cast<Fichier*>(objet);
		delete fichier;
	}

	void struct_tcp_delete(const void *svm, void *objet)
	{
		TCP *tcp = reinterpret_cast<TCP*>(objet);
		delete tcp;
	}

	SVM_Structure function_device_terminal_open(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String parametre = ::svm_parameter_keyword_get(svm,argv[0]);
		std::string nom(parametre.string,parametre.size);
		Terminal *terminal = nullptr;
		if(nom=="STDIN")
		{
			terminal=new Terminal(0,stdin);
			int flags = ::fcntl(terminal->_id, F_GETFL, 0);
			terminal->_bloquant = not(flags bitand O_NONBLOCK);
		}
		else if((nom=="STDOUT") or (nom=="STDERR"))
		{
			terminal=new Terminal((nom=="STDOUT")?1:2,(nom=="STDOUT")?stdout:stderr);
		}
		else
		{
			::svm_processor_current_raise_error_internal__raw(svm,FAILURE,"Unknown device");
		}
		SVM_Structure structure_terminal = ::svm_structure_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),terminal);
		::svm_variable_scope_set_global(svm,structure_terminal);
		return structure_terminal;
	}

	SVM_Value function_device_terminal_print(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Terminal *terminal = reinterpret_cast<Terminal *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),parametre));
		switch(terminal->_id)
		{
			case 0: return ::svm_value_string_new__raw(svm,"Terminal STDIN");
			case 1: return ::svm_value_string_new__raw(svm,"Terminal STDOUT");
			case 2: return ::svm_value_string_new__raw(svm,"Terminal STDERR");
			default: return ::svm_value_string_new__raw(svm,"Invalid terminal");
		}
	}
	
	SVM_Value function_device_terminal_read(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Terminal *terminal = reinterpret_cast<Terminal *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),parametre));
		if(terminal->_id!=0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid read from terminal device.");
		}
		::pause_processus_courant(svm);
		char tampon[1025];
		int lus = ::read(terminal->_id,tampon,1024);
		::reprise_processus_courant(svm);
		if(lus<0)
		{
			if(errno==EINTR)
			{
				::svm_processor_current_raise_error_external__raw(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","interrupted"),"Read interrupted.");
			}
			if(((errno==EAGAIN) or (errno==EWOULDBLOCK)) and not terminal->_bloquant)
			{
				return ::svm_value_string_new__raw(svm,"");
			}
			std::ostringstream oss;
			oss << "Read error on stdin: " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		if(lus==0)
		{
			return ::svm_value_string_new_null(svm);
		}
		return ::svm_value_string_new__buffer(svm,tampon,lus);
	}

	SVM_Value function_device_terminal_write(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Terminal *terminal = reinterpret_cast<Terminal *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),parametre));
		if(terminal->_id==0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid write to terminal device.");
		}
		SVM_String tampon = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[1]));
		ssize_t ecrits = ::write(terminal->_id,tampon.string,tampon.size);
		if(ecrits<0)
		{
			std::ostringstream oss;
			oss << "Write error on std" << ((terminal->_id==1)?"out":"err") << ": " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		if(static_cast<size_t>(ecrits)!=tampon.size)
		{
			std::ostringstream oss;
			oss << "Write error on std" << ((terminal->_id==1)?"out":"err") << ": Incomplete write to device.";
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		return nullptr;
	}

	SVM_Value function_device_terminal_command(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		if((argc-1)%3!=0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
		}
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Terminal *terminal = reinterpret_cast<Terminal *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),parametre));
		for(size_t indice = 1 ; indice<argc ; indice+=3)
		{
			if(not ::svm_parameter_type_is_keyword(svm,argv[indice]))
			{
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
			}
			if(not ::svm_parameter_type_is_marker(svm,argv[indice+1]))
			{
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
			}
			SVM_String separateur = ::svm_parameter_marker_get(svm,argv[indice+1]);
			if(std::string(separateur.string,separateur.size)!="=")
			{
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
			}
			SVM_String commande_brute = ::svm_parameter_keyword_get(svm,argv[indice]);
			std::string commande(commande_brute.string,commande_brute.size);
			if(commande=="BLOCKING")
			{
				SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice+2]);
				if(not ::svm_value_type_is_boolean(svm,valeur))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
				}
				SVM_Boolean booleen = ::svm_value_boolean_get(svm,valeur);
				terminal->_bloquant=booleen==TRUE;
				if(terminal->_bloquant)
				{
					int flags = ::fcntl(terminal->_id, F_GETFL, 0);
					::fcntl(terminal->_id, F_SETFL, flags bitand (compl O_NONBLOCK));
				}
				else
				{
					int flags = ::fcntl(terminal->_id, F_GETFL, 0);
					::fcntl(terminal->_id, F_SETFL, flags bitor O_NONBLOCK);
				}
			}
			if(commande=="BUFFER")
			{
				SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice+2]);
				if(not ::svm_value_type_is_boolean(svm,valeur))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
				}
				SVM_Boolean booleen = ::svm_value_boolean_get(svm,valeur);
				if(booleen)
				{
					struct termios term;
					::tcgetattr(terminal->_id, &term);

					term.c_lflag |= ICANON;

					::tcsetattr(terminal->_id, TCSANOW, &term);
				}
				else
				{
					struct termios term;
					::tcgetattr(terminal->_id, &term);

					term.c_lflag &= ~ICANON;

					::tcsetattr(terminal->_id, TCSANOW, &term);
				}
			}
			if(commande=="ECHO")
			{
				SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice+2]);
				if(not ::svm_value_type_is_boolean(svm,valeur))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
				}
				SVM_Boolean booleen = ::svm_value_boolean_get(svm,valeur);
				if(booleen)
				{
					struct termios term;
					::tcgetattr(terminal->_id, &term);

					term.c_lflag |= ECHO;

					::tcsetattr(terminal->_id, TCSANOW, &term);
				}
				else
				{
					struct termios term;
					::tcgetattr(terminal->_id, &term);

					term.c_lflag &= ~ECHO;

					::tcsetattr(terminal->_id, TCSANOW, &term);
				}
			}
			if(commande=="DELETE")
			{
				SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice+2]);
				if(not ::svm_value_type_is_boolean(svm,valeur))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid terminal command.");
				}
				SVM_Boolean booleen = ::svm_value_boolean_get(svm,valeur);
				terminal->_ferme = (booleen==TRUE);
			}
		}
		return ::svm_value_string_new_null(svm);
	}

	SVM_Value function_device_terminal_idle(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Terminal *terminal = reinterpret_cast<Terminal *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),parametre));
		if(terminal->_id==0)
		{
			SVM_Value_Integer lecture = ::svm_parameter_value_get(svm,argv[1]);
			::svm_value_integer_set(svm,lecture,0);
		}
		else
		{
			SVM_Value_Integer ecriture = ::svm_parameter_value_get(svm,argv[2]);
			::svm_value_integer_set(svm,ecriture,terminal->_id);
		}
		return nullptr;
	}

	SVM_Value function_device_terminal_close(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Terminal *terminal = reinterpret_cast<Terminal *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","terminal"),parametre));
		if(terminal->_ferme)
		{
			struct termios term;
			::tcgetattr(terminal->_id, &term);

			term.c_lflag |= ICANON;
			term.c_lflag |= ECHO;

			::tcsetattr(terminal->_id, TCSANOW, &term);
			::close(terminal->_id);
		}
		return ::svm_value_boolean_new(svm,TRUE);
	}
	
	SVM_Structure function_device_file_open(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String param_mode = ::svm_parameter_marker_get(svm,argv[0]);
		std::string str_mode(param_mode.string,param_mode.size);
		Fichier::Mode mode;
		if(str_mode=="<") mode=Fichier::Mode::LECTURE;
		if(str_mode==">") mode=Fichier::Mode::ECRITURE;
		if(str_mode=="<>") mode=Fichier::Mode::LECTURE_ECRITURE;
		if(str_mode==">>") mode=Fichier::Mode::AJOUT;
		SVM_String str_nom = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[1]));
		std::string nom(str_nom.string,str_nom.size);
		int mode_open;
		switch(mode)
		{
			case Fichier::Mode::LECTURE: mode_open = O_RDONLY; break;
			case Fichier::Mode::ECRITURE: mode_open = O_WRONLY|O_CREAT|O_TRUNC; break;
			case Fichier::Mode::LECTURE_ECRITURE: mode_open = O_RDWR|O_CREAT; break;
			case Fichier::Mode::AJOUT: mode_open = O_WRONLY|O_CREAT|O_APPEND; break;
		}
		int droits = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		for(size_t p = 2 ; p<argc ; ++p)
		{
			SVM_String str_option = ::svm_parameter_keyword_get(svm,argv[p]);
			std::string option(str_option.string,str_option.size);
			if(option=="EXEC")
			{
				droits |= S_IXUSR | S_IXGRP | S_IXOTH;
			}
			if(option=="PRIV")
			{
				droits &= ~ (S_IRWXG | S_IRWXO);
			}
		}
		int id = ::open(nom.c_str(),mode_open,droits);
		if(id<0)
		{
			std::ostringstream oss;
			oss << "Unable to open file " << nom << ": " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		Fichier *fichier = new Fichier(id,nom,mode);
		return ::svm_structure_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","file"),fichier);
	}

	SVM_Value function_device_file_print(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Fichier *fichier = reinterpret_cast<Fichier *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","file"),parametre));
		std::ostringstream oss;
		oss << "File ";
		switch(fichier->_mode)
		{
			case Fichier::Mode::LECTURE: oss << "<"; break;
			case Fichier::Mode::ECRITURE: oss << ">"; break;
			case Fichier::Mode::LECTURE_ECRITURE: oss << "<>"; break;
			case Fichier::Mode::AJOUT: oss << ">>"; break;
		}
		oss << " " << fichier->_fichier << " (" << fichier->_id << ")";
		return ::svm_value_string_new__raw(svm,oss.str().c_str());
	}
	
	SVM_Value function_device_file_read(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Fichier *fichier = reinterpret_cast<Fichier *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","file"),parametre));
		if(not ((fichier->_mode==Fichier::Mode::LECTURE) or (fichier->_mode==Fichier::Mode::LECTURE_ECRITURE)))
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"File mode does not allow read operation.");
		}
		char tampon[1025];
		int lus = ::read(fichier->_id,tampon,1024);
		if(lus<0)
		{
			if(errno==EINTR)
			{
				::svm_processor_current_raise_error_external__raw(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","interrupted"),"Read interrupted.");
			}
			std::ostringstream oss;
			oss << "Read error on file " << fichier->_fichier << ": " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		if(lus==0)
		{
			return ::svm_value_string_new_null(svm);
		}
		return ::svm_value_string_new__buffer(svm,tampon,lus);
	}
	
	SVM_Value function_device_file_write(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Fichier *fichier = reinterpret_cast<Fichier *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","file"),parametre));
		if(not ((fichier->_mode==Fichier::Mode::ECRITURE) or (fichier->_mode==Fichier::Mode::LECTURE_ECRITURE) or (fichier->_mode==Fichier::Mode::AJOUT)))
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"File mode does not allow write operation.");
		}
		SVM_String tampon = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[1]));
		ssize_t ecrits = ::write(fichier->_id,tampon.string,tampon.size);
		if(ecrits<0)
		{
			std::ostringstream oss;
			oss << "Write error to file " << fichier->_fichier << ": " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		if(static_cast<size_t>(ecrits)!=tampon.size)
		{
			std::ostringstream oss;
			oss << "Write error to file " << fichier->_fichier << ": Incomplete write to device.";
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		return nullptr;
	}

	SVM_Value function_device_file_command(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Fichier *fichier = reinterpret_cast<Fichier *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","file"),parametre));
		for(size_t indice = 1 ; indice<argc ; )
		{
			if(not ::svm_parameter_type_is_keyword(svm,argv[indice]))
			{
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid file command.");
			}
			SVM_String commande_brute = ::svm_parameter_keyword_get(svm,argv[indice]);
			std::string commande(commande_brute.string,commande_brute.size);
			if(commande=="SEEK")
			{
				++indice;
				if(indice>=argc)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid file command.");
				}
				int origine = SEEK_CUR;
				if(::svm_parameter_type_is_keyword(svm,argv[indice]))
				{
					SVM_String str_origine = ::svm_parameter_keyword_get(svm,argv[indice]);
					std::string orig(str_origine.string,str_origine.size);
					if(orig=="BEGIN")
					{
						origine = SEEK_SET;
					}
					else if(orig=="END")
					{
						origine = SEEK_END;
					}
					else
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid file command.");
					}
					++indice;
					if(indice>=argc)
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid file command.");
					}
				}
				if(not ::svm_parameter_type_is_value(svm,argv[indice]))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid file command.");
				}
				SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice]);
				if(not ::svm_value_type_is_integer(svm,valeur))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid file command.");
				}
				long long int deplacement = ::svm_value_integer_get(svm,valeur);
				off_t position = ::lseek(fichier->_id,static_cast<off_t>(deplacement),origine);
				if(position<0)
				{
					std::ostringstream oss;
					oss << "Invalid file shift. lseek: " << ::strerror_local(errno);
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
				}
				return ::svm_value_integer_new(svm,position);
				++indice;
			}
		}
		return ::svm_value_integer_new_null(svm);
	}

	SVM_Value function_device_file_close(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		Fichier *fichier = reinterpret_cast<Fichier *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","file"),parametre));
		::close(fichier->_id);
		return ::svm_value_boolean_new(svm,TRUE);
	}

	SVM_Value function_device_tcp_open(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String param_mode = ::svm_parameter_marker_get(svm,argv[0]);
		std::string str_mode(param_mode.string,param_mode.size);
		bool serveur = str_mode == "<";
		SVM_String param_ip = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[1]));
		std::string str_ip(param_ip.string,param_ip.size);
		SVM_Value param_port = ::svm_parameter_value_get(svm,argv[2]);
		std::string str_port;
		if(::svm_value_type_is_integer(svm,param_port))
		{
			long long int int_port = ::svm_value_integer_get(svm,param_port);
			std::ostringstream oss;
			oss << int_port;
			str_port = oss.str();
		}
		else
		{
			SVM_String string_port = ::svm_value_string_get(svm,param_port);
			str_port = std::string(string_port.string,string_port.size);
		}
		int id = 0;
		if(serveur)
		{
			id = TCP::ouvrir<true,true,false>(svm,SOCK_STREAM,str_ip,str_port);
		}
		else
		{
			id = TCP::ouvrir<false,false,true>(svm,SOCK_STREAM,str_ip,str_port);
		}
		TCP* tcp = new TCP(serveur,str_ip,str_port,id);
		return ::svm_structure_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),tcp);
	}

	SVM_Value function_device_tcp_print(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		TCP *tcp = reinterpret_cast<TCP *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),parametre));
		std::ostringstream oss;
		oss << "TCP socket ";
		if(tcp->_serveur)
		{
			oss << "< " << tcp->_ip_locale << ":" << tcp->_port_local << " (" << tcp->_id << ")";
		}
		else
		{
			oss << "> " << (tcp->_auto?"< ":"> ") << tcp->_ip_distante << ":" << tcp->_port_distant << " (" << tcp->_id << ")";
		}
		return ::svm_value_string_new__raw(svm,oss.str().c_str());
	}
	
	SVM_Value function_device_tcp_read(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		TCP *tcp = reinterpret_cast<TCP *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),parametre));
		if(tcp->_serveur)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid read from TCP socket device.");
		}
		::pause_processus_courant(svm);
		char tampon[1025];
		int lus = ::recv(tcp->_id,tampon,1024,0);
		::reprise_processus_courant(svm);
		if(lus<0)
		{
			if(errno==EINTR)
			{
				::svm_processor_current_raise_error_external__raw(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","interrupted"),"Read interrupted.");
			}
			if(((errno==EAGAIN) or (errno==EWOULDBLOCK)) and not tcp->_bloquant)
			{
				return ::svm_value_string_new__raw(svm,"");
			}
			std::ostringstream oss;
			oss << "Read error on TCP socket: " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		if(lus==0)
		{
			return ::svm_value_string_new_null(svm);
		}
		return ::svm_value_string_new__buffer(svm,tampon,lus);
	}

	SVM_Value function_device_tcp_write(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		TCP *tcp = reinterpret_cast<TCP *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),parametre));
		if(tcp->_serveur)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid write to TCP socket device.");
		}
		SVM_String tampon = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[1]));
		ssize_t ecrits = ::send(tcp->_id,tampon.string,tampon.size,MSG_NOSIGNAL);
		if(ecrits<0)
		{
			std::ostringstream oss;
			oss << "Write error on TCP socket: " << ::strerror_local(errno);
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		if(static_cast<size_t>(ecrits)!=tampon.size)
		{
			std::ostringstream oss;
			oss << "Write error on TCP socket: Incomplete write to device.";
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
		}
		return nullptr;
	}

	SVM_Value function_device_tcp_command(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		TCP *tcp = reinterpret_cast<TCP *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),parametre));
		for(size_t indice = 1 ; indice<argc ; )
		{
			if(not ::svm_parameter_type_is_keyword(svm,argv[indice]))
			{
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
			}
			SVM_String commande_brute = ::svm_parameter_keyword_get(svm,argv[indice]);
			std::string commande(commande_brute.string,commande_brute.size);
			if(commande=="CLIENT")
			{
				if(not tcp->_serveur)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid client acceptation from TCP socket device.");
				}
				struct sockaddr client;
				socklen_t longueur = sizeof(client);
				int fd;
				errno=0;
				::pause_processus_courant(svm);
				fd = ::accept(tcp->_id,&client,&longueur);
				::reprise_processus_courant(svm);
				if (fd < 0)
				{
					if(errno==EINTR)
					{
						::svm_processor_current_raise_error_external__raw(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","interrupted"),"Client wait interrupted.");
					}
					if((errno==EAGAIN) or (errno==EWOULDBLOCK))
					{
						return ::svm_value_plugin_new_null(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","device"));
					}
					else
					{
						std::ostringstream details;
						details << "Unable to create client connection." << std::endl << "accept: " << ::strerror_local(errno); 
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,details.str().c_str());
					}
				}
				else
				{
					std::string ip;
					std::string port;
					TCP::resolution_ip_port(&client,longueur,ip,port);
					TCP* tcp_client = new TCP(false,ip,port,fd);
					tcp_client->_auto=true;
					SVM_Structure struct_tcp_client = ::svm_structure_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),tcp_client);
					::svm_variable_scope_set_global(svm,struct_tcp_client);
					SVM_Value_PluginEntryPoint type_peripherique = ::svm_value_pluginentrypoint_new__raw(svm,"com","tcp");
					Peripherique *peripherique = new Peripherique(struct_tcp_client);
					SVM_Value_PluginEntryPoint fonction_fermeture = ::fonction_peripherique(svm,type_peripherique,"close");
					::svm_variable_scope_set_global(svm, fonction_fermeture);
					peripherique->_fermeture=fonction_fermeture;
					SVM_Value_PluginEntryPoint fonction_affichage = ::fonction_peripherique(svm,type_peripherique,"print");
					::svm_variable_scope_set_global(svm, fonction_affichage);
					peripherique->_affichage=fonction_affichage;
					SVM_Value_PluginEntryPoint fonction_lecture = ::fonction_peripherique(svm,type_peripherique,"read");
					::svm_variable_scope_set_global(svm, fonction_lecture);
					peripherique->_lecture=fonction_lecture;
					SVM_Value_PluginEntryPoint fonction_ecriture = ::fonction_peripherique(svm,type_peripherique,"write");
					::svm_variable_scope_set_global(svm, fonction_ecriture);
					peripherique->_ecriture=fonction_ecriture;
					SVM_Value_PluginEntryPoint fonction_commande = ::fonction_peripherique(svm,type_peripherique,"command");
					::svm_variable_scope_set_global(svm, fonction_commande);
					peripherique->_commande=fonction_commande;
					SVM_Value_PluginEntryPoint fonction_attente = ::fonction_peripherique(svm,type_peripherique,"idle");
					::svm_variable_scope_set_global(svm, fonction_attente);
					peripherique->_attente=fonction_attente;
					return ::svm_value_plugin_new(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","device"),peripherique);
				}
				++indice;
			}
			else if(commande=="BLOCKING")
			{
				if(indice+2>=argc)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				if(not ::svm_parameter_type_is_marker(svm,argv[indice+1]))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				SVM_String separateur = ::svm_parameter_marker_get(svm,argv[indice+1]);
				if(std::string(separateur.string,separateur.size)!="=")
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice+2]);
				if(not ::svm_value_type_is_boolean(svm,valeur))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				SVM_Boolean booleen = ::svm_value_boolean_get(svm,valeur);
				if(tcp->_serveur)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid receive mode update to TCP socket device.");
				}
				tcp->_bloquant=booleen==TRUE;
				if(tcp->_bloquant)
				{
					int flags = ::fcntl(tcp->_id, F_GETFL, 0);
					::fcntl(tcp->_id, F_SETFL, flags bitand (compl O_NONBLOCK));
				}
				else
				{
					int flags = ::fcntl(tcp->_id, F_GETFL, 0);
					::fcntl(tcp->_id, F_SETFL, flags bitor O_NONBLOCK);
				}
				indice += 3;
			}
			else if(commande=="CLOSE")
			{
				if(indice+1>=argc)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				if(not ::svm_parameter_type_is_marker(svm,argv[indice+1]))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				if(tcp->_serveur)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid close command to TCP socket device.");
				}
				SVM_String str_sens = ::svm_parameter_marker_get(svm,argv[indice+1]);
				std::string sens(str_sens.string,str_sens.size);
				if(sens==">")
				{
					::shutdown(tcp->_id,SHUT_WR);
					tcp->_fermeture_envoi = true;
				}
				else if(sens=="<")
				{
					::shutdown(tcp->_id,SHUT_RD);
					tcp->_fermeture_reception = true;
				}
				else if(sens=="<>")
				{
					::shutdown(tcp->_id,SHUT_RDWR);
					tcp->_fermeture_envoi = true;
					tcp->_fermeture_reception = true;
				}
				else
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				indice += 2;
			}
			else if(commande=="LINGER")
			{
				if(tcp->_serveur)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid linger command to TCP socket device.");
				}
				linger linger;
				linger.l_onoff = 0;
				linger.l_linger = 0;
				if(indice+1>=argc)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				if(::svm_parameter_type_is_value(svm,argv[indice+1]))
				{
					SVM_Value valeur = ::svm_parameter_value_get(svm,argv[indice+1]);
					if(not ::svm_value_type_is_integer(svm,valeur))
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
					}
					auto l = ::svm_value_integer_get(svm,valeur);
					if(l<0)
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
					}
					linger.l_onoff = 1;
					linger.l_linger = l;
				}
				else if(::svm_parameter_type_is_keyword(svm,argv[indice+1]))
				{
					SVM_String mc = ::svm_parameter_keyword_get(svm,argv[indice+1]);
					if(std::string(mc.string,mc.size)!="OFF")
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
					}
				}
				else
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				if(::setsockopt(tcp->_id,SOL_SOCKET,SO_LINGER,&linger,sizeof(linger)))
				{
					std::ostringstream oss;
					oss	<< "Unable to set linger mode on TCP socket." << std::endl
						<< ::strerror_local(errno) << std::endl;
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
				}
				indice += 2;
			}
			else if(commande=="NODELAY")
			{
				if(tcp->_serveur)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid nodelay command to TCP socket device.");
				}
				if(indice+1>=argc)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				if(not ::svm_parameter_type_is_value(svm,argv[indice+1]))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				SVM_Value v = ::svm_parameter_value_get(svm,argv[indice+1]);
				if(not ::svm_value_type_is_boolean(svm,v))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid TCP socket command.");
				}
				SVM_Boolean b = ::svm_value_boolean_get(svm,v);
				int i = b?1:0;
				::setsockopt( tcp->_id, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
				indice += 2;
			}
			else if(commande=="REMOTE")
			{
				if(tcp->_serveur)
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid remote command to TCP socket device.");
				}
				SVM_Memory_Zone zone = ::svm_memory_zone_new(svm);
				::svm_memory_zone_append_internal__raw(svm,zone,STRING,2);
				SVM_Value_Pointer pointeur = ::svm_memory_allocate(svm,::svm_kernel_get_current(svm),zone);
				SVM_Address adresse = ::svm_value_pointer_get_address(svm,pointeur);
				::svm_memory_write_address(svm,::svm_kernel_get_current(svm),adresse,::svm_value_string_new__buffer(svm,tcp->_ip_distante.c_str(),tcp->_ip_distante.size()));
				::svm_memory_write_address(svm,::svm_kernel_get_current(svm),adresse+1,::svm_value_string_new__buffer(svm,tcp->_port_distant.c_str(),tcp->_port_distant.size()));
				return pointeur;
				++indice;
			}
			else
			{
				std::ostringstream oss;
				oss << "Invalid command " << commande << ".";
				::svm_processor_current_raise_error_internal__raw(svm,FAILURE,oss.str().c_str());
			}
		}
		return ::svm_value_plugin_new_null(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","device"));
	}

	SVM_Value function_device_tcp_idle(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		TCP *tcp = reinterpret_cast<TCP *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),parametre));
		if(tcp->_serveur)
		{
			SVM_Value_Integer lecture = ::svm_parameter_value_get(svm,argv[1]);
			::svm_value_integer_set(svm,lecture,tcp->_id);
		}
		else
		{
			if(not tcp->_fermeture_reception)
			{
				SVM_Value_Integer lecture = ::svm_parameter_value_get(svm,argv[1]);
				::svm_value_integer_set(svm,lecture,tcp->_id);
			}
			if(not tcp->_fermeture_envoi)
			{
				SVM_Value_Integer ecriture = ::svm_parameter_value_get(svm,argv[2]);
				::svm_value_integer_set(svm,ecriture,tcp->_id);
			}
		}
		return nullptr;
	}

	SVM_Value function_device_tcp_close(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure parametre = ::svm_parameter_structure_get(svm,argv[0]);
		TCP *tcp = reinterpret_cast<TCP *>(::svm_structure_get_internal(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp"),parametre));
		if(not tcp->_fermeture_envoi)
		{
			::shutdown(tcp->_id,SHUT_WR);
		}
		if(not tcp->_fermeture_reception)
		{
			::shutdown(tcp->_id,SHUT_RD);
		}
		::close(tcp->_id);
		return ::svm_value_boolean_new(svm,TRUE);
	}

	SVM_Value function_protocol_available(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String tampon = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		if((::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[1]))) and (tampon.size==0))
		{
			return ::svm_value_integer_new_null(svm);
		}
		return ::svm_value_integer_new(svm,tampon.size);
	}

	SVM_Value function_protocol_line(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String tampon = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		if((::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[1]))) and (tampon.size>0))
		{
			return ::svm_value_integer_new(svm,tampon.size);
		}
		char delimiteur = '\n';
		if(argc>2)
		{
			SVM_String delim = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[2]));
			if(delim.size>=1)
			{
				delimiteur = delim.string[0];
			}
		}
		size_t ligne = std::string(tampon.string,tampon.size).find(delimiteur);
		if(ligne==std::string::npos)
		{
			return ::svm_value_integer_new_null(svm);
		}
		return ::svm_value_integer_new(svm,ligne+1);
	}
	
	SVM_Value function_protocol_size(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_String tampon = ::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0]));
		SVM_Boolean fin = ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[1]));
		long int taille = ::svm_value_integer_get(svm,::svm_parameter_value_get(svm,argv[2]));
		if(taille<=0)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Size protocol with negative or zero integer.");
		}
		if(tampon.size>=static_cast<size_t>(taille))
			return ::svm_value_integer_new(svm,taille);
		if(fin)
		{
			return ::svm_value_integer_new(svm,-tampon.size);
		}
		return ::svm_value_integer_new_null(svm);
	}

	SVM_Value function_protocol_all(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		if(not ::svm_value_boolean_get(svm,::svm_parameter_value_get(svm,argv[1])))
		{
			return ::svm_value_integer_new_null(svm);
		}
		return ::svm_value_integer_new(svm,::svm_value_string_get(svm,::svm_parameter_value_get(svm,argv[0])).size);
	}
	
	SVM_Value instruction_open(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_PluginEntryPoint type_peripherique = ::svm_parameter_value_get(svm,argv[0]);
		SVM_Value_PluginEntryPoint fonction_ouverture = ::fonction_peripherique(svm,type_peripherique,"open");
		if(not ::svm_plugin_has_function(svm,fonction_ouverture,argc-1,argv+1,::svm_structure_new_null(svm,type_peripherique)))
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Unable to open device.");
		}
		SVM_Value_PluginEntryPoint fonction_fermeture = ::fonction_peripherique(svm,type_peripherique,"close");
		SVM_Parameter *parametres = ::svm_parameter_array_new(svm,4);
		parametres[0] = ::svm_parameter_structure_new(svm,::svm_structure_new_null(svm,type_peripherique));
		if(not ::svm_plugin_has_function(svm,fonction_fermeture,1,parametres,::svm_parameter_value_new(svm,::svm_value_boolean_new(svm,TRUE))))
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid device: no close operation defined.");
		}
		SVM_Structure canal = ::svm_function_call(svm,fonction_ouverture,argc-1,argv+1);
		::svm_variable_scope_set_global(svm,canal);
		Peripherique *peripherique = new Peripherique(canal);
		peripherique->_ouverture = fonction_ouverture;
		::svm_variable_scope_set_global(svm,fonction_ouverture);
		peripherique->_fermeture = fonction_fermeture;
		::svm_variable_scope_set_global(svm,fonction_fermeture);
		return ::fonctions_facultatives_peripherique(svm,type_peripherique,peripherique,parametres);
	}

	SVM_Value instruction_read(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		Peripherique *vrai_peripherique = reinterpret_cast<Peripherique*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0])));
		SVM_Value_PluginEntryPoint fonction_lecture = vrai_peripherique->_lecture;
		if(not fonction_lecture)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Device does not support read operation.");
		}
		SVM_Value_PluginEntryPoint protocole = ::svm_parameter_value_get(svm,argv[1]);
		SVM_Value_PluginEntryPoint fonction_protocole = ::svm_value_pluginentrypoint_new_prefix(svm,protocole,"protocol");
		SVM_Parameter *parametres_lecture = ::svm_parameter_array_new(svm,1);
		parametres_lecture[0] = ::svm_parameter_structure_new(svm,vrai_peripherique->_canal);
		SVM_Parameter *parametres_protocole = ::svm_parameter_array_new(svm,argc);
		parametres_protocole[0] = ::svm_parameter_value_new(svm,::svm_value_string_new_null(svm));
		parametres_protocole[1] = ::svm_parameter_value_new(svm,::svm_value_boolean_new_null(svm));
		for(size_t p = 2 ; p<argc ; ++p)
		{
			parametres_protocole[p] = argv[p];
		}
		if(not ::svm_plugin_has_function(svm,fonction_protocole,argc,parametres_protocole,::svm_parameter_value_new(svm,::svm_value_integer_new_null(svm))))
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid protocol.");
		}
		std::lock_guard<std::mutex> protection(vrai_peripherique->_acces_tampon);
		SVM_Boolean eof = FALSE;
		bool lecture = false;
		for( ; ; )
		{
			parametres_protocole[0] = ::svm_parameter_value_new(svm,::svm_value_string_new__buffer(svm,vrai_peripherique->_tampon.str().c_str(),vrai_peripherique->_tampon.str().size()));
			parametres_protocole[1] = ::svm_parameter_value_new(svm,::svm_value_boolean_new(svm,eof));
			SVM_Value_Integer bloc = ::svm_function_call(svm,fonction_protocole,argc,parametres_protocole);
			if(not ::svm_value_state_is_null(svm,bloc))
			{
				long int taille_bloc = ::svm_value_integer_get(svm,bloc);
				if(taille_bloc<0)
				{
					vrai_peripherique->_tampon.str(vrai_peripherique->_tampon.str().substr(-taille_bloc));
					vrai_peripherique->_tampon.seekp(0,std::ios_base::end);
					continue;
				}
				else
				{
					if(lecture or (taille_bloc>0))
					{
						SVM_Value_String resultat = ::svm_value_string_new__buffer(svm,vrai_peripherique->_tampon.str().substr(0,taille_bloc).c_str(),taille_bloc);
						vrai_peripherique->_tampon.str(vrai_peripherique->_tampon.str().substr(taille_bloc));
						vrai_peripherique->_tampon.seekp(0,std::ios_base::end);
						return resultat;
					}
				}
			}
			if(eof)
			{
				return ::svm_value_string_new_null(svm);
			}
			SVM_Value_String lu = ::svm_function_call(svm,fonction_lecture,1,parametres_lecture);
			lecture = true;
			if(::svm_value_state_is_null(svm,lu))
			{
				eof = TRUE;
				continue;
			}
			SVM_String str_lu = ::svm_value_string_get(svm,lu);
			vrai_peripherique->_tampon << std::string(str_lu.string,str_lu.size);
		}
		return nullptr;
	}

	SVM_Value instruction_write(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		Peripherique *vrai_peripherique = reinterpret_cast<Peripherique*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0])));
		SVM_Value_PluginEntryPoint fonction_ecriture = vrai_peripherique->_ecriture;
		if(not fonction_ecriture)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Device does not support write operation.");
		}
		SVM_Parameter *parametres_ecriture = ::svm_parameter_array_new(svm,2);
		parametres_ecriture[0] = ::svm_parameter_structure_new(svm,vrai_peripherique->_canal);
		for(size_t p = 1 ; p<argc ; ++p)
		{
			parametres_ecriture[1] = ::svm_parameter_value_new(svm,::svm_value_string_new(svm,::svm_value_print(svm,::svm_parameter_value_get(svm,argv[p]))));
			::svm_function_call(svm,fonction_ecriture,2,parametres_ecriture);
		}
		return nullptr;
	}

	SVM_Value instruction_command(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		Peripherique *vrai_peripherique = reinterpret_cast<Peripherique*>(::svm_value_plugin_get_internal(svm,::svm_parameter_value_get(svm,argv[0])));
		argv[0] = ::svm_parameter_structure_new(svm,vrai_peripherique->_canal);
		SVM_Value_PluginEntryPoint fonction_commande = vrai_peripherique->_commande;
		if(not fonction_commande)
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Device does not support command operation.");
		}
		SVM_Variable retour = ::svm_function_call(svm,fonction_commande,argc,argv);
		if(not ::svm_variable_type_is_value(svm,retour))
		{
			::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid return from command operation.");
		}
		return retour;
	}

	void enregistre_peripherique(const void *svm, SVM_Value_Plugin peripherique, const std::string& mode, SVM_Value protocole, SVM_Size taille_parametres, SVM_Parameter *parametres, std::map<int,Attente>& lecture, std::map<int,Attente>& ecriture, fd_set& id_lecture, fd_set& id_ecriture, int& fd_max, SVM_Value representant)
	{
		Peripherique *vrai_peripherique = reinterpret_cast<Peripherique*>(::svm_value_plugin_get_internal(svm,peripherique));
		if(vrai_peripherique->_attente)
		{
			SVM_Value_Integer id_peripherique_lecture = ::svm_value_integer_new_null(svm);
			SVM_Value_Integer id_peripherique_ecriture = ::svm_value_integer_new_null(svm);
			SVM_Parameter* parametres_attente = ::svm_parameter_array_new(svm,4);
			parametres_attente[0] = ::svm_parameter_structure_new(svm,vrai_peripherique->_canal);
			parametres_attente[1] = ::svm_parameter_value_new(svm,id_peripherique_lecture);
			parametres_attente[2] = ::svm_parameter_value_new(svm,id_peripherique_ecriture);
			parametres_attente[3] = ::svm_parameter_value_new(svm,::svm_value_integer_new_null(svm));
			::svm_function_call(svm,vrai_peripherique->_attente,4,parametres_attente);
			if(mode=="<")
			{
				if(not ::svm_value_state_is_null(svm,id_peripherique_lecture))
				{
					long long int id = ::svm_value_integer_get(svm,id_peripherique_lecture);
					if(id>=fd_max) fd_max=id;
					auto it = lecture.insert(std::make_pair(id,Attente(id,vrai_peripherique,protocole,taille_parametres,parametres,representant)));
					if(not it.second)
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Duplicate read device in idle operation.");
					}
					FD_SET(id,&id_lecture);
				}
			}
			else
			{
				if(not ::svm_value_state_is_null(svm,id_peripherique_ecriture))
				{
					long long int id = ::svm_value_integer_get(svm,id_peripherique_ecriture);
					if(id>=fd_max) fd_max=id;
					auto it = ecriture.insert(std::make_pair(id,Attente(id,vrai_peripherique,representant)));
					if(not it.second)
					{
						::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Duplicate write device in idle operation.");
					}
					FD_SET(id,&id_ecriture);
				}
			}
		}
	}

	SVM_Value instruction_idle(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		std::map<int,Attente> lecture;
		std::map<int,Attente> ecriture;
		std::vector<std::unique_ptr<std::lock_guard<std::mutex> > > verrous;
		fd_set id_lecture;
		fd_set id_ecriture;
		FD_ZERO(&id_lecture);
		FD_ZERO(&id_ecriture);
		int fd_max=0;
		for(size_t indice = 0 ; indice<argc ; )
		{
			SVM_String str_mode = ::svm_parameter_marker_get(svm,argv[indice]);
			std::string mode(str_mode.string,str_mode.size);
			SVM_Value_PluginEntryPoint protocole = nullptr;
			SVM_Size taille_parametres = 2;
			SVM_Parameter *parametres = nullptr;
			if(mode=="<")
			{
				++indice;
				if(::svm_parameter_type_is_marker(svm,argv[indice]))
				{
					++indice;
					protocole = ::svm_parameter_value_get(svm,argv[indice]);
					protocole = ::svm_value_pluginentrypoint_new_prefix(svm,protocole,"protocol");
					std::vector<SVM_Parameter> parametres_temporaires;
					for( ; ; ++indice )
					{
						if(::svm_parameter_type_is_marker(svm,argv[indice]))
							break;
						parametres_temporaires.push_back(argv[indice]);
					}
					parametres = ::svm_parameter_array_new(svm,2+parametres_temporaires.size());
					parametres[0] = ::svm_parameter_value_new(svm,::svm_value_string_new_null(svm));
					parametres[1] = ::svm_parameter_value_new(svm,::svm_value_boolean_new_null(svm));
					size_t i = 1;
					for(auto& p:parametres_temporaires)
					{
						parametres[++i]=p;
					}
					taille_parametres=2+parametres_temporaires.size();
					++indice;
				}
				else if(::svm_value_type_is_pluginentrypoint(svm,::svm_parameter_value_get(svm,argv[indice])))
				{
					protocole = ::svm_parameter_value_get(svm,argv[indice]);
					protocole = ::svm_value_pluginentrypoint_new_prefix(svm,protocole,"protocol");
					parametres = ::svm_parameter_array_new(svm,2);
					parametres[0] = ::svm_parameter_value_new(svm,::svm_value_string_new_null(svm));
					parametres[1] = ::svm_parameter_value_new(svm,::svm_value_boolean_new_null(svm));
					++indice;
				}
				if(protocole and not ::svm_plugin_has_function(svm,protocole,taille_parametres,parametres,::svm_parameter_value_new(svm,::svm_value_integer_new_null(svm))))
				{
					::svm_processor_current_raise_error_internal__raw(svm,DEVICE,"Invalid protocol.");
				}
			}
			SVM_Kernel noyau = ::svm_kernel_get_current(svm);
			for( ; ; ++indice)
			{
				if(indice>=argc)
					break;
				if(::svm_parameter_type_is_marker(svm,argv[indice]))
					break;
				SVM_Value_Pointer valeur = ::svm_parameter_value_get(svm,argv[indice]);
				SVM_Address debut = ::svm_value_pointer_get_address(svm,valeur);
				SVM_Address fin = debut+::svm_value_pointer_get_size(svm,valeur);
				SVM_Value_PluginEntryPoint comdevice = ::svm_value_pluginentrypoint_new__raw(svm,"com","device");
				for(SVM_Address adresse=debut ; adresse<fin ; ++adresse)
				{
					if(not ::svm_memory_address_is_defined(svm,noyau,adresse))
						continue;
					if(not ::svm_memory_address_is_initialised(svm,noyau,adresse))
						continue;
					SVM_Type type = ::svm_memory_address_get_type(svm,noyau,adresse);
					if(not ::svm_type_equal_external(svm,type,comdevice))
						continue;
					SVM_Value_Plugin peripherique = ::svm_memory_read_address(svm,noyau,adresse);
					SVM_Value_Pointer representant = ::svm_value_pointer_new__raw(svm,adresse,1);
					enregistre_peripherique(svm,peripherique,mode,protocole,taille_parametres,parametres,lecture,ecriture,id_lecture,id_ecriture,fd_max,representant);
				}
			}
			if(indice>=argc)
				break;
		}
		for(auto& l: lecture)
		{
			verrous.push_back(std::make_unique<std::lock_guard<std::mutex> >(l.second._peripherique->_acces_tampon));
		}
		++fd_max;
		for( ; ; )
		{
			for(auto& p: lecture)
			{
				if(not p.second._protocole)
					continue;
				p.second._parametres[0] = ::svm_parameter_value_new(svm,::svm_value_string_new__buffer(svm,p.second._peripherique->_tampon.str().c_str(),p.second._peripherique->_tampon.str().size()));
				p.second._parametres[1] = ::svm_parameter_value_new(svm,::svm_value_boolean_new(svm,p.second._eof?TRUE:FALSE));
				for( ; ; )
				{
					SVM_Value_Integer bloc = ::svm_function_call(svm,p.second._protocole,p.second._taille_parametres,p.second._parametres);
					if(::svm_value_state_is_null(svm,bloc))
					{
						if(p.second._eof)
							return p.second._representant;
						break;
					}
					long long int taille = ::svm_value_integer_get(svm,bloc);
					if(taille<0)
					{
						p.second._peripherique->_tampon.str(p.second._peripherique->_tampon.str().substr(-taille));
						p.second._peripherique->_tampon.seekp(0,std::ios_base::end);
						continue;
					}
					if(taille==0)
						break;
					return p.second._representant;
				}
			}
			::pause_processus_courant(svm);
			int r = ::select(fd_max,&id_lecture,&id_ecriture,nullptr,nullptr);
			::reprise_processus_courant(svm);
			if(r<0)
			{
				if(errno==EINTR)
				{
					::svm_processor_current_raise_error_external__raw(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","interrupted"),"Idle interrupted.");
				}

				std::ostringstream oss;
				oss << "Idle failed. select: " << ::strerror_local(errno);
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
			}
			for(int i=0 ; i<fd_max ; ++i)
			{
				if(FD_ISSET(i,&id_lecture))
				{
					Attente& a = lecture.find(i)->second;
					if(not a._protocole)
					{
						return a._representant;
					}
					SVM_Parameter *p = ::svm_parameter_array_new(svm,1);
					p[0] = ::svm_parameter_structure_new(svm,a._peripherique->_canal);
					SVM_Value_String lu = ::svm_function_call(svm,a._peripherique->_lecture,1,p);
					if(::svm_value_state_is_null(svm,lu))
					{
						a._eof = true;
						continue;
					}
					SVM_String str_lu = ::svm_value_string_get(svm,lu);
					a._peripherique->_tampon << std::string(str_lu.string,str_lu.size);
				}
			}
			for(int i=0 ; i<fd_max ; ++i)
			{
				if(FD_ISSET(i,&id_ecriture))
				{
					Attente& a = ecriture.find(i)->second;
					return a._representant;
				}
			}
		}
		return nullptr;
	}

	SVM_Value instruction_equal(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Value_Plugin peripherique = ::svm_parameter_value_get(svm,argv[0]);
		Peripherique *gauche = reinterpret_cast<Peripherique*>(::svm_value_plugin_get_internal(svm,peripherique));
		peripherique = ::svm_parameter_value_get(svm,argv[1]);
		Peripherique *droite = reinterpret_cast<Peripherique*>(::svm_value_plugin_get_internal(svm,peripherique));
		return ::svm_value_boolean_new__raw(svm,gauche->_canal==droite->_canal);
	}

	SVM_Variable function_device(const void *svm, SVM_Size argc, SVM_Parameter argv[])
	{
		SVM_Structure canal = ::svm_parameter_structure_get(svm,argv[0]);
		SVM_Value_PluginEntryPoint type_peripherique = ::svm_structure_get_type(svm,canal);
		SVM_Value_PluginEntryPoint fonction_fermeture = ::fonction_peripherique(svm,type_peripherique,"close");
		SVM_Parameter *parametres = ::svm_parameter_array_new(svm,4);
		parametres[0] = ::svm_parameter_structure_new(svm,::svm_structure_new_null(svm,type_peripherique));
		if(not ::svm_plugin_has_function(svm,fonction_fermeture,1,parametres,::svm_parameter_value_new(svm,::svm_value_boolean_new(svm,TRUE))))
		{
			return ::svm_value_plugin_new_null(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","device"));
		}
		Peripherique *peripherique = new Peripherique(canal);
		::svm_variable_scope_set_global(svm,canal);
		peripherique->_fermeture = fonction_fermeture;
		::svm_variable_scope_set_global(svm,fonction_fermeture);
		return ::fonctions_facultatives_peripherique(svm,type_peripherique,peripherique,parametres);
	}

	void plugin_initialisation(const void *svm)
	{
		SVM_Value_Integer file = ::svm_plugin_get_option(svm,::svm_value_pluginentrypoint_new__raw(svm,"com","tcp_queue"));
		if(not ::svm_value_state_is_null(svm,file))
		{
			auto ifile = ::svm_value_integer_get(svm,file);
			if((ifile>0) and (ifile<=SOMAXCONN))
			{
				tcp_file = static_cast<int>(ifile);
			}
			else
			{
				std::ostringstream oss;
				oss << "Invalid TCP listening queue size, " << ifile << " not between 1 and " << SOMAXCONN;
				::svm_processor_current_raise_error_internal__raw(svm,DEVICE,oss.str().c_str());
			}
		}
	}
}
