---
title: Introduction to Synchronisation in Machine Components
---
# Introduction to Synchronisation in Machine Components

Synchronisation is a mechanism used to control access to shared resources in a concurrent environment. It ensures that multiple threads or processes can operate on shared data without causing data corruption or inconsistency. In the Machine Components, synchronisation is implemented using various constructs such as mutexes and atomic variables.

## Synchronisation in Acces Struct

The `Acces` struct uses a boolean `_synchronise` and mutexes <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="362:12:12" line-data="				std::lock_guard&lt;std::mutex&gt; verrou(_protection);">`_protection`</SwmToken> and `_ecrivain` to manage access to shared resources. The `Lecteur` and `Ecrivain` structs are friends of `Acces` and utilize these synchronisation mechanisms to ensure safe read and write operations.

## Synchronisation in Notification Struct

The <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="139:3:3" line-data="	DECL_SHARED_PTR(Notification);">`Notification`</SwmToken> struct uses an atomic boolean <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="143:2:2" line-data="		:_notification(false) {}">`_notification`</SwmToken> to manage notification states in a thread-safe manner.

<SwmSnippet path="/src/machine/elements/synchronisation/evenement.h" line="139">

---

The <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="139:3:3" line-data="	DECL_SHARED_PTR(Notification);">`Notification`</SwmToken> struct defines an atomic boolean <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="143:2:2" line-data="		:_notification(false) {}">`_notification`</SwmToken> to ensure thread-safe notification state management.

```c
	DECL_SHARED_PTR(Notification);
	struct Notification
	{
		Notification()
		:_notification(false) {}
		void arme(const BoiteGeneriqueSP& boite)
		{
			_boite = boite;
		}
		void desarme()
		{
			_boite.reset();
		}
		void initialise()
		{
			_notification = false;
		}
		bool verifie()
		{
			return _notification.load();
		}
```

---

</SwmSnippet>

## Synchronisation in Livraison Struct

The <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="353:3:3" line-data="	struct Livraison">`Livraison`</SwmToken> struct employs mutexes to protect its internal data structures and ensure safe message delivery and retrieval.

<SwmSnippet path="/src/machine/elements/synchronisation/evenement.h" line="353">

---

The <SwmToken path="src/machine/elements/synchronisation/evenement.h" pos="353:3:3" line-data="	struct Livraison">`Livraison`</SwmToken> struct uses mutexes to protect its internal data structures, ensuring safe message delivery and retrieval.

```c
	struct Livraison
	{
		Livraison() = default;
		Livraison(const Livraison&) = delete;
		virtual ~Livraison()
		{
			SVM_TRACE("DESTRUCTION livraison");
			decltype(_boites) boites;
			{
				std::lock_guard<std::mutex> verrou(_protection);
				_boites.swap(boites);
			}
			for(auto& b:boites)
			{
				b.second->_fin.store(true,std::memory_order_relaxed);
				b.second->_attente.notify_all();
			}
		}
		bool creation_boite(const AdresseSP& adresse)
		{
			SVM_TRACE("AJOUT " << adresse);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
