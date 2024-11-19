---
title: Basic Concepts of Process Management
---
## Overview

Process management involves the creation, execution, and termination of processes within a virtual machine environment. The <SwmToken path="src/machine/elements/processus/processus.h" pos="95:3:3" line-data="	struct Processus : public std::enable_shared_from_this&lt;Processus&gt;">`Processus`</SwmToken> class is central to this functionality, handling the lifecycle of a process, including its creation, execution, and termination.

## Processus Class

The <SwmToken path="src/machine/elements/processus/processus.h" pos="95:3:3" line-data="	struct Processus : public std::enable_shared_from_this&lt;Processus&gt;">`Processus`</SwmToken> class is defined in <SwmPath>[src/machine/elements/processus/processus.h](src/machine/elements/processus/processus.h)</SwmPath>. It includes methods for creating, executing, and managing the lifecycle of a process. The class also handles interruptions and errors during process execution.

<SwmSnippet path="/src/machine/elements/processus/processus.h" line="95">

---

The <SwmToken path="src/machine/elements/processus/processus.h" pos="95:3:3" line-data="	struct Processus : public std::enable_shared_from_this&lt;Processus&gt;">`Processus`</SwmToken> class definition includes several friend structures and static methods for creating processes. These methods allow for different configurations and parameters for process creation.

```c
	struct Processus : public std::enable_shared_from_this<Processus>
	{
		friend struct PiegeProcessus;
		friend struct SVM::Machine::Interface::Outils;
		friend struct SVM::Machine::Interface::VerrouProcessus;
		friend struct SVM::Machine::Interface::Fonction;
		friend struct SVM::Machine::Interface::VariableProcessus;
		friend struct Processeur::Instruction::Extension;
		friend struct Ordonnanceur::Ordonnanceur;
		friend struct Ordonnanceur::OrdonnanceurDefaut;
		friend struct Ordonnanceur::OrdonnanceurExtension;
		friend struct Systeme::Systeme;
		static ProcessusSP creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, SVM_Noyau::NoyauSP& noyau);
		static ProcessusSP creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, SVM_Valeur::CodeSP& code, SVM_Memoire::MemoireSP& memoire, const bool transmet_interruption = true, const bool retour_est_arret = false, const bool mode_protege = false, const SVM_Noyau::ControleAccesSP& controle_acces = SVM_Noyau::ControleAccesSP());
		static ProcessusSP creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, const SVM_Valeur::AdresseInstruction& code, SVM_Memoire::MemoireSP& memoire, const bool transmet_interruption = true, const bool retour_est_arret = false, const bool mode_protege = false, const SVM_Noyau::ControleAccesSP& controle_acces = SVM_Noyau::ControleAccesSP());
		Processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const bool terminaison_automatique);
		Processus(const Processus&) = delete;
		~Processus();
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Processus& p)
		{
```

---

</SwmSnippet>

## Execution Method

The <SwmToken path="src/machine/elements/processus/processus.cpp" pos="87:4:4" line-data="void Processus::execution()">`execution`</SwmToken> method is responsible for running the process in a separate thread and managing its state transitions. This method ensures that the process operates independently and handles its state changes appropriately.

<SwmSnippet path="/src/machine/elements/processus/processus.cpp" line="87">

---

The <SwmToken path="src/machine/elements/processus/processus.cpp" pos="87:4:4" line-data="void Processus::execution()">`execution`</SwmToken> method starts a new thread for the process and manages its state transitions. It includes error handling to manage interruptions during execution.

```c++
void Processus::execution()
{
	try
	{
		_travailleur = std::thread([this] ()
		{
		SVM_TRACE("PROCESS " << _nom << " depart travailleur");
		std::lock_guard<std::mutex> protege(_execution);
		if(not _etat.executable())
			return;
		SVM_Valeur::InterruptionSP interruption_transmise;
		for( ; ; )
		{
			
			try
			{
				_noyau_courant = _sequenceur->noyau_courant();
				if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
			}
			catch(SVM_Valeur::Interruption& i)
			{
```

---

</SwmSnippet>

## Interactions Method

The <SwmToken path="src/machine/elements/processus/processus.cpp" pos="240:4:4" line-data="void Processus::interactions()">`interactions`</SwmToken> method manages the interactions and state changes of the process based on events. This method ensures that the process responds to events and updates its state accordingly.

<SwmSnippet path="/src/machine/elements/processus/processus.cpp" line="240">

---

The <SwmToken path="src/machine/elements/processus/processus.cpp" pos="240:4:4" line-data="void Processus::interactions()">`interactions`</SwmToken> method handles the process's interactions and state changes based on events. It includes logging and synchronization mechanisms to manage the process state.

```c++
void Processus::interactions()
{
	SVM_TRACE("PROCESS " << _nom << " INTERACTIONS");
	for(;;)
	{
		SVM_TRACE("PROCESS " << _nom << " TENTATIVE");
		if((_etat.etat_global()==Etat::Execution::EXECUTION) and (_notification.load(std::memory_order_relaxed)==0))
		{
			SVM_TRACE("PROCESS " << _nom << " EXECUTION");
			return;
		}
		SVM_TRACE("PROCESS " << _nom << " ATTENTE " << (_notification) << " ETAT COURANT: " << Etat::texte(_etat.etat_global()));
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere(_adresse,source,evenement))
			continue;
		--_notification;
		if(_notification<0)
		{
			SVM_TRACE("Nombre de notifications " << _notification << " invalide pour " << *_adresse );
		}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
