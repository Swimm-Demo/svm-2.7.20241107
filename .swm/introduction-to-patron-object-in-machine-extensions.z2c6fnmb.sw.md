---
title: Introduction to Patron Object in Machine Extensions
---
# Introduction to Patron Object in Machine Extensions

The Patron Object is a base structure used to define various extensions within the virtual machine. It provides a common interface for extensions to implement specific functionalities. The Patron Object includes methods for compatibility checks, formatting, and debugging.

# Derived Classes

Derived classes such as <SwmToken path="src/machine/extensions/patron_objet_extension.h" pos="62:3:3" line-data="	DECL_SHARED_PTR(PatronTypeExtension);">`PatronTypeExtension`</SwmToken>, <SwmToken path="src/machine/extensions/patron_objet_extension.h" pos="94:3:3" line-data="	DECL_SHARED_PTR(PatronStructureExtension);">`PatronStructureExtension`</SwmToken>, and <SwmToken path="src/machine/extensions/description.cpp" pos="309:1:1" line-data="	PatronInstructionExtension::Instruction i = reinterpret_cast&lt;PatronInstructionExtension::Instruction&gt;(::dlsym(fichier,oss.str().c_str()));">`PatronInstructionExtension`</SwmToken> extend the Patron Object to implement specific behaviors. These derived classes override the virtual methods of the Patron Object to provide their own implementations.

# Name Entry Point

The Patron Object also manages a name entry point, which is used to identify the extension.

<SwmSnippet path="/src/machine/extensions/patron_objet_extension.h" line="35">

---

# Patron Object Definition

The definition of the Patron Object includes methods for compatibility checks, formatting, and debugging. The constructor initializes the name entry point, and several virtual methods are defined for compatibility checks, formatting, and debugging.

```c
	struct PatronObjetExtension
	{
		PatronObjetExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:_nom(nom) {}
		virtual ~PatronObjetExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const PatronObjetExtension& p)
		{
			p.format(f);
			return f;
		}
		virtual void format(std::ostream& os) const = 0;
		virtual void nom(std::ostream& os) const
		{
			format(os);
		}
		virtual void interruption(std::set<SVM::Machine::Element::Valeur::Interruption>& i) const {};
		virtual std::string debugueur() const
		{
			std::ostringstream oss;
```

---

</SwmSnippet>

# Example: <SwmToken path="src/machine/extensions/description.cpp" pos="309:1:1" line-data="	PatronInstructionExtension::Instruction i = reinterpret_cast&lt;PatronInstructionExtension::Instruction&gt;(::dlsym(fichier,oss.str().c_str()));">`PatronInstructionExtension`</SwmToken>

An example of how the <SwmToken path="src/machine/extensions/description.cpp" pos="309:1:1" line-data="	PatronInstructionExtension::Instruction i = reinterpret_cast&lt;PatronInstructionExtension::Instruction&gt;(::dlsym(fichier,oss.str().c_str()));">`PatronInstructionExtension`</SwmToken> class, which extends the Patron Object, is used to define an instruction that can be executed by the virtual machine.

<SwmSnippet path="/src/machine/extensions/description.cpp" line="308">

---

This code snippet shows how the <SwmToken path="src/machine/extensions/description.cpp" pos="309:1:1" line-data="	PatronInstructionExtension::Instruction i = reinterpret_cast&lt;PatronInstructionExtension::Instruction&gt;(::dlsym(fichier,oss.str().c_str()));">`PatronInstructionExtension`</SwmToken> class is used to define an instruction. It demonstrates the process of creating an instruction, setting parameters, and returning a shared pointer to the <SwmToken path="src/machine/extensions/description.cpp" pos="309:1:1" line-data="	PatronInstructionExtension::Instruction i = reinterpret_cast&lt;PatronInstructionExtension::Instruction&gt;(::dlsym(fichier,oss.str().c_str()));">`PatronInstructionExtension`</SwmToken>.

```c++
	oss << "instruction_" << _instruction.nom_entree();
	PatronInstructionExtension::Instruction i = reinterpret_cast<PatronInstructionExtension::Instruction>(::dlsym(fichier,oss.str().c_str()));
	if(not i) { throw ExtensionFonctionNonDefinie(nom,"SVM_Value",oss.str(),"const void *svm, const SVM_Size argc, SVM_Parameter argv[]"); }
	oss.str("");
	oss << *_parametres;
	AutomateNonDeterministeSP parametre = *_parametres;
	AutomateDeterministeSP automate_parametres = parametre->determinise(oss.str());
	AutomateDeterministeSP automate_retour;
	if(_retour)
	{
		oss.str("");
		oss << *_retour;
		AutomateNonDeterministeSP retour = *_retour;
		automate_retour = retour->determinise(oss.str());
	}
	return std::make_shared<PatronInstructionExtension>(_instruction,_systeme,_attente,_remplacement,i,automate_parametres,automate_retour);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
