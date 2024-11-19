---
title: Global Configuration Overview
---
# Global Configuration Overview

Global Configuration refers to the centralized setup and definitions that are used throughout the system to ensure consistency and manageability. It includes various macros, type definitions, and utility functions that are used across different modules.

## Macros

Macros like `ASSERT` are defined to handle assertions with custom messages, ensuring that errors are caught and reported consistently throughout the codebase.

<SwmSnippet path="/src/global/global.h" line="57">

---

The `ASSERT` macro is defined in the global configuration to handle assertions with custom messages.

```c
#include <config.h>

#define ASSERT(exp, msg) assert(((void)msg, exp))
```

---

</SwmSnippet>

## Type Definitions

Type definitions such as <SwmToken path="src/global/global.h" pos="61:3:3" line-data="#define SHARED_PTR(Type) std::shared_ptr&lt; Type &gt;">`SHARED_PTR`</SwmToken>, <SwmToken path="src/global/global.h" pos="69:3:3" line-data="#define WEAK_PTR(Type) std::weak_ptr&lt; Type &gt;">`WEAK_PTR`</SwmToken>, and <SwmToken path="src/global/global.h" pos="76:3:3" line-data="#define UNIQUE_PTR(Type) std::unique_ptr&lt; Type &gt;">`UNIQUE_PTR`</SwmToken> are provided to standardize the use of smart pointers.

<SwmSnippet path="/src/global/global.h" line="61">

---

These macros define shared, weak, and unique pointers to standardize their usage across the system.

```c
#define SHARED_PTR(Type) std::shared_ptr< Type >

#define DECL_SHARED_PTR(Type) \
struct Type;\
typedef SHARED_PTR(Type) Type##SP;\
typedef SHARED_PTR(const Type) Type##CSP


#define WEAK_PTR(Type) std::weak_ptr< Type >

#define DECL_WEAK_PTR(Type) \
struct Type;\
typedef WEAK_PTR(Type) Type##WP;\
typedef WEAK_PTR(const Type) Type##CWP

#define UNIQUE_PTR(Type) std::unique_ptr< Type >

#define DECL_UNIQUE_PTR(Type) \
struct Type;\
typedef UNIQUE_PTR(Type) Type##UP;\
typedef UNIQUE_PTR(const Type) Type##CUP
```

---

</SwmSnippet>

## Utility Functions

The <SwmToken path="src/global/global.cpp" pos="28:4:4" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`Chaine`</SwmToken> struct offers common functionalities like string escaping.

<SwmSnippet path="/src/global/global.cpp" line="28">

---

The <SwmToken path="src/global/global.cpp" pos="28:6:6" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`echappe`</SwmToken> function in the <SwmToken path="src/global/global.cpp" pos="28:4:4" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`Chaine`</SwmToken> struct provides string escaping functionality.

```c++
std::string Chaine::echappe(const std::string& chaine)
{
	std::ostringstream resultat;
	for(auto it=chaine.cbegin() ; it!=chaine.cend() ; ++it)
	{
		switch(*it)
		{
			case '\n':
				resultat << "\\n";
				break;
			case '\r':
				resultat << "\\r";
				break;
			case '\t':
				resultat << "\\t";
				break;
			case '\\':
			case '"':
				resultat << "\\" << (*it);
				break;
			default:
```

---

</SwmSnippet>

## File Handling

The <SwmToken path="src/global/global.cpp" pos="163:2:2" line-data="bool Fichier::teste_fichier(std::string fichier)">`Fichier`</SwmToken> struct provides functions for file handling, such as testing if a file exists and opening a file.

<SwmSnippet path="/src/global/global.cpp" line="163">

---

The <SwmToken path="src/global/global.cpp" pos="163:4:4" line-data="bool Fichier::teste_fichier(std::string fichier)">`teste_fichier`</SwmToken> function in the <SwmToken path="src/global/global.cpp" pos="163:2:2" line-data="bool Fichier::teste_fichier(std::string fichier)">`Fichier`</SwmToken> struct tests if a file exists.

```c++
bool Fichier::teste_fichier(std::string fichier)
{
	if(fichier=="-")
		return true;
	if(::access(fichier.c_str(),R_OK))
		return false;
	struct stat buf;
	if(::stat(fichier.c_str(),&buf))
		return false;
	if(S_ISDIR(buf.st_mode))
		return false;
	return true;
}
```

---

</SwmSnippet>

## Logging

The <SwmToken path="src/global/global.h" pos="142:3:3" line-data="	struct Print">`Print`</SwmToken> struct provides a thread-safe way to handle logging and output operations.

<SwmSnippet path="/src/global/global.h" line="142">

---

The <SwmToken path="src/global/global.h" pos="142:3:3" line-data="	struct Print">`Print`</SwmToken> struct ensures thread-safe logging and output operations.

```c
	struct Print
	{
		Print() {}
		~Print()
		{
			std::lock_guard<std::mutex> v(Print::print_mutex());
			std::cerr << _oss.str() << std::endl;
		}
		template<typename T>
			Print& operator<<(const T& t)
			{
				_oss << t;
				return *this;
			}
		template<typename T>
			Print& operator<<(const T&& t)
			{
				_oss << t;
				return *this;
			}
		template<typename T>
```

---

</SwmSnippet>

## TCP Connections

The <SwmToken path="src/global/global.h" pos="208:3:3" line-data="	struct TCP">`TCP`</SwmToken> struct and its internal <SwmToken path="src/global/global.h" pos="193:3:3" line-data="	struct MemoireTCP">`MemoireTCP`</SwmToken> struct manage TCP connections and related memory operations.

<SwmSnippet path="/src/global/global.h" line="208">

---

The <SwmToken path="src/global/global.h" pos="208:3:3" line-data="	struct TCP">`TCP`</SwmToken> struct handles TCP connections and related memory operations.

```c
	struct TCP
	{
		TCP(const bool serveur, const std::string& ip, const std::string& port, const int id)
			:_ip_distante(ip), _port_distant(port), _id(id), _serveur(serveur)
		{
			if(_serveur)
			{
				_ip_locale.swap(_ip_distante);
				_port_local.swap(_port_distant);
			}
		}
		std::string _ip_locale;
		std::string _port_local;
		std::string _ip_distante;
		std::string _port_distant;
		int _id;
		bool _serveur;

		static bool resolution_ip_port(const struct sockaddr* sock, const socklen_t longueur, std::string& ip, std::string& port)
		{
			char host[4096+1];
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
