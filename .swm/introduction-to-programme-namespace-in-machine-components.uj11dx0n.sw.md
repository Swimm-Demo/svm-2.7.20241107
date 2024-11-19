---
title: Introduction to Programme Namespace in Machine Components
---
# Introduction to Programme in Machine Components

Programme refers to the components and structures that define and manage the execution flow within the virtual machine. It includes various elements such as instructions, memory addresses, and values that are essential for the operation of the virtual machine.

## Programme Namespace

The <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="36:2:2" line-data="namespace Programme">`Programme`</SwmToken> namespace encapsulates various components and structures that are crucial for defining and managing the execution flow within the virtual machine. It includes classes and structures like <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="38:3:3" line-data="	DECL_SHARED_PTR(Adresse);">`Adresse`</SwmToken>, <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="28:12:12" line-data="namespace SVM_Valeur = SVM::Machine::Element::Valeur;">`Valeur`</SwmToken>, and specific instruction handlers that facilitate the execution of code.

## Example of Programme Usage

The <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="38:3:3" line-data="	DECL_SHARED_PTR(Adresse);">`Adresse`</SwmToken> structure within the <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="36:2:2" line-data="namespace Programme">`Programme`</SwmToken> namespace is used to manage memory addresses. It includes various methods for decoration, evaluation, explanation, formatting, and HTML representation of addresses.

<SwmSnippet path="/src/machine/elements/programme/adresse_programme.h" line="36">

---

The <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="38:3:3" line-data="	DECL_SHARED_PTR(Adresse);">`Adresse`</SwmToken> structure includes methods such as <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="44:5:5" line-data="		virtual void decoration(SVM_Valeur::Decorateur&amp; decorateur) = 0;">`decoration`</SwmToken>, <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="45:7:7" line-data="		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP&amp; ce) const = 0;">`evaluation`</SwmToken>, <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="46:3:3" line-data="		void explique(const SVM_Noyau::NoyauSP&amp; noyau, std::ostream&amp; os) const;">`explique`</SwmToken>, <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="48:5:5" line-data="		virtual void format(std::ostream&amp; os) const = 0;">`format`</SwmToken>, and <SwmToken path="src/machine/elements/programme/adresse_programme.h" pos="55:5:5" line-data="		virtual void html(std::ostream&amp; os) const = 0;">`html`</SwmToken> to manage and represent memory addresses.

```c
namespace Programme
{
	DECL_SHARED_PTR(Adresse);

	struct Adresse
	{
		Adresse() = default;
		virtual ~Adresse() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP& ce) const = 0;
		void explique(const SVM_Noyau::NoyauSP& noyau, std::ostream& os) const;
		virtual SVM_Valeur::AdresseMemoire explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const = 0;
		virtual void format(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Adresse& adresse)
		{
			adresse.format(f);
			return f;
		}
		virtual void html(std::ostream& os) const = 0;
```

---

</SwmSnippet>

## Programme Endpoints

Programme endpoints handle various aspects of programme analysis and execution within the virtual machine.

### machineelementsprogrammeanalyseurerror

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="26:4:4" line-data="extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string&amp; nom, SVM_Valeur::CodeSP&amp; code, SVM_Valeur::InterruptionSP&amp; erreur, const SVM_Valeur::PositionInstructionSP&amp; position, std::string mesg);">`machineelementsprogrammeanalyseurerror`</SwmToken> function is used to handle errors during the analysis of programme instructions. It takes parameters such as the location of the error, the scanner object, the name of the programme, the code, the error object, the position of the instruction, and an error message.

<SwmSnippet path="/src/machine/elements/programme/analyseur/analyseur.syn.ypp" line="26">

---

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="26:4:4" line-data="extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string&amp; nom, SVM_Valeur::CodeSP&amp; code, SVM_Valeur::InterruptionSP&amp; erreur, const SVM_Valeur::PositionInstructionSP&amp; position, std::string mesg);">`machineelementsprogrammeanalyseurerror`</SwmToken> function signature shows the parameters it takes for handling errors during programme analysis.

```ypp
extern int machineelementsprogrammeanalyseurerror(YYLTYPE *llocp, void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position, std::string mesg);
```

---

</SwmSnippet>

### machineelementsprogrammeanalyseurlex

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`machineelementsprogrammeanalyseurlex`</SwmToken> function is responsible for lexical analysis. It processes the input and returns tokens that represent the basic elements of the programme's syntax.

<SwmSnippet path="/src/machine/elements/programme/analyseur/analyseur.syn.ypp" line="27">

---

The <SwmToken path="src/machine/elements/programme/analyseur/analyseur.syn.ypp" pos="27:4:4" line-data="extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`machineelementsprogrammeanalyseurlex`</SwmToken> function signature shows its role in lexical analysis by processing input and returning tokens.

```ypp
extern int machineelementsprogrammeanalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
