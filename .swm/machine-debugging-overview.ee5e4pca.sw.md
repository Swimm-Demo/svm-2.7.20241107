---
title: Machine Debugging Overview
---
# Machine Debugging Overview

Machine Debugging refers to the process of identifying and resolving issues within the virtual machine environment. This document provides an overview of the key components and methods involved in machine debugging.

## Debugueur Class

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="36:2:2" line-data="DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP&amp; debug)">`Debugueur`</SwmToken> class is central to the debugging process, providing methods for creating and managing debugging sessions.

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="36">

---

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="36:4:4" line-data="DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP&amp; debug)">`creation`</SwmToken> method in <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="36:2:2" line-data="DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP&amp; debug)">`Debugueur`</SwmToken> initializes a new debugging session, setting up necessary components such as network connections and breakpoints. If the debugger cannot be started, the <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="52:3:3" line-data="		throw PasDeDebuggueur();">`PasDeDebuggueur`</SwmToken> exception is thrown, indicating a critical failure in the debugging setup.

```c++
DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP& debug)
{
	if(not debug)
	{
		return DebugueurSP();
	}
	DebugueurSP debugueur = std::make_shared<Debugueur>(debug->_titre, debug->_ip, debug->_port, debug->_style, debug->_clients, debug->_securite, debug->_historique);
	debugueur->_adresse = SVM::Machine::Debugueur::Livraison::adresse(debugueur->shared_from_this());
	debugueur->_livraison->creation_boite(debugueur->_adresse);
	SVM_TRACE("Debugueur " << (*debugueur));
	debugueur->_points_arret = Debugueur::ajoute_piege<PiegePointsArret>(debugueur);
	std::ostringstream os;
	debugueur->_fd_serveur = TCP::ouvrir<true,true,false>(os,SOCK_STREAM,debugueur->_ip,debugueur->_port);
	if(debugueur->_fd_serveur<0)
	{
		SVM::Machine::Trace(debug->_historique,"debug") << os.str() << "Abort Simple Virtual Machine boot!";
		throw PasDeDebuggueur();
	}
	debugueur->creation_effecteurs();
	return debugueur;
}
```

---

</SwmSnippet>

## <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="52:3:3" line-data="		throw PasDeDebuggueur();">`PasDeDebuggueur`</SwmToken> Exception

If the debugger cannot be started, the <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="52:3:3" line-data="		throw PasDeDebuggueur();">`PasDeDebuggueur`</SwmToken> exception is thrown, indicating a critical failure in the debugging setup.

<SwmSnippet path="/src/machine/debugueur/debugueur.h" line="219">

---

The <SwmToken path="src/machine/debugueur/debugueur.h" pos="219:1:1" line-data="	PasDeDebuggueur()">`PasDeDebuggueur`</SwmToken> exception is defined here, providing a detailed error message when the debugger fails to start.

```c
	PasDeDebuggueur()
	{
		DETAILS_EXCEPTION("Unable to start debugger.");
	}
```

---

</SwmSnippet>

## <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="54:3:3" line-data="	debugueur-&gt;creation_effecteurs();">`creation_effecteurs`</SwmToken> Method

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="54:3:3" line-data="	debugueur-&gt;creation_effecteurs();">`creation_effecteurs`</SwmToken> method within <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="36:2:2" line-data="DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP&amp; debug)">`Debugueur`</SwmToken> sets up the necessary effectors for handling debugging commands and responses.

## <SwmToken path="src/machine/debugueur/analyseur/analyseur.lex.cpp" pos="1824:2:2" line-data="void machinedebugueuranalyseurset_debug (int  bdebug , yyscan_t yyscanner)">`machinedebugueuranalyseurset_debug`</SwmToken> Function

The <SwmToken path="src/machine/debugueur/analyseur/analyseur.lex.cpp" pos="1824:2:2" line-data="void machinedebugueuranalyseurset_debug (int  bdebug , yyscan_t yyscanner)">`machinedebugueuranalyseurset_debug`</SwmToken> function in the analyzer module allows enabling or disabling debugging for the lexical analyzer.

<SwmSnippet path="/src/machine/debugueur/analyseur/analyseur.lex.cpp" line="1824">

---

The <SwmToken path="src/machine/debugueur/analyseur/analyseur.lex.cpp" pos="1824:2:2" line-data="void machinedebugueuranalyseurset_debug (int  bdebug , yyscan_t yyscanner)">`machinedebugueuranalyseurset_debug`</SwmToken> function enables or disables debugging for the lexical analyzer by setting the <SwmToken path="src/machine/debugueur/analyseur/analyseur.lex.cpp" pos="1827:1:1" line-data="    yy_flex_debug = bdebug ;">`yy_flex_debug`</SwmToken> variable.

```c++
void machinedebugueuranalyseurset_debug (int  bdebug , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    yy_flex_debug = bdebug ;
}
```

---

</SwmSnippet>

## Machine Debugging Endpoints

Machine Debugging Endpoints provide functions to initialize and destroy the lexical analyzer for the machine debugger.

### <SwmToken path="src/machine/debugueur/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machinedebugueuranalyseurlex_init(void *scanner);">`machinedebugueuranalyseurlex_init`</SwmToken>

The <SwmToken path="src/machine/debugueur/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machinedebugueuranalyseurlex_init(void *scanner);">`machinedebugueuranalyseurlex_init`</SwmToken> function initializes the lexical analyzer for the machine debugger. This is a crucial step in setting up the debugging environment, ensuring that the analyzer is ready to process input.

<SwmSnippet path="/src/machine/debugueur/analyseur/analyseur.syn.ypp" line="27">

---

The <SwmToken path="src/machine/debugueur/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machinedebugueuranalyseurlex_init(void *scanner);">`machinedebugueuranalyseurlex_init`</SwmToken> function is declared here, initializing the lexical analyzer for the machine debugger.

```ypp
extern int machinedebugueuranalyseurlex_init(void *scanner);
```

---

</SwmSnippet>

### <SwmToken path="src/machine/debugueur/analyseur/analyseur.syn.ypp" pos="28:4:4" line-data="extern int machinedebugueuranalyseurlex_destroy(void *scanner);">`machinedebugueuranalyseurlex_destroy`</SwmToken>

The <SwmToken path="src/machine/debugueur/analyseur/analyseur.syn.ypp" pos="28:4:4" line-data="extern int machinedebugueuranalyseurlex_destroy(void *scanner);">`machinedebugueuranalyseurlex_destroy`</SwmToken> function cleans up and destroys the lexical analyzer for the machine debugger. This function is called to release resources and ensure a clean shutdown of the analyzer.

<SwmSnippet path="/src/machine/debugueur/analyseur/analyseur.syn.ypp" line="28">

---

The <SwmToken path="src/machine/debugueur/analyseur/analyseur.syn.ypp" pos="28:4:4" line-data="extern int machinedebugueuranalyseurlex_destroy(void *scanner);">`machinedebugueuranalyseurlex_destroy`</SwmToken> function is declared here, cleaning up and destroying the lexical analyzer for the machine debugger.

```ypp
extern int machinedebugueuranalyseurlex_destroy(void *scanner);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
