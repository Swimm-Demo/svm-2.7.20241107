---
title: Exploring Installation in Global Configuration
---
# Overview

Installation in Global Configuration refers to the setup and configuration of various parameters and directories required for the Simple Virtual Machine (SVM) to function correctly. The <SwmToken path="src/global/installation.h" pos="39:5:5" line-data="			auto v=Installation::variables();">`Installation`</SwmToken> struct in <SwmPath>[src/global/installation.h](src/global/installation.h)</SwmPath> provides methods to retrieve configuration values and keys, ensuring that the necessary environment variables and paths are correctly set.

# Methods in Installation Struct

The <SwmToken path="src/global/installation.h" pos="39:7:7" line-data="			auto v=Installation::variables();">`variables`</SwmToken> function within the <SwmToken path="src/global/installation.h" pos="39:5:5" line-data="			auto v=Installation::variables();">`Installation`</SwmToken> struct returns a map of key-value pairs representing different configuration settings such as <SwmToken path="src/global/installation.h" pos="61:8:8" line-data="				{ &quot;name&quot;, PACKAGE_NAME },">`PACKAGE_NAME`</SwmToken>, <SwmToken path="src/global/installation.h" pos="62:8:8" line-data="				{ &quot;version&quot;, PACKAGE_VERSION },">`PACKAGE_VERSION`</SwmToken>, <SwmToken path="src/global/installation.h" pos="63:8:8" line-data="				{ &quot;debug_site&quot;, DEBUGGER_SITE },">`DEBUGGER_SITE`</SwmToken>, and various directory paths. These configuration settings are crucial for the proper installation and execution of the SVM, as they define where binaries, libraries, and other resources are located.

<SwmSnippet path="/src/global/installation.h" line="37">

---

The <SwmToken path="src/global/installation.h" pos="37:7:7" line-data="		static std::string valeur(const std::string&amp; clef)">`valeur`</SwmToken> method allows retrieval of specific configuration values based on a key. It searches through the variables map and throws a <SwmToken path="src/global/installation.h" pos="43:3:3" line-data="				throw ClefInvalide();">`ClefInvalide`</SwmToken> exception if the key is not found.

```c
		static std::string valeur(const std::string& clef)
		{
			auto v=Installation::variables();
			auto it = v.find(clef);
			if(it==v.end())
			{
				throw ClefInvalide();
			}
			return it->second;
		}
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/installation.h" line="47">

---

The <SwmToken path="src/global/installation.h" pos="47:12:12" line-data="		static std::set&lt;std::string&gt; clefs()">`clefs`</SwmToken> method provides a set of all available configuration keys. It iterates through the variables map and collects all the keys into a set.

```c
		static std::set<std::string> clefs()
		{
			std::set<std::string> r;
			for(const auto &c : Installation::variables())
			{
				r.insert(r.end(),c.first);
			}
			return r;
		}
```

---

</SwmSnippet>

# Usage Example

In <SwmPath>[src/machine/interface/svm.cpp](src/machine/interface/svm.cpp)</SwmPath>, the <SwmToken path="src/global/installation.h" pos="39:5:5" line-data="			auto v=Installation::variables();">`Installation`</SwmToken> struct is used to retrieve a configuration value using the <SwmToken path="src/global/installation.h" pos="37:7:7" line-data="		static std::string valeur(const std::string&amp; clef)">`valeur`</SwmToken> method.

<SwmSnippet path="/src/machine/interface/svm.cpp" line="4496">

---

Example usage of the <SwmToken path="src/machine/interface/svm.cpp" pos="4497:13:13" line-data="		auto v = SVM::Global::Installation::valeur(key);">`valeur`</SwmToken> method to retrieve a configuration value.

```c++
	{
		auto v = SVM::Global::Installation::valeur(key);
		return ::svm_string_new(svm,v.c_str(),v.size());
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
