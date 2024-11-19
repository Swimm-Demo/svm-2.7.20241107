---
title: Introduction to Ordonnanceur
---
# Introduction to Scheduler

The Ordonnanceur is a component responsible for managing the scheduling and execution of processes within the system. It initializes by creating a delivery address and a message box for communication. The execution method runs in a separate thread, handling events such as process state changes, attachments, and detachments. It interacts with other components like the system and debugger to ensure proper process management and error handling. The Ordonnanceur also includes mechanisms for notifying state changes and handling process locking.

## Ordonnanceur Initialization

The Ordonnanceur initializes by creating a delivery address and a message box for communication.

<SwmSnippet path="/src/machine/elements/ordonnanceur/ordonnanceur.cpp" line="27">

---

The constructor of the Ordonnanceur class initializes the scheduler with a name and a system reference. It also sets up the delivery mechanism for communication.

```c++
Ordonnanceur::Ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme)
	:_nom(nom), _systeme(systeme), _livraison(systeme->_livraison)
	{
		ASSERT(systeme,"ordonnanceur sans systeme");
	}
```

---

</SwmSnippet>

## Ordonnanceur Execution

The execution method runs in a separate thread, handling events such as process state changes, attachments, and detachments.

<SwmSnippet path="/src/machine/elements/ordonnanceur/ordonnanceur.cpp" line="50">

---

The <SwmToken path="src/machine/elements/ordonnanceur/ordonnanceur.cpp" pos="50:4:4" line-data="void Ordonnanceur::execution()">`execution`</SwmToken> method starts a new thread that continuously processes events from the delivery mechanism. It handles different types of events based on the state of the processes and the system.

```c++
void Ordonnanceur::execution()
{
	try
	{
		_effecteur = std::thread([this] ()
		{
			thread_local size_t echeance = 0;
			for( ; ; )
			{
				SVM_Synchronisation::AdresseSP source;
				SVM_Systeme::EvenementSP evenement;
				if(echeance==0)
				{
					if(not _livraison->recupere(_adresse,source,evenement))
						continue;
				}
				else
				{
					if(not _livraison->recupere_echeance(_adresse,source,evenement,echeance))
					{
						try
```

---

</SwmSnippet>

## Ordonnanceur Interaction

Ordonnanceur interacts with other components like the system and debugger to ensure proper process management and error handling.

<SwmSnippet path="/src/machine/elements/ordonnanceur/ordonnanceur.h" line="81">

---

The Ordonnanceur class definition includes methods for attaching and detaching processes, notifying state changes, and handling termination requests. These methods facilitate interaction with other system components.

```c
struct Ordonnanceur : public std::enable_shared_from_this<Ordonnanceur>
{
	friend struct SVM::Machine::Interface::Outils;
	friend struct SVM::Machine::Interface::VerrouProcessus;
	Ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme);
	Ordonnanceur(const Ordonnanceur&) = delete;
	virtual ~Ordonnanceur();

	bool operator<(const Ordonnanceur& o) const
	{
		if(not _nom)
			return true;
		if(not o._nom)
			return false;
		return _nom<o._nom;
	}
	void attache(const SVM_Synchronisation::AdresseSP& source, const SVM_Processus::ProcessusSP& processus, const size_t parametre);
	void detache(const SVM_Synchronisation::AdresseSP& source, const SVM_Processus::ProcessusSP& processus, const size_t parametre);
	void notifie(const SVM_Synchronisation::AdresseSP& source, const size_t parametre);
	void terminaison_demandee(const SVM_Synchronisation::AdresseSP& source);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
