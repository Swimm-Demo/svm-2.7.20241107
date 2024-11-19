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

#include <src/global/global.h>

#include <src/demarrage/lanceur/analyseur/analyseur.h>


int main(int argc, char *argv[])
{

//	std::cerr << "argc=" << argc << std::endl;
//	std::cerr << "argv:" << std::endl;
//	for(int i=0 ; i<argc ; ++i)
//	{
//		std::cerr << "[" << i << "] " << argv[i] << std::endl;
//	}
	int code_retour = 0;
	if(argc<2)
	{
		std::cerr << "Boot error: Missing file name in command line" << std::endl;
		return 126;
	}

	SVM::Demarrage::Lanceur::LanceurSP lanceur;
	SVM_Machine::MachineSP machine;
	try
	{
		std::string fichier(argv[1]);
		if(fichier=="-h")
		{
			lanceur = std::make_shared<SVM::Demarrage::Lanceur::Lanceur>(std::string());
			lanceur->aide(std::cerr,argv[0]);
		}
		else if(fichier=="-v")
		{
			lanceur = std::make_shared<SVM::Demarrage::Lanceur::Lanceur>(std::string());
			lanceur->version(std::cerr,argv[0]);
		}
		else
		{
			lanceur = SVM::Demarrage::Lanceur::Analyseur::analyse(fichier);
			machine = lanceur->machine_virtuelle(argc,argv);
			if(not machine)
			{
				code_retour = 126;
			}
			else
			{
				code_retour = machine->execution();
			}
		}
	}
	catch(SVM::Demarrage::Lanceur::Erreur& erreur)
	{
		std::cerr << erreur << std::endl << std::endl;
		if(static_cast<bool>(lanceur))
		{
			lanceur->aide(std::cerr);
		}
		code_retour = 127;
	}
	catch(SVM_Valeur::ExceptionExecution& e)
	{
		SVM_Valeur::Interruption i(e);
		i.erreur(std::cerr);
		std::cerr << std::endl;
		code_retour = 126;
	}
	catch(SVM_Valeur::Interruption& i)
	{	
		i.erreur(std::cerr);
		std::cerr << std::endl;
		code_retour = 126;
	}
	catch(SVM_Valeur::ExceptionDemarrage& e)
	{
		std::cerr << e << std::endl;
		code_retour = 126;
	}
	
	SVM_TRACE("Fin de la VM detectee : " << code_retour);
#ifdef LINUX_EXTENSIONS
	if(not code_retour)
	{
		::pthread_exit(nullptr);
	}
#endif
	return code_retour;
}
