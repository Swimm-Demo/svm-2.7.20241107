---
title: Basic Concepts of Automate in Machine Extensions
---
# Basic Concepts of Automate in Machine Extensions

Automate refers to the implementation of deterministic and non-deterministic finite automata within the virtual machine environment. These automata are used to manage and process regular expressions and state transitions, which are essential for various computational tasks within the virtual machine.

## Deterministic Finite Automaton (DFA)

The deterministic finite automaton (DFA) is implemented in the <SwmToken path="src/machine/extensions/automate_deterministe.h" pos="34:3:3" line-data="	struct AutomateDeterministe">`AutomateDeterministe`</SwmToken> class. This class includes methods for checking state inclusion and conformity, ensuring that the automaton behaves as expected when processing input sequences.

<SwmSnippet path="/src/machine/extensions/automate_deterministe.h" line="34">

---

The <SwmToken path="src/machine/extensions/automate_deterministe.h" pos="34:3:3" line-data="	struct AutomateDeterministe">`AutomateDeterministe`</SwmToken> class defines a deterministic finite automaton with methods for state inclusion and conformity checks.

```c
	struct AutomateDeterministe
	{
		friend struct AutomateNonDeterministe;
		explicit AutomateDeterministe(const std::string& regex)
		:_regex(regex)
		{

		}
		bool inclus_dans(const AutomateDeterministeSP& automate) const;
		std::pair<bool,size_t> conforme(const std::vector<RegexParametresElementCSP>& valeurs) const;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const AutomateDeterministe& a)
		{
			f << a._regex;
			return f;
		}
		operator std::string () const
		{
			std::ostringstream f;
			f << "AutomateDeterministe: " << _regex << std::endl;
			f << "Initial : " << _initial << std::endl;
```

---

</SwmSnippet>

## Non-Deterministic Finite Automaton (NFA)

The non-deterministic finite automaton (NFA) is implemented in the <SwmToken path="src/machine/extensions/automate_deterministe.h" pos="36:5:5" line-data="		friend struct AutomateNonDeterministe;">`AutomateNonDeterministe`</SwmToken> class. This class handles more complex state transitions that can include multiple possible next states, making it suitable for more flexible and powerful pattern matching and state management tasks.

<SwmSnippet path="/src/machine/extensions/automate_non_deterministe.h" line="88">

---

The <SwmToken path="src/machine/extensions/automate_non_deterministe.h" pos="88:3:3" line-data="	struct AutomateNonDeterministe">`AutomateNonDeterministe`</SwmToken> class defines a non-deterministic finite automaton with methods for adding initial and final states and transitions.

```c
	struct AutomateNonDeterministe
	{
		AutomateNonDeterministe()
		{ }
		AutomateNonDeterministe(const RegexParametresElementCSP& t)
		{
			EtatNonDeterministeSP i = EtatNonDeterministe::nouvel_etat();
			EtatNonDeterministeSP f = EtatNonDeterministe::nouvel_etat();
			ajout_initial(i);
			ajout_final(f);
			ajout_transition(i,t,f);
		}
		void ajout_initial(const EtatNonDeterministeSP& etat)
		{
			_initial.insert(etat);
		}
		void ajout_final(const EtatNonDeterministeSP& etat)
		{
			_final.insert(etat);
		}
		void ajout_vide()
```

---

</SwmSnippet>

## Usage in Code

Automate is used in various parts of the codebase, including <SwmPath>[src/machine/extensions/automate_deterministe.cpp](src/machine/extensions/automate_deterministe.cpp)</SwmPath>, <SwmPath>[src/machine/extensions/automate_deterministe.h](src/machine/extensions/automate_deterministe.h)</SwmPath>, and <SwmPath>[src/machine/extensions/automate_non_deterministe.h](src/machine/extensions/automate_non_deterministe.h)</SwmPath>.

## Example of Automate Usage

The function <SwmToken path="src/machine/extensions/automate_non_deterministe.h" pos="146:3:3" line-data="		void ajout_automate_serie(const AutomateNonDeterministe&amp; a)">`ajout_automate_serie`</SwmToken> demonstrates how non-deterministic automata can be combined in series. It merges the initial states, transitions, and final states of two non-deterministic automata.

<SwmSnippet path="/src/machine/extensions/automate_non_deterministe.h" line="146">

---

The function <SwmToken path="src/machine/extensions/automate_non_deterministe.h" pos="146:3:3" line-data="		void ajout_automate_serie(const AutomateNonDeterministe&amp; a)">`ajout_automate_serie`</SwmToken> in the <SwmToken path="src/machine/extensions/automate_non_deterministe.h" pos="146:7:7" line-data="		void ajout_automate_serie(const AutomateNonDeterministe&amp; a)">`AutomateNonDeterministe`</SwmToken> class shows how to combine two non-deterministic automata in series.

```c
		void ajout_automate_serie(const AutomateNonDeterministe& a)
		{
			if(_initial.empty())
			{
				_initial = a._initial;
			}
			_transitions.insert(a._transitions.begin(),a._transitions.end());
			_transitions_instantanees.insert(a._transitions_instantanees.begin(),a._transitions_instantanees.end());
			for(auto& eav:_final)
			{
				for(auto& ear:a._initial)
				{
					ajout_transition_instantanee(eav,ear);
				}
			}
			_final = a._final;
		}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
