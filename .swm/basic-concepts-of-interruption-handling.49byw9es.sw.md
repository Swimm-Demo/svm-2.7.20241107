---
title: Basic Concepts of Interruption Handling
---
# Interruption Handling Overview

Interruption handling refers to the mechanisms and processes used to manage hardware interruptions within the virtual machine environment. The <SwmToken path="src/machine/interruption/interruption.cpp" pos="30:2:2" line-data="void Interruption::bloque_signaux()">`Interruption`</SwmToken> class is central to this functionality, providing methods to block signals, create interruptions, and handle received signals.

# Blocking Signals

The method <SwmToken path="src/machine/interruption/interruption.cpp" pos="30:4:4" line-data="void Interruption::bloque_signaux()">`bloque_signaux`</SwmToken> is used to block certain signals from interrupting the process, ensuring that only specific signals are allowed to pass through.

<SwmSnippet path="/src/machine/interruption/interruption.cpp" line="30">

---

The <SwmToken path="src/machine/interruption/interruption.cpp" pos="30:4:4" line-data="void Interruption::bloque_signaux()">`bloque_signaux`</SwmToken> function blocks specific signals such as <SwmToken path="src/machine/interruption/interruption.cpp" pos="34:7:7" line-data="	::sigdelset(&amp;signaux_bloques,SIGSTOP);">`SIGSTOP`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="35:7:7" line-data="	::sigdelset(&amp;signaux_bloques,SIGTSTP);">`SIGTSTP`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="36:7:7" line-data="	::sigdelset(&amp;signaux_bloques,SIGCONT);">`SIGCONT`</SwmToken>, and <SwmToken path="src/machine/interruption/interruption.cpp" pos="37:7:7" line-data="	::sigdelset(&amp;signaux_bloques,SIGSEGV);">`SIGSEGV`</SwmToken> to ensure they do not interrupt the process.

```c++
void Interruption::bloque_signaux()
{
	sigset_t signaux_bloques;
	::sigfillset(&signaux_bloques);
	::sigdelset(&signaux_bloques,SIGSTOP);
	::sigdelset(&signaux_bloques,SIGTSTP);
	::sigdelset(&signaux_bloques,SIGCONT);
	::sigdelset(&signaux_bloques,SIGSEGV);
	::sigprocmask(SIG_BLOCK,&signaux_bloques,nullptr);
}
```

---

</SwmSnippet>

# Creating Interruptions

The <SwmToken path="src/machine/interruption/interruption.cpp" pos="48:4:4" line-data="InterruptionSP Interruption::creation_interruptions()">`creation_interruptions`</SwmToken> method sets up the necessary signal handlers and starts a dedicated thread to wait for and handle incoming signals.

<SwmSnippet path="/src/machine/interruption/interruption.cpp" line="48">

---

The <SwmToken path="src/machine/interruption/interruption.cpp" pos="48:4:4" line-data="InterruptionSP Interruption::creation_interruptions()">`creation_interruptions`</SwmToken> function initializes a shared pointer to an <SwmToken path="src/machine/interruption/interruption.cpp" pos="48:2:2" line-data="InterruptionSP Interruption::creation_interruptions()">`Interruption`</SwmToken> object, sets up signal handlers, and starts a thread to handle incoming signals.

```c++
InterruptionSP Interruption::creation_interruptions()
{
	InterruptionSP interruption = std::make_shared<Interruption>();
	struct sigaction sigaction;
	sigset_t sigset;
	::sigemptyset(&sigset);
	sigaction.sa_handler=::gestionnairesignalvide;
	sigaction.sa_flags=0;
	sigaction.sa_mask=sigset;
	::sigaction(REACTIVE_SIGNAL,&sigaction,nullptr);
	try
	{
		InterruptionWP avatar = interruption;
		interruption->_effecteur = std::thread([avatar] ()
			{
				SVM_TRACE("creation HwInt");
				sigset_t signaux_attendus;
				::sigfillset(&signaux_attendus);
				for( ; ; )
				{
					int signal_recu = -1;
```

---

</SwmSnippet>

# Handling Received Signals

When a signal is received, the <SwmToken path="src/machine/interruption/interruption.cpp" pos="95:4:4" line-data="void Interruption::interruptions_materielles(const SVM::Machine::Element::Systeme::SystemeSP&amp; systeme, const int signal_recu)">`interruptions_materielles`</SwmToken> method is invoked to determine the type of interruption and take appropriate action, such as updating counters or notifying the system.

<SwmSnippet path="/src/machine/interruption/interruption.cpp" line="95">

---

The <SwmToken path="src/machine/interruption/interruption.cpp" pos="95:4:4" line-data="void Interruption::interruptions_materielles(const SVM::Machine::Element::Systeme::SystemeSP&amp; systeme, const int signal_recu)">`interruptions_materielles`</SwmToken> function handles different types of signals, such as <SwmToken path="src/machine/interruption/interruption.cpp" pos="102:3:3" line-data="		case SIGQUIT:">`SIGQUIT`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="104:3:3" line-data="		case SIGABRT:">`SIGABRT`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="107:3:3" line-data="		case SIGALRM:">`SIGALRM`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="108:3:3" line-data="		case SIGTERM:">`SIGTERM`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="109:3:3" line-data="		case SIGINT:">`SIGINT`</SwmToken>, <SwmToken path="src/machine/interruption/interruption.cpp" pos="110:3:3" line-data="		case SIGHUP:">`SIGHUP`</SwmToken>, and <SwmToken path="src/machine/interruption/interruption.cpp" pos="113:3:3" line-data="		case SIGILL:">`SIGILL`</SwmToken>, and takes appropriate actions based on the signal received.

```c++
void Interruption::interruptions_materielles(const SVM::Machine::Element::Systeme::SystemeSP& systeme, const int signal_recu)
{
	std::lock_guard<std::mutex> verrou(_protection);
	SVM_Valeur::Interruption::InterruptionInterne interne;
	bool quitter = false;
	switch(signal_recu)
	{
		case SIGQUIT:
			quitter = true;
		case SIGABRT:
			interne = SVM_Valeur::Interruption::InterruptionInterne::ECHEC;
			break;
		case SIGALRM:
		case SIGTERM:
		case SIGINT:
		case SIGHUP:
			interne = SVM_Valeur::Interruption::InterruptionInterne::TERMINAISON;
			break;
		case SIGILL:
			interne = SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR;
			break;
```

---

</SwmSnippet>

# Tracking Interruption Counts

The <SwmToken path="src/machine/interruption/interruption.h" pos="54:3:3" line-data="	struct CompteurInterruption">`CompteurInterruption`</SwmToken> class is used to keep track of the number of times an interruption has occurred, providing methods to increment and decrement the counter.

<SwmSnippet path="/src/machine/interruption/interruption.h" line="54">

---

The <SwmToken path="src/machine/interruption/interruption.h" pos="54:3:3" line-data="	struct CompteurInterruption">`CompteurInterruption`</SwmToken> class maintains an atomic counter to track interruptions, with methods to increment (<SwmToken path="src/machine/interruption/interruption.h" pos="58:3:3" line-data="		void ecriture() { ++_compteur; }">`ecriture`</SwmToken>) and decrement (<SwmToken path="src/machine/interruption/interruption.h" pos="59:3:3" line-data="		bool lecture()">`lecture`</SwmToken>) the counter.

```c
	struct CompteurInterruption
	{
		CompteurInterruption()
		:_compteur(0) {}
		void ecriture() { ++_compteur; }
		bool lecture()
		{
			if(_compteur.load(std::memory_order_relaxed)==0)
				return false;
			--_compteur;
			return true;
		}
		size_t compteur() const
		{
			return _compteur.load(std::memory_order_relaxed);
		}
		private:
			std::atomic<size_t> _compteur;
	};
```

---

</SwmSnippet>

# Handling Interruption Failures

The <SwmToken path="src/machine/interruption/interruption.h" pos="44:1:1" line-data="		InterruptionNonLance(const std::string&amp; details)">`InterruptionNonLance`</SwmToken> exception is thrown if an interruption cannot be started, indicating a failure in the interruption handling process.

<SwmSnippet path="/src/machine/interruption/interruption.h" line="44">

---

The <SwmToken path="src/machine/interruption/interruption.h" pos="44:1:1" line-data="		InterruptionNonLance(const std::string&amp; details)">`InterruptionNonLance`</SwmToken> exception provides details about the failure when an interruption cannot be started.

```c
		InterruptionNonLance(const std::string& details)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Hardware interruption not started: " << details);
		}
		virtual ~InterruptionNonLance() {}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
