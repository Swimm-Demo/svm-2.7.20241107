---
title: Overview of the Global Namespace
---
# What is Global Namespace

The Global namespace within the SVM project contains various utility structures and functions that are essential for different operations. It includes structures like <SwmToken path="src/global/global.cpp" pos="28:4:4" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`Chaine`</SwmToken>, <SwmToken path="src/global/global.cpp" pos="163:2:2" line-data="bool Fichier::teste_fichier(std::string fichier)">`Fichier`</SwmToken>, <SwmToken path="src/global/global.h" pos="142:3:3" line-data="	struct Print">`Print`</SwmToken>, <SwmToken path="src/global/global.h" pos="208:3:3" line-data="	struct TCP">`TCP`</SwmToken>, and the internal namespace <SwmToken path="src/global/global.h" pos="191:2:2" line-data="namespace Interne">`Interne`</SwmToken>.

## Chaine Struct

The <SwmToken path="src/global/global.cpp" pos="28:4:4" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`Chaine`</SwmToken> struct within the Global namespace provides functions for escaping and unescaping strings, duplicating strings, and escaping HTML content.

<SwmSnippet path="/src/global/global.cpp" line="28">

---

The <SwmToken path="src/global/global.cpp" pos="28:4:4" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`Chaine`</SwmToken> struct's <SwmToken path="src/global/global.cpp" pos="28:6:6" line-data="std::string Chaine::echappe(const std::string&amp; chaine)">`echappe`</SwmToken> function is responsible for escaping special characters in a string. It iterates through the string and replaces characters like newline (<SwmToken path="src/global/global.cpp" pos="35:4:5" line-data="			case &#39;\n&#39;:">`\n`</SwmToken>), carriage return (<SwmToken path="src/global/global.cpp" pos="38:4:5" line-data="			case &#39;\r&#39;:">`\r`</SwmToken>), and tab (<SwmToken path="src/global/global.cpp" pos="41:4:5" line-data="			case &#39;\t&#39;:">`\t`</SwmToken>) with their escaped versions.

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

## Fichier Struct

The <SwmToken path="src/global/global.cpp" pos="163:2:2" line-data="bool Fichier::teste_fichier(std::string fichier)">`Fichier`</SwmToken> struct offers functions to test file accessibility and to open files safely.

<SwmSnippet path="/src/global/global.cpp" line="163">

---

The <SwmToken path="src/global/global.cpp" pos="163:2:2" line-data="bool Fichier::teste_fichier(std::string fichier)">`Fichier`</SwmToken> struct's <SwmToken path="src/global/global.cpp" pos="163:4:4" line-data="bool Fichier::teste_fichier(std::string fichier)">`teste_fichier`</SwmToken> function checks if a file is accessible and not a directory. The <SwmToken path="src/global/global.cpp" pos="177:5:5" line-data="FILE *Fichier::ouvrir(std::string fichier)">`ouvrir`</SwmToken> function safely opens a file, ensuring it is not empty and handling standard input appropriately.

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

FILE *Fichier::ouvrir(std::string fichier)
{
	ASSERT(not fichier.empty(),"fichier vide en entree");
	if(fichier==FICHIER_STDIN)
	{
		return stdin;
	}
```

---

</SwmSnippet>

## Print Struct

The <SwmToken path="src/global/global.h" pos="142:3:3" line-data="	struct Print">`Print`</SwmToken> struct is designed for thread-safe printing to standard error, utilizing a mutex to ensure that print operations do not interfere with each other.

<SwmSnippet path="/src/global/global.h" line="142">

---

The <SwmToken path="src/global/global.h" pos="142:3:3" line-data="	struct Print">`Print`</SwmToken> struct uses a mutex to lock the printing operation, ensuring thread safety. It overrides the `<<` operator to append data to an internal string stream, which is then printed to standard error upon destruction.

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

## TCP Struct

The <SwmToken path="src/global/global.h" pos="208:3:3" line-data="	struct TCP">`TCP`</SwmToken> struct within the Global namespace handles TCP connections, including IP and port resolution, and provides functions for opening and closing connections.

<SwmSnippet path="/src/global/global.h" line="208">

---

The <SwmToken path="src/global/global.h" pos="208:3:3" line-data="	struct TCP">`TCP`</SwmToken> struct manages TCP connections by storing IP and port information and providing methods for resolving IP addresses and ports. It also includes functionality for both server and client modes.

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

## Interne Namespace

The <SwmToken path="src/global/global.h" pos="191:2:2" line-data="namespace Interne">`Interne`</SwmToken> namespace within Global contains internal structures like <SwmToken path="src/global/global.h" pos="193:3:3" line-data="	struct MemoireTCP">`MemoireTCP`</SwmToken>, which manages memory for TCP operations.

<SwmSnippet path="/src/global/global.h" line="191">

---

The <SwmToken path="src/global/global.h" pos="193:3:3" line-data="	struct MemoireTCP">`MemoireTCP`</SwmToken> struct within the <SwmToken path="src/global/global.h" pos="191:2:2" line-data="namespace Interne">`Interne`</SwmToken> namespace handles memory allocation and deallocation for TCP operations, ensuring that resources are properly managed.

```c
namespace Interne
{
	struct MemoireTCP
	{
		MemoireTCP()
		{
			res0 = nullptr;
		}

		~MemoireTCP()
		{
			if(res0)
				::freeaddrinfo(res0);
		}
		struct addrinfo *res0;
	};
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
