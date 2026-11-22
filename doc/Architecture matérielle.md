
# Carte graphique
- Une **carte graphique** possède largement plus de cœurs qu'un CPU (environ 10000 cœurs pour un GPU par rapport à environ 10 pour un CPU)
- La carte graphique est plus restrictive sur ce qu'elle peut faire tourner par rapport au CPU. Par exemple, il est impossible de faire tourner un OS ou un logiciel sur une carte graphique
- La carte graphique est plus adaptée à effectuer les mêmes opérations sur un très grand jeu de données (SIMD). 
- La carte graphique est composée de plusieurs composants, parmi eux : 
	- **GPU** : 
		- le "CPU" de la carte graphique, est composé de transistors et de cœurs. 
		- La hiérarchie est la suivante : 
			- le GPU est composée de **GPCs** (Graphics Processor Cluster)
			- chaque GPC contient des **SMs** (Streaming Multiprocessor)
			- chaque SM contient des cœurs (**CUDA cores** <=> (unified) shader cores, **Tensor cores** et **Ray Tracing cores** => ce sont les cœurs du GPU)
		- Chaque type de cœur réalise des opérations spécifiques : 
			- CUDA core : opérations arithmétiques, c'est ce type de coeur qui exécute les shaders. Ces coeurs disposent de threads (SIMT)
			- Tensor core : opérations matricielles
			- Ray Tracing core : opérations de ray tracing
		- A l'époque, chaque type de shader (vertex, fragment etc.) avait son propre type de coeur qui lui était réservé, chacun avec une architecture matérielle et une ISA différentes. Cela est peu efficace si une charge de travail demande beaucoup de coeurs pour un certain type de shader (ex : vertex) et peu pour un autre (ex : fragment) : en effet, peu des coeurs pour les fragment shaders seraient utilisés alors qu'ils auraient pu l'être pour faire tourner les opérations des vertex shaders dont les coeurs sont eux surchargés.
		  Aujourd'hui, les coeurs qui font tourner les shaders (= CUDA cores) sont unifiés : ils ont tous les mêmes capacités matérielles (quasiment la même ISA). On appelle cette architecture une **unified shader architecture**
		- Le GPU dispose de registres, d'un cache L1 privé à chaque SM et d'un cache L2 partagé entre tous les SMs
	- Connectiques : 
		- Display ports / HDMIs où on branche l'écran
		- prise où on branche l'alimentation
		- connecteur PCIe que l'on connecte à la carte mère
	- Graphics memory chips : 
		- c'est la **VRAM** (Video RAM), équivalent de la RAM pour le CPU 
		- elles utilisent la technologie **GDDR6(X)**/**GDDR7**
		- c'est la mémoire la plus grande mais aussi la plus lente
- Exemple d'utilisation de la carte graphique : effectuer la même opérations sur tous les pixels de l'écran. On peut réaliser l'opération en parallèle sur chaque cœur du GPU car les opérations n'ont pas de dépendance entres elles