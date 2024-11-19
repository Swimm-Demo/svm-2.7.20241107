---
title: Handling Pending Interruptions Flow
---
This document explains the flow of handling pending interruptions within a virtual machine. The process involves several steps, including waiting for commands, retrieving necessary data, and managing the interruptions.

The flow starts with the system waiting for a command. Once a command is received, the system retrieves the necessary data and processes it. If an interruption occurs, it is added to the list of pending interruptions. The system then continues to monitor and manage these interruptions to ensure smooth operation.

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

# Where is this flow used?

This flow is used multiple times in the codebase as represented in the following diagram:

```mermaid
graph TD;
      subgraph srcmachineelements["src/machine/elements"]
00055ebdbae4797690010b2b2457e038d5e8e65e5219139dfff8a44793503e7a(creation_processus):::rootsStyle --> e832cf371011944993279344360dbf0651ad87adf17fe3c76cf1b699303f1804(execution)
end

subgraph srcmachineelementsprocesseur["src/machine/elements/processeur"]
e832cf371011944993279344360dbf0651ad87adf17fe3c76cf1b699303f1804(execution) --> b87564347004fbdfe28adcb5cbe24fcfd35302e366cdbeeaaf82278d816b02f7(interruption)
end

subgraph srcmachineelementsprocesseur["src/machine/elements/processeur"]
b87564347004fbdfe28adcb5cbe24fcfd35302e366cdbeeaaf82278d816b02f7(interruption) --> 1a10593a720f6961b79a7bfea548c1e8e21dc17103510f96d624dd869e08704d(interruptions_en_attente)
end

subgraph srcmachineelementsprocesseur["src/machine/elements/processeur"]
4f3c4be011b8a8e8f62de1983872e043ada6fe065ce74b3010648ccf7bd8a02f(decoration):::rootsStyle --> b87564347004fbdfe28adcb5cbe24fcfd35302e366cdbeeaaf82278d816b02f7(interruption)
end

subgraph srcmachineelementsprocesseur["src/machine/elements/processeur"]
0587223ce8ac5f187b1ddc0313922b6be78126860546c0f4279b43f58c5acdbe(nettoie_interruptions):::rootsStyle --> 1a10593a720f6961b79a7bfea548c1e8e21dc17103510f96d624dd869e08704d(interruptions_en_attente)
end


      classDef mainFlowStyle color:#000000,fill:#7CB9F4
classDef rootsStyle color:#000000,fill:#00FFF4
classDef Style1 color:#000000,fill:#00FFAA
classDef Style2 color:#000000,fill:#FFFF00
classDef Style3 color:#000000,fill:#AA7CB9
```

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
