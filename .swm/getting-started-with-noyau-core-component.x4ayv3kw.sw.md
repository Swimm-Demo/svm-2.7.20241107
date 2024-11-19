---
title: Getting Started with Noyau Core Component
---
## Overview

Noyau is a core component within the Machine Elements of the SVM project. It is responsible for managing the state and execution of processes within the virtual machine. Noyau interacts with other components like the Processeur and Sequenceur to coordinate the execution flow and handle interruptions.

## Noyau Structure

The Noyau structure includes several important members such as <SwmToken path="src/machine/elements/noyau/noyau.cpp" pos="56:9:9" line-data="	bool fin = not _etat.execution(_processus.lock(),changement);">`_etat`</SwmToken>, which holds the current state of execution, and <SwmToken path="src/machine/elements/noyau/noyau.h" pos="143:13:13" line-data="		ControleAccesSP controle_acces() const { return _controle_acces; } // pour API">`_controle_acces`</SwmToken>, which manages access control. The <SwmToken path="src/machine/elements/noyau/noyau.h" pos="103:3:3" line-data="	DECL_SHARED_PTR(PiegeNoyau);">`PiegeNoyau`</SwmToken> structure within Noyau is used for debugging purposes, allowing the state of the Noyau to be updated and cascaded.

<SwmSnippet path="/src/machine/elements/noyau/noyau.h" line="114">

---

The Noyau structure includes several important members and friends that interact with various components and instructions within the SVM project.

```c
	struct Noyau : public std::enable_shared_from_this<Noyau>
	{
		friend struct Sequenceur::SequenceurDefaut;
		friend struct Sequenceur::SequenceurExtension;
		friend struct SVM_Processeur::Processeur;
		friend struct SVM_Instruction::AffectationValeur;
		friend struct SVM_Instruction::AffectationBloc;
		friend struct SVM_Instruction::AffectationInstruction;
		friend struct SVM_Instruction::Appel;
		friend struct SVM_Instruction::ConditionAdresseDefinie;
		friend struct SVM_Instruction::ConditionAdresseInitialisee;
		friend struct SVM_Instruction::ConditionDrapeau;
		friend struct SVM_Instruction::Decalage;
		friend struct SVM_Instruction::Drapeau;
		friend struct SVM_Instruction::Interruption;
		friend struct SVM_Instruction::Local;
		friend struct SVM_Instruction::Memoire;
		friend struct SVM_Instruction::Retour;
		friend struct SVM_Instruction::Saut;
		friend struct SVM_Programme::LectureMemoire;
		friend struct SVM_Programme::PointeurAlias;
```

---

</SwmSnippet>

## Noyau Execution

The <SwmToken path="src/machine/elements/noyau/noyau.cpp" pos="52:7:7" line-data="const Etat&amp; Noyau::execution()">`execution`</SwmToken> method in Noyau coordinates the execution flow and handles interruptions. It ensures that the state of the Noyau is properly managed and that the appropriate execution mode is selected based on the current state.

<SwmSnippet path="/src/machine/elements/noyau/noyau.cpp" line="52">

---

The <SwmToken path="src/machine/elements/noyau/noyau.cpp" pos="52:7:7" line-data="const Etat&amp; Noyau::execution()">`execution`</SwmToken> method locks the execution state, checks for changes, and determines the appropriate execution mode (protected or normal) based on the current state.

```c++
const Etat& Noyau::execution()
{
	std::lock_guard<std::mutex> protege(_execution);
	bool changement;
	bool fin = not _etat.execution(_processus.lock(),changement);
	if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
	if(fin)
	{
		return _etat;
	}
	if(_etat._mode_protege)
	{
		_processeur->execution<SVM_Processeur::Processeur::Mode::PROTEGE>(this->shared_from_this());
	}
	else
	{
		_processeur->execution<SVM_Processeur::Processeur::Mode::NORMAL>(this->shared_from_this());
	}
	return _etat;
}
```

---

</SwmSnippet>

## Noyau Trace

The <SwmToken path="src/machine/elements/noyau/noyau.cpp" pos="112:4:4" line-data="void Noyau::trace() const">`trace`</SwmToken> method in Noyau logs the state of the Noyau when an interruption occurs, providing a core dump for debugging. This method is crucial for diagnosing issues within the virtual machine by capturing the state at the time of interruption.

<SwmSnippet path="/src/machine/elements/noyau/noyau.cpp" line="112">

---

The <SwmToken path="src/machine/elements/noyau/noyau.cpp" pos="112:4:4" line-data="void Noyau::trace() const">`trace`</SwmToken> method captures the state of the Noyau and logs it for debugging purposes, providing a detailed core dump.

```c++
void Noyau::trace() const
{
	auto processus = _processus.lock();
	if(not processus) return;
	auto systeme = processus->_systeme.lock();
	if(not systeme) return;
	auto machine = systeme->_machine.lock();
	if(not machine) return;
	std::ostringstream oss;
	processus->nom_flux(oss);
	SVM::Machine::Trace trace(machine->_historique,oss.str());
	trace << "Kernel interrupted: ";
	_etat.interrompu()->erreur(trace);
	trace << "" << std::endl << std::endl << "Core dump:" << std::endl << (*this);
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
