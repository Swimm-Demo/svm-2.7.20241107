---
title: Handling Write Breakpoint Flow
---
This document explains the process of handling a write breakpoint. The process involves setting a breakpoint, waiting for a command, and retrieving the necessary information.

The flow starts with setting a write breakpoint, then it waits for a command to be issued. Once the command is received, it retrieves the necessary information to proceed.

Here is a high level diagram of the flow, showing only the most important functions:

```mermaid
graph TD;
      subgraph srcmachinedebugueurdebugueurcpp["src/machine/debugueur/debugueur.cpp"]
aa5ed0ab7c5ef749b9b43c9ace4f63710d9a5affaaf4f4c9ff6700ea1e50485a(point_arret) --> b9d6ba680f9311844758d78d12e4784a1769eec4cb5fcb8c5a6fa33505e099ce(attente_commande)
end

subgraph srcmachineelements["src/machine/elements"]
b9d6ba680f9311844758d78d12e4784a1769eec4cb5fcb8c5a6fa33505e099ce(attente_commande) --> 970ba4a1b78e9c73fe22ff1da4d6bb06bed9d62fdaf42b11b33ce7f20069b122(recupere)
end


      classDef mainFlowStyle color:#000000,fill:#7CB9F4
classDef rootsStyle color:#000000,fill:#00FFF4
classDef Style1 color:#000000,fill:#00FFAA
classDef Style2 color:#000000,fill:#FFFF00
classDef Style3 color:#000000,fill:#AA7CB9
```

# Flow drill down

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
