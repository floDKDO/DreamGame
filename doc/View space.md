# Caméra, view space et view matrix

## Caméra
OpenGL n'a pas de notion de caméra : on doit l'implémenter nous-mêmes.
La caméra a une position et une orientation dans le world space. Pour passer du local space au world space, il faut multiplier la position de la caméra par sa model matrix (passage du local space de la caméra au world space), cette transformation étant appelée **camera transformation**.
La caméra doit pointer vers les **z négatifs**, c'est-à-dire vers l'avant / l'intérieur de l'écran.
Voir https://www.songho.ca/opengl/gl_transform.html#example1 qui propose un GUI.

## View space
Dans cet espace de coordonnées, la position de **la caméra est l'origine** (0, 0, 0) : toutes les coordonnées sont donc exprimées selon la position et l'orientation de la caméra car c'est eux qui définissent les 3 axes/vecteurs x, y et z du view space. On voit donc ce que "voit" la caméra.
Cet espace de coordonnées suit par défaut la **right-hand rule**, donc que l'axe des z (positifs) pointe vers moi devant l'écran. 

## View matrix
Aussi appelée **lookAt** matrix, elle a pour objectif de convertir des coordonnées world en coordonnées view. On sait que la *camera transformation* transforme les coordonnées du local space de la caméra vers le world space. On a donc que l'inverse de cette matrice permet de passer du world space vers le local space de la caméra, ce qui représente donc la view matrix.

Il faut noter que cette matrice ne déplace rien, elle permet juste de passer d'un système de coordonnées à un autre (ici du world space à view space). En effet, elle contient notamment les 3 axes/vecteurs du view space.

Voici les différentes étapes à effectuer pour obtenir cette view matrix, celle-ci étant composée d'une partie translation et d'une partie rotation.

### Partie translation
Soit la position de la caméra qui vaut (x, y, z) dans le world space. Pour passer du local space de la caméra au world space, on a additionné (x, y, z) à sa position. 
Pour effectuer l'inverse, c'est-à-dire passer du world space au local space de la caméra, il faut **soustraire** (x, y, z).

### Partie rotation
Soit la caméra qui pointe vers un point (a, b, c) : elle possède donc une certaine rotation/orientation. 

On veut calculer les 3 vecteurs orthogonaux **x** (= *right*), **y** (= *up*) et **z** (= *forward*) qui forment le view space : 
- **Vecteur forward (z)** : ce vecteur part de la cible (<=> *target* <=> *center*) de la caméra (= position du joueur dans un jeu à la troisième personne) vers la position de la caméra (le vecteur obtenu "entre" dans la caméra, donc pointe vers les z négatifs). 
  On l'obtient avec le calcul suivant : $position\_camera - cible$.
  On pourrait se demander pourquoi ce vecteur ne "sort" pas de la caméra ($cible - position\_camera$). 
  La raison est la suivante : l'axe/vecteur z du view space doit, comme tout espace de coordonnées, pointer vers les z positifs. On sait que la view matrix contient les 3 axes du view space (x, y et z) : notre objectif ici est d'obtenir ce vecteur z. Or, l'opération $cible - position\_camera$ donne un vecteur qui pointe vers les z négatifs (sens inverse de ce que l'on cherche). C'est donc pour cela que l'on réalise l'opération $position\_camera - cible$ qui donne un vecteur qui pointe vers les z positifs, sens inverse de là où regarde la caméra. 

- **Vecteur right (x)** : on l'obtient en réalisant un produit vectoriel entre le vecteur forward et un vecteur qui pointe vers le haut dans le world space (ex : (0, 1, 0)).

- **Vecteur up (y)** : on l'obtient en réalisant un produit vectoriel entre le vecteur forward et le vecteur right.

### Obtention de la matrice
La view matrix est obtenue en multipliant deux matrices : 
- la première contient les 3 vecteurs obtenus dans la partie rotation. Cette matrice doit être inversée (= transposée car elle est une matrice orthogonale) pour réaliser l'opération inverse (si on ne l'inverse pas, on réalise l'opération "local space de la caméra vers world space").
- la deuxième contient, dans sa 4ème colonne, la translation obtenue dans la partie translation.

Au final, on obtient la view matrix qui permet de convertir des coordonnées du world space vers le view space (<=> local space de la caméra).

A noter que **glm** peut directement calculer cette matrice via la méthode **glm::lookAt** (ici équivalente à **glm::lookAtRH**, RH = Right-Hand).
