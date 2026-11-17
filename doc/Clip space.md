# Définitions  
- Il existe deux principaux types de perspectives (voir https://www.songho.ca/opengl/gl_transform.html#example2 qui propose un GUI) : 
	- **Perspective** : 
		- son **view frustum** est une pyramide dont la base est le *far plane* et la pointe carrée le *near plane* (les 4 sommets des deux plans sont reliés ensemble). Ces lignes se rejoignent en un point dans la caméra, appelé *centre de projection*. Tout ce qui n'appartient pas au view frustum sera clipped.
		- elle ne conserve pas les parallèles : deux droites qui étaient parallèles avant la projection perspective ne le resteront pas forcément après l'application de la projection perspective
		- elle prend en compte la perspective : plus un objet de la scène est proche du near plane, plus il apparaîtra grand sur l'écran 2D et inversement (modification de "w" pour réaliser cela)
	- **Orthographique** : 
		- c'est un type de **projection parallèle**
		- son **view volume** est une boîte rectangulaire dont le near plane et le far plane sont les deux faces qui sont face à la caméra. Les lignes issues des sommets de ces deux faces ne se rejoignent jamais. Tout ce qui n'appartient pas au view frustum sera clipped.
		- elle conserve les parallèles : deux droites qui étaient parallèles avant la projection orthographique le resteront après l'application de la projection orthographique
		- elle ne prend pas en compte la perspective : peu importe la distance de l'objet dans la scène, sa taille ne sera pas modifiée sur l'écran 2D (pas de scale => "w" n'est pas modifié)
- **Centre de projection** : point 3D qui représente la position de l'œil / de la lentille de la caméra
- **Point de fuite** : *vanishing point* en anglais, point 2D sur l'image / écran où les droites parallèles d'une espace 3D semblent se rejoindre
- **Plan de projection** : plan sur lequel la scène en 3D est projetée
- **Canonical view volume** : aussi appelé *unit cube*, le view frustum / view volume (selon le type de projection utilisée) est transformé en un canonical view volume lors de la projection
- **Clip space** : après avoir appliqué la projection, le view frustum / view volume (selon le type de projection utilisée) est un canonical view volume. Comme il est plus simple de savoir si une primitive se trouve à l'intérieur d'un cube plutôt que dans une pyramide, l'étape de *clipping* est réalisée dans ce space

# Informations supplémentaires
- Les paramètres *near* et *far* des fonctions de *glm* représentent une distance depuis la caméra et non leur position