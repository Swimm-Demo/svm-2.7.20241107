---
title: Introduction to Analyseur
---
# Introduction to Analyseur

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="30:3:3" line-data="	struct Analyseur">`Analyseur`</SwmToken> structure is a key component in the virtual machine environment, providing a static method called <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:11:11" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`analyse`</SwmToken> that processes an input source string and returns a <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:9:9" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`LanceurSP`</SwmToken> object. This object is essential for starting the virtual machine.

# Analyseur Structure

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="30:3:3" line-data="	struct Analyseur">`Analyseur`</SwmToken> structure is defined in the <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.h](src/demarrage/lanceur/analyseur/analyseur.h)</SwmPath> file. It includes a static method <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:11:11" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`analyse`</SwmToken> which takes a string parameter <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:20:20" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`source`</SwmToken> and returns a <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:9:9" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`LanceurSP`</SwmToken> object.

<SwmSnippet path="/src/demarrage/lanceur/analyseur/analyseur.h" line="30">

---

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="30:3:3" line-data="	struct Analyseur">`Analyseur`</SwmToken> structure definition in <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.h](src/demarrage/lanceur/analyseur/analyseur.h)</SwmPath> shows the declaration of the <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:11:11" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`analyse`</SwmToken> method.

```c
	struct Analyseur
	{
		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string& source);
	};
```

---

</SwmSnippet>

# Analyse Method Definition

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:11:11" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`analyse`</SwmToken> method is defined in the <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.cpp](src/demarrage/lanceur/analyseur/analyseur.cpp)</SwmPath> file. It processes the input source string and returns a launcher object (<SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:9:9" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`LanceurSP`</SwmToken>) that is used to start the virtual machine.

<SwmSnippet path="/src/demarrage/lanceur/analyseur/analyseur.cpp" line="57">

---

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.cpp" pos="57:10:10" line-data="SVM::Demarrage::Lanceur::LanceurSP Analyseur::analyse(const std::string&amp; source)">`analyse`</SwmToken> method implementation in <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.cpp](src/demarrage/lanceur/analyseur/analyseur.cpp)</SwmPath> shows the steps taken to process the input source string and return a <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.cpp" pos="57:6:6" line-data="SVM::Demarrage::Lanceur::LanceurSP Analyseur::analyse(const std::string&amp; source)">`LanceurSP`</SwmToken> object.

```c++
SVM::Demarrage::Lanceur::LanceurSP Analyseur::analyse(const std::string& source)
{
	SVM::Demarrage::Lanceur::LanceurSP lanceur(new SVM::Demarrage::Lanceur::Lanceur(source));
	Memoire_demarragelanceuranalyseur memoire;
	::demarragelanceuranalyseurlex_init(&memoire.scanner);
	if(not SVM::Global::Fichier::teste_fichier(source))
	{
		throw Erreur() << "File " << source << " can not be opened or read";
	}
	memoire.fichier = SVM::Global::Fichier::ouvrir(source);
	memoire.tampon = ::demarragelanceuranalyseur_create_buffer(memoire.fichier,YY_BUF_SIZE,memoire.scanner);
	::demarragelanceuranalyseur_switch_to_buffer(memoire.tampon,memoire.scanner);
	if(::demarragelanceuranalyseurparse(memoire.scanner,lanceur))
	{
		throw lanceur->erreur();
	}
	lanceur->decoration();
	return lanceur;
}
```

---

</SwmSnippet>

# Usage of Analyse Method

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="32:11:11" line-data="		static SVM::Demarrage::Lanceur::LanceurSP analyse(const std::string&amp; source);">`analyse`</SwmToken> method is used to initialize the <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.cpp" pos="59:9:9" line-data="	SVM::Demarrage::Lanceur::LanceurSP lanceur(new SVM::Demarrage::Lanceur::Lanceur(source));">`lanceur`</SwmToken> object in the <SwmPath>[src/demarrage/binaire/svm.cpp](src/demarrage/binaire/svm.cpp)</SwmPath> file. This object is then used to create the virtual machine instance.

<SwmSnippet path="/src/demarrage/binaire/svm.cpp" line="57">

---

The <SwmToken path="src/demarrage/binaire/svm.cpp" pos="58:13:13" line-data="			lanceur = SVM::Demarrage::Lanceur::Analyseur::analyse(fichier);">`analyse`</SwmToken> method is called in <SwmPath>[src/demarrage/binaire/svm.cpp](src/demarrage/binaire/svm.cpp)</SwmPath> to initialize the <SwmToken path="src/demarrage/binaire/svm.cpp" pos="58:1:1" line-data="			lanceur = SVM::Demarrage::Lanceur::Analyseur::analyse(fichier);">`lanceur`</SwmToken> object, which is then used to create the virtual machine instance.

```c++
		{
			lanceur = SVM::Demarrage::Lanceur::Analyseur::analyse(fichier);
			machine = lanceur->machine_virtuelle(argc,argv);
```

---

</SwmSnippet>

# Analyseur Endpoints

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.h" pos="30:3:3" line-data="	struct Analyseur">`Analyseur`</SwmToken> structure also includes endpoints for error handling and lexical analysis.

## demarragelanceuranalyseurerror

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.syn.ypp" pos="25:4:4" line-data="extern int demarragelanceuranalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP&amp; lanceur, std::string mesg);">`demarragelanceuranalyseurerror`</SwmToken> function handles errors during the analysis process. It takes parameters for location, scanner, launcher, and an error message.

<SwmSnippet path="/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" line="25">

---

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.syn.ypp" pos="25:4:4" line-data="extern int demarragelanceuranalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP&amp; lanceur, std::string mesg);">`demarragelanceuranalyseurerror`</SwmToken> function declaration in <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.syn.ypp](src/demarrage/lanceur/analyseur/analyseur.syn.ypp)</SwmPath> shows the parameters it takes for error handling.

```ypp
extern int demarragelanceuranalyseurerror(YYLTYPE *llocp, void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur, std::string mesg);
```

---

</SwmSnippet>

## demarragelanceuranalyseurlex

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.syn.ypp" pos="26:4:4" line-data="extern int demarragelanceuranalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`demarragelanceuranalyseurlex`</SwmToken> function performs lexical analysis. It takes parameters for value, location, and scanner.

<SwmSnippet path="/src/demarrage/lanceur/analyseur/analyseur.syn.ypp" line="26">

---

The <SwmToken path="src/demarrage/lanceur/analyseur/analyseur.syn.ypp" pos="26:4:4" line-data="extern int demarragelanceuranalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);">`demarragelanceuranalyseurlex`</SwmToken> function declaration in <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.syn.ypp](src/demarrage/lanceur/analyseur/analyseur.syn.ypp)</SwmPath> shows the parameters it takes for lexical analysis.

```ypp
extern int demarragelanceuranalyseurlex(YYSTYPE *lvalp, YYLTYPE *llocp, void *scanner);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
