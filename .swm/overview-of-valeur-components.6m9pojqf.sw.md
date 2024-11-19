---
title: Overview of Valeur Components
---
# Overview of Valeur Components

Valeur is a namespace within the `SVM::Machine::Element` namespace. It encapsulates various components and functionalities related to handling values in the virtual machine.

## Valeur Namespace

The <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="29:2:2" line-data="namespace Valeur">`Valeur`</SwmToken> namespace includes several structs that manage different aspects of values, such as comparisons, expressions, interruptions, instructions, and memory pointers. These components are essential for the effective handling of values within the virtual machine.

## Components of Valeur

The <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="29:2:2" line-data="namespace Valeur">`Valeur`</SwmToken> namespace includes the following components:

- <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="31:3:3" line-data="	struct Comparaison">`Comparaison`</SwmToken>: Manages total order comparisons between values.

- `ExpressionCondition`: Handles conditional expressions.

- `Interruption`: Manages interruptions within the machine.

- `Instruction`: Represents instructions in the virtual machine.

- `ArbreSubstitution`: Manages substitution trees.

- `PointEntreeExtension`: Handles entry points for extensions.

- `Arret`: Manages machine stops or interruptions.

- `Code`: Represents code within the machine.

- `Exception`: Handles exceptions.

- `AdresseInstruction`: Manages instruction addresses.

- `Parametre`: Manages parameters that hold values.

- `PositionInstruction`: Manages instruction positions.

- `Pointeur`: Manages memory addresses and sizes.

- `Decorateur`: Modifies the behavior of the machine with additional code.

## Example Usage

The <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="43:5:5" line-data="		static Comparaison comparaison_ordre_total(const T&amp; gauche, const T&amp; droite, const bool faible = false)">`comparaison_ordre_total`</SwmToken> function in <SwmPath>[src/machine/elements/valeur/comparaison.h](src/machine/elements/valeur/comparaison.h)</SwmPath> performs a total order comparison between two values.

<SwmSnippet path="/src/machine/elements/valeur/comparaison.h" line="29">

---

The <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="43:5:5" line-data="		static Comparaison comparaison_ordre_total(const T&amp; gauche, const T&amp; droite, const bool faible = false)">`comparaison_ordre_total`</SwmToken> function in the <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="31:3:3" line-data="	struct Comparaison">`Comparaison`</SwmToken> struct performs a total order comparison between two values. It sets various boolean flags to indicate the result of the comparison.

```c
namespace Valeur
{
	struct Comparaison
	{
		bool _egal = false;
		bool _inferieur = false;
		bool _superieur = false;
		bool _ordre = false;
		bool _total = false;
		bool _type = false;
		bool _nul = false;
		bool _faible = false;

		template<typename T>
		static Comparaison comparaison_ordre_total(const T& gauche, const T& droite, const bool faible = false)
		{
			Comparaison c;
			c._egal = gauche==droite;
			c._inferieur = gauche<droite;
			c._superieur = gauche>droite;
			c._ordre = true;
```

---

</SwmSnippet>

## Usage of Valeur

The <SwmToken path="src/machine/elements/valeur/comparaison.h" pos="29:2:2" line-data="namespace Valeur">`Valeur`</SwmToken> namespace is used to manage different aspects of values within the virtual machine. It provides various structs and functions to handle comparisons, expressions, interruptions, instructions, and memory pointers effectively.

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
