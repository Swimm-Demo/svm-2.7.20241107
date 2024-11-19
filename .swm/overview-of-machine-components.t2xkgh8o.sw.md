---
title: Overview of Machine Components
---
## Introduction

Machine components are integral parts of the virtual machine environment, responsible for various functionalities such as memory management, synchronization, and process handling. This document provides an overview of the key classes and their roles within the system.

## Systeme Class

The <SwmToken path="src/machine/elements/processus/processus.h" pos="273:7:7" line-data="		SVM::Machine::Element::Systeme::SystemeWP _systeme;">`Systeme`</SwmToken> class is a key component that initializes and manages system-level operations, including the creation of delivery boxes and the initialization of extensions. It sets up the system by creating a shared <SwmToken path="src/machine/elements/processus/processus.h" pos="273:7:7" line-data="		SVM::Machine::Element::Systeme::SystemeWP _systeme;">`Systeme`</SwmToken> object, initializing addresses, and setting up traps and interrupt handlers.

## Machine Class

The <SwmToken path="src/machine/elements/processus/processus.h" pos="271:3:3" line-data="		SVM::Machine::Element::Ordonnanceur::OrdonnanceurWP _ordonnanceur;">`Machine`</SwmToken> class is referenced within the <SwmToken path="src/machine/elements/processus/processus.h" pos="273:7:7" line-data="		SVM::Machine::Element::Systeme::SystemeWP _systeme;">`Systeme`</SwmToken> class to link system operations with the machine's base functionalities, such as debugging and extension management.

## Noyau Class

The `Noyau` class provides access to the machine through its <SwmToken path="src/machine/elements/processus/processus.h" pos="22:6:6" line-data="#include &lt;src/machine/elements/processus/etat_processus.h&gt;">`machine`</SwmToken> method, which retrieves the machine instance associated with the current process.

<SwmSnippet path="/src/machine/elements/processus/processus.h" line="267">

---

The `Noyau` class includes a reference to the current kernel.

```c
		Etat _etat;
		public:
		SVM_Noyau::NoyauSP _noyau_courant;
		private:
```

---

</SwmSnippet>

## Processus Class

The <SwmToken path="src/machine/elements/processus/processus.h" pos="70:2:2" line-data="namespace Processus">`Processus`</SwmToken> class contains a reference to the <SwmToken path="src/machine/elements/processus/processus.h" pos="273:7:7" line-data="		SVM::Machine::Element::Systeme::SystemeWP _systeme;">`Systeme`</SwmToken> class, establishing a connection between processes and the system they operate within.

<SwmSnippet path="/src/machine/elements/processus/processus.h" line="271">

---

The <SwmToken path="src/machine/elements/processus/processus.h" pos="70:2:2" line-data="namespace Processus">`Processus`</SwmToken> class includes references to the scheduler, system, base, and sequencer.

```c
		SVM::Machine::Element::Ordonnanceur::OrdonnanceurWP _ordonnanceur;
		public:
		SVM::Machine::Element::Systeme::SystemeWP _systeme;
		SVM_Base::BaseWP _base;
		SVM_Sequenceur::SequenceurSP _sequenceur;
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/processus.h" line="272">

---

The <SwmToken path="src/machine/elements/processus/processus.h" pos="35:12:12" line-data="namespace SVM_Base = SVM::Machine::Element::Base;">`Base`</SwmToken> class includes references to the system, base, sequencer, and a set of kernels.

```c
		public:
		SVM::Machine::Element::Systeme::SystemeWP _systeme;
		SVM_Base::BaseWP _base;
		SVM_Sequenceur::SequenceurSP _sequenceur;
		std::set<SVM_Noyau::NoyauSP> _noyaux;
```

---

</SwmSnippet>

## Endpoints of Machine Components

Endpoints of machine components handle specific tasks such as error handling and lexical analysis during program execution.

### machineelementsprogrammeanalyseurerror

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="26:4:4" line-data="extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string&amp; nom, SVM_Valeur::CodeSP&amp; code, SVM_Valeur::InterruptionSP&amp; erreur, const SVM_Valeur::PositionInstructionSP&amp; position, std::string mesg);">`machineelementsprogrammeanalyseurerror`</SwmToken> function is used to handle errors during the analysis of a program. It takes parameters such as the location of the error, the scanner, the name of the program, the code, the error type, the position of the instruction, and an error message.

<SwmSnippet path="/src/machine/elements/programme/analyseur/analyseur.syn.ypp" line="26">

---

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="26:4:4" line-data="extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string&amp; nom, SVM_Valeur::CodeSP&amp; code, SVM_Valeur::InterruptionSP&amp; erreur, const SVM_Valeur::PositionInstructionSP&amp; position, std::string mesg);">`machineelementsprogrammeanalyseurerror`</SwmToken> function signature.

```ypp
extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position, std::string mesg);
```

---

</SwmSnippet>

### machineelementsprogrammeanalyseurlex

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`machineelementsprogrammeanalyseurlex`</SwmToken> function is responsible for lexical analysis. It takes parameters such as the value and location of the token, and the scanner.

<SwmSnippet path="/src/machine/elements/programme/analyseur/analyseur.syn.ypp" line="27">

---

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`machineelementsprogrammeanalyseurlex`</SwmToken> function signature.

```ypp
extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
