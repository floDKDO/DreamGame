# Définitions
- **API graphique** : elle définit un ensemble de fonctions permettant d'interagir avec le GPU, offrant une interface commune pour tous les types de GPUs peu importe leur architecture matérielle. Exemples : OpenGL, Vulkan, Direct3D, Metal...
- **Driver graphique** : il implémente en réelles commandes GPU les actions qui sont définies par l'API graphique. Il doit prendre en compte l'architecture matérielle du GPU car il communique directement avec lui. C'est du code noyau qui s'exécute de manière privilégiée.
- **Langage de shader** : langage qui permet la réimplémentation des étapes du pipeline graphique (= les rendre programmables via l'écriture de shaders). Exemples : GLSL, HLSL...
- **SPIR-V** : langage intermédiaire pour les langages de shaders. Par exemple, GLSL et HLSL peuvent être représentés en SPIR-V
- **CUDA / OpenCL** : deux langages qui permettent le *GPGPU* (General-Purpose computing on Graphics Processing Units), c'est-à-dire d'effectuer des calculs généralistes sur GPU. CUDA est propriétaire NVIDIA, OpenCL est open source
- **Pourquoi les jeux vidéos demandent de compiler leurs shaders ?** : par rapport aux CPUs qui ont des ISA standardisées (x86, ARM...), presque chaque GPU dispose de sa propre ISA. Il est donc impossible pour un jeu d'embarquer une version compilée des shaders pour chaque ISA possible : il y en a beaucoup trop. En effet, même des CPUs d'un même vendeur ont une ISA différente. Les shaders peuvent néanmoins être préalablement compilés en SPIR-V mais devront quand même être compilés en "binaire" sur la machine du joueur. 
  Il faut noter que cela ne concerne que les ordinateurs personnels où n'importe quel modèle de carte graphique peut être installé. Dans le cas d'une console, le matériel étant connu à l'avance, les shaders pourront donc être directement compilés dans le jeu.

# Informations sur OpenGL
- **Taille des buffers dans un framebuffer** : 
	- Cas du framebuffer présent par défaut : chaque buffer a la taille de la fenêtre (cela est logique comme ce framebuffer est créé par SDL dans mon cas et c'est cette dernière qui gère la fenêtre)
	- Cas d'un FBO : chaque buffer a la taille qu'on a précisé lors de sa création (les deux derniers paramètres de ```glTextureStorage2D()``` et de ```glNamedRenderbufferStorage()``` pour les textures et les renderbuffers respectivement)
- **Attacher un stencil buffer seul à un framebuffer** : cela n'est pas vraiment supporté (voir https://community.khronos.org/t/fbo-and-texture-about-gl-stencil-attachment/61912/3)
- **Column-major ou row-major** : 
	- voir https://fgiesen.wordpress.com/2012/02/12/row-major-vs-column-major-row-vectors-vs-column-vectors/ et https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry//row-major-vs-column-major-vector.html
	- column-major et row-major dans le contexte d'une matrice peuvent signifier deux choses : 
		- comment les éléments des matrices sont stockés en mémoire (colonne par colonne pour column-major et ligne par ligne pour row-major) : OpenGL/GLSL stocke en mémoire les matrices en column-major alors que les langages C et C++ les stockent en row-major. La façon dont sont stockées les matrices en mémoire n'influent en rien les opérations réalisées sur ces matrices (si les deux matrices multipliées sont stockées de la même façon bien-sûr)
		- comment traiter les vecteurs : (comme une matrice Nx1 (vecteur colonne => column-major) ou 1xN (vecteur ligne => row-major)) : OpenGL/GLSL traite les vecteurs comme des vecteurs colonnes => column-major
	- les matrices dans *glm* sont représentées en column-major (comme OpenGL/GLSL car *glm* est basé sur ces derniers). Exemple : 
	```
	glm::mat4 matrix(1.0f);
	std::cout << matrix[0] << std::endl; //affiche la première colonne de la matrice
	std::cout << matrix[1][2] << std::endl; //affiche la troisième ligne de la deuxième colonne de la matrice
	```
	- les types matrices *glm::matXxY* signifient que la matrice dispose de X colonnes et de Y lignes, ce qui est l'inverse de la notation mathématique
	- dans le cas d'une multiplication matrice/vecteur, on parle de : 
		- left/pre-multiplication quand le vecteur est un vecteur ligne (row-major) et qu'il est à gauche dans la multiplication => 1xN \* NxN
		- right/post-multiplication quand le vecteur est un vecteur colonne (column-major) et qu'il est à droite dans la multiplication => NxN \* Nx1
	- GLSL propose les deux layouts suivants pour le stockage des matrices : 
		- ```layout(column_major)``` : par défaut
		- ```layout(row_major)``` : GLSL récupère les éléments de la matrice comme si elle était stockée en row_major mais physiquement, la matrice reste stockée en column-major
	- les trois multiplications matrice/vecteur suivantes sont équivalentes : 
	```
	mat4 mvp = projection_matrix_ * view_matrix_ * model_matrix_;
	gl_Position = mvp * vec4(position_attribute_, 1.0); //4x4 * 4x1 (= vecteur colonne)
	```
	```
	mat4 mvp = projection_matrix_ * view_matrix_ * model_matrix_;
	gl_Position = vec4(position_attribute_, 1.0) * transpose(mvp); //1x4 * 4x4 (= vecteur ligne)
	```
	```
	mat4 mvp = projection_matrix_ * view_matrix_ * model_matrix_;
	gl_Position = vec4(position_attribute_, 1.0) * mvp; //1x4 * 4x4 (= vecteur ligne). Cette multiplication fonctionne uniquement si la matrice mvp possède layout row-major "layout(row_major)"
	```
	=> GLSL "transforme" automatiquement le vecteur en vecteur colonne ou vecteur ligne selon l'endroit où il est placé dans la multiplication (voir https://stackoverflow.com/questions/9863875/glm-how-to-transpose-a-vector)  
	- la fonction ```glUniformMatrix*()``` possède le paramètre *transpose*. Si sa valeur vaut GL\_FALSE, alors la matrice spécifiée est considérée comme étant en column-major alors que s'il vaut GL\_TRUE, alors elle est considérée comme étant en row-major (elle sera transposée pour devenir column-major). Si je passe une matrice de *glm* (ex : *glm::mat4*), comme elles sont représentées en column-major et qu'OpenGL attend aussi que la matrice soit en column-major, je dois mettre *GL\_FALSE* comme valeur à ce paramètre ("*Notice that all matrix types are _column-major_ rather than row-major. Hence the need to pass GL_FALSE to glUniformMatrix4fv.*").
	- Conclusion : OpenGL/GLSL/glm stocke les matrices en mémoire en **column-major** et les vecteurs sont des **vecteurs colonnes**. Dans une matrice de transformation, ses trois premières colonnes représente un axe/vecteur du système de coordonnées en question et sa dernière colonne un vecteur de translation