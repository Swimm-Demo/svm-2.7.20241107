---
title: Exploring the Systeme Component
---
# Overview

The <SwmToken path="src/machine/elements/systeme/systeme.h" pos="57:3:3" line-data="	struct Systeme : public std::enable_shared_from_this&lt;Systeme&gt;">`Systeme`</SwmToken> class represents the system component within the virtual machine environment. It is responsible for managing various processes and their execution within the machine. The <SwmToken path="src/machine/elements/systeme/systeme.h" pos="57:3:3" line-data="	struct Systeme : public std::enable_shared_from_this&lt;Systeme&gt;">`Systeme`</SwmToken> class includes methods for creating the system, handling interruptions, and managing processes. It interacts with other components such as <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="40:7:7" line-data="	systeme-&gt;_adresse = Livraison::adresse(systeme-&gt;shared_from_this()); ">`Livraison`</SwmToken> for delivery management and <SwmToken path="src/machine/elements/systeme/systeme.h" pos="59:5:5" line-data="		friend struct Ordonnanceur::OrdonnanceurDefaut;">`Ordonnanceur`</SwmToken> for scheduling.

# Systeme Class

The <SwmToken path="src/machine/elements/systeme/systeme.h" pos="57:3:3" line-data="	struct Systeme : public std::enable_shared_from_this&lt;Systeme&gt;">`Systeme`</SwmToken> class is defined in <SwmPath>[src/machine/elements/systeme/systeme.h](src/machine/elements/systeme/systeme.h)</SwmPath>. It includes methods and member variables essential for system management.

<SwmSnippet path="/src/machine/elements/systeme/systeme.h" line="57">

---

The <SwmToken path="src/machine/elements/systeme/systeme.h" pos="57:3:3" line-data="	struct Systeme : public std::enable_shared_from_this&lt;Systeme&gt;">`Systeme`</SwmToken> class definition includes methods for creating the system, handling interruptions, and managing processes. It also includes friend declarations for <SwmToken path="src/machine/elements/systeme/systeme.h" pos="59:5:5" line-data="		friend struct Ordonnanceur::OrdonnanceurDefaut;">`Ordonnanceur`</SwmToken> and <SwmToken path="src/machine/elements/systeme/systeme.h" pos="61:5:5" line-data="		friend struct Processus::Processus;">`Processus`</SwmToken> classes.

```c
	struct Systeme : public std::enable_shared_from_this<Systeme>
	{
		friend struct Ordonnanceur::OrdonnanceurDefaut;
		friend struct Ordonnanceur::OrdonnanceurExtension;
		friend struct Processus::Processus;
		static SystemeSP creation_systeme(const SVM_Machine::MachineSP& machine);
		Systeme(const SVM_Machine::MachineSP& machine);
		virtual ~Systeme();
		int execution(const SVM::Machine::MachineSP& machine, std::vector<SVM_Machine::Processus>& liste_processus);
		void terminaison_demandee();
		void acquittement_terminaison(const SVM_Synchronisation::AdresseSP& source);
		void ajoute_processus(const SVM_Processus::ProcessusSP& processus);
		bool retire_processus(const SVM_Processus::ProcessusSP& processus);
		std::vector<SVM_Processus::ProcessusSP> liste_processus() const;
		const void* ajout_environnement(const SVM_Interface::EnvironnementSP& environnement);
		bool enleve_environnement(const void *clef);
		void notifie_etat(const SVM_Synchronisation::AdresseSP source, const SVM_Processus::Etat::Execution etat)
		{
			_livraison->poste(_adresse,source,std::make_shared<Evenement>(Evenement::Type::ETAT,etat));
		}
		SHARED_PTR(size_t) traitement_processus_mort(const SVM_Processus::ProcessusSP& processus,const SVM_Processus::Etat::Execution etat_declare);
```

---

</SwmSnippet>

# Systeme Constructor

The constructor initializes the <SwmToken path="src/machine/elements/systeme/systeme.h" pos="57:3:3" line-data="	struct Systeme : public std::enable_shared_from_this&lt;Systeme&gt;">`Systeme`</SwmToken> object with a machine and sets up the <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="40:7:7" line-data="	systeme-&gt;_adresse = Livraison::adresse(systeme-&gt;shared_from_this()); ">`Livraison`</SwmToken> component.

# Systeme Creation

The <SwmToken path="src/machine/elements/systeme/systeme.h" pos="62:5:5" line-data="		static SystemeSP creation_systeme(const SVM_Machine::MachineSP&amp; machine);">`creation_systeme`</SwmToken> method initializes the system, sets up addresses, and creates necessary boxes for event handling.

<SwmSnippet path="/src/machine/elements/systeme/systeme.cpp" line="37">

---

The <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="37:4:4" line-data="SystemeSP Systeme::creation_systeme(const SVM_Machine::MachineSP&amp; machine)">`creation_systeme`</SwmToken> method sets up the system by initializing addresses, creating event handling boxes, and setting up the <SwmToken path="src/machine/elements/systeme/systeme.h" pos="59:5:5" line-data="		friend struct Ordonnanceur::OrdonnanceurDefaut;">`Ordonnanceur`</SwmToken> components.

```c++
SystemeSP Systeme::creation_systeme(const SVM_Machine::MachineSP& machine)
{
	auto systeme = std::make_shared<Systeme>(machine);
	systeme->_adresse = Livraison::adresse(systeme->shared_from_this()); 
	systeme->_livraison->creation_boite(systeme->_adresse);
	systeme->_livraison->_piege = SVM::Machine::Debugueur::Debugueur::ajoute_piege<PiegeEvenements>(machine->_base->_debugueur);
	machine->_base->_gestionnaire_interruptions_materielles->_systeme = systeme;
	machine->_base->_gestionnaire_extensions->initialisation_generale(machine);
	auto ord_defaut = SVM_Ordonnanceur::OrdonnanceurDefaut::creation_ordonnanceur(systeme->shared_from_this());
	systeme->_ordonnanceurs.insert(std::make_pair(SVM_Valeur::PointEntreeExtensionSP(),ord_defaut));
	for(auto& o : machine->_base->_gestionnaire_extensions->liste_ordonannanceurs())
	{
		auto nom = std::make_shared<SVM_Valeur::PointEntreeExtension>(o);
		auto ord_extension = SVM_Ordonnanceur::OrdonnanceurExtension::creation_ordonnanceur(nom,systeme->shared_from_this());
		systeme->_ordonnanceurs.insert(std::make_pair(nom,ord_extension));
	}
	for(auto& o: systeme->_ordonnanceurs)
	{
		o.second->execution();
	}
	return systeme;
```

---

</SwmSnippet>

# Handling Interruptions

The <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="319:4:4" line-data="void Systeme::interruption_materielle()">`interruption_materielle`</SwmToken> method handles hardware interruptions by notifying all processes.

<SwmSnippet path="/src/machine/elements/systeme/systeme.cpp" line="319">

---

The <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="319:4:4" line-data="void Systeme::interruption_materielle()">`interruption_materielle`</SwmToken> method locks the protection mutex and notifies all processes of the interruption.

```c++
void Systeme::interruption_materielle()
{
	std::lock_guard<std::mutex> verrou(_protection);
	for(auto& p: _processus)
	{
		p->notification_interruption();
	}
}
```

---

</SwmSnippet>

# Termination Request

The <SwmToken path="src/machine/elements/systeme/systeme.h" pos="66:3:3" line-data="		void terminaison_demandee();">`terminaison_demandee`</SwmToken> method posts a termination event to the system.

<SwmSnippet path="/src/machine/elements/systeme/systeme.cpp" line="314">

---

The <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="314:4:4" line-data="void Systeme::terminaison_demandee()">`terminaison_demandee`</SwmToken> method posts a termination event using the <SwmToken path="src/machine/elements/systeme/systeme.cpp" pos="40:7:7" line-data="	systeme-&gt;_adresse = Livraison::adresse(systeme-&gt;shared_from_this()); ">`Livraison`</SwmToken> component.

```c++
void Systeme::terminaison_demandee()
{
	_livraison->poste(_adresse,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::TERMINAISON));
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
