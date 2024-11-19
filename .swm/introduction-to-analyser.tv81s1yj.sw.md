---
title: Introduction to Analyser
---
# Introduction to Analyser

The Analyser component is responsible for analyzing and parsing input data within the virtual machine environment. It utilizes a Bison parser to process the input data and generate a structured representation of the data.

<SwmSnippet path="/src/machine/extensions/gestionnaire/gestionnaire.cpp" line="77">

---

If the parsing process encounters any errors, it throws a `ConfigurationInvalide` exception with details about the error. Upon successful parsing, the function returns a <SwmToken path="src/machine/extensions/gestionnaire/gestionnaire.cpp" pos="77:1:1" line-data="		DescriptionSP description = SVM_Extension::Analyseur::analyse(extension._nom,extension_configuration._configuration);">`DescriptionSP`</SwmToken> object that contains the parsed data and performs verification on it.

```c++
		DescriptionSP description = SVM_Extension::Analyseur::analyse(extension._nom,extension_configuration._configuration);
		auto it = gestionnaire->_extensions.insert(description->_nom);
```

---

</SwmSnippet>

## Usage in Gestionnaire

In this example, <SwmToken path="src/machine/extensions/gestionnaire/gestionnaire.cpp" pos="77:9:9" line-data="		DescriptionSP description = SVM_Extension::Analyseur::analyse(extension._nom,extension_configuration._configuration);">`Analyseur`</SwmToken> is used to analyze an extension configuration and insert the resulting description into a manager's extensions.

<SwmSnippet path="/src/machine/extensions/gestionnaire/gestionnaire.cpp" line="76">

---

The code demonstrates how <SwmToken path="src/machine/extensions/gestionnaire/gestionnaire.cpp" pos="77:9:9" line-data="		DescriptionSP description = SVM_Extension::Analyseur::analyse(extension._nom,extension_configuration._configuration);">`Analyseur`</SwmToken> is used to analyze an extension configuration and insert the resulting description into a manager's extensions.

```c++
		}
		DescriptionSP description = SVM_Extension::Analyseur::analyse(extension._nom,extension_configuration._configuration);
		auto it = gestionnaire->_extensions.insert(description->_nom);
```

---

</SwmSnippet>

# Analyseur Endpoints

The Analyser component provides several endpoints for lexical analysis and initialization.

## machineextensionsanalyseurlex

The <SwmToken path="src/machine/extensions/analyseur/analyseur.syn.ypp" pos="28:4:4" line-data="extern int machineextensionsanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`machineextensionsanalyseurlex`</SwmToken> function is responsible for lexical analysis. It processes the input data and returns tokens that are used by the parser to build the syntax tree.

<SwmSnippet path="/src/machine/extensions/analyseur/analyseur.syn.ypp" line="28">

---

The function <SwmToken path="src/machine/extensions/analyseur/analyseur.syn.ypp" pos="28:4:4" line-data="extern int machineextensionsanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`machineextensionsanalyseurlex`</SwmToken> processes the input data and returns tokens for the parser.

```ypp
extern int machineextensionsanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
```

---

</SwmSnippet>

## <SwmToken path="src/machine/extensions/analyseur/analyseur.syn.ypp" pos="29:4:4" line-data="extern int machineextensionsanalyseurlex_init(void *scanner);">`machineextensionsanalyseurlex_init`</SwmToken>

The <SwmToken path="src/machine/extensions/analyseur/analyseur.syn.ypp" pos="29:4:4" line-data="extern int machineextensionsanalyseurlex_init(void *scanner);">`machineextensionsanalyseurlex_init`</SwmToken> function initializes the lexical scanner. It sets up the necessary environment for the lexical analysis to take place.

<SwmSnippet path="/src/machine/extensions/analyseur/analyseur.syn.ypp" line="29">

---

The function <SwmToken path="src/machine/extensions/analyseur/analyseur.syn.ypp" pos="29:4:4" line-data="extern int machineextensionsanalyseurlex_init(void *scanner);">`machineextensionsanalyseurlex_init`</SwmToken> initializes the lexical scanner.

```ypp
extern int machineextensionsanalyseurlex_init(void *scanner);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
