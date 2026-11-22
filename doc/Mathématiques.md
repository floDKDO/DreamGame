# Vecteurs

## Définition
Un vecteur classique, aussi appelé **free vector**, possède une **direction** et une **magnitude**, et ne possède pas de direction.
Il existe plusieurs types de vecteurs : 
- **vecteur direction** : seule la direction du vecteur nous importe (= vecteur **unitaire**) 
- **vecteur position** : vecteur partant de l'origine O vers un point P (x, y) est le vecteur position OP. Il désigne le déplacement nécessaire à effectuer depuis O pour atteindre P. Il a été placé à l'origine mais n'a quand même pas de position !
- **vecteur déplacement** : vecteur AB entre deux points A et B, s'obtient en effectuant la soustraction $B - A$
Il faut noter que ces types de vecteurs restent des vecteurs : ils n'ont rien de spécial, à part qu'ils désignent un cas particulier.

## Opérations

### Addition
Soient deux vecteurs A et B. On souhaite obtenir le vecteur C qui vaut l'addition de A avec B ($C = A + B$). Il suffit d'additionner les composantes et A et B une à une pour obtenir C.

Deux méthodes visuelles sont possibles : 
- **méthode du triangle** : coller le début de B sur la fin de A. Le vecteur C devient l'hypoténuse du triangle (pour effectuer le déplacement du vecteur A et du vecteur B, je pars du début de A pour arriver à la fin de B).
- **méthode du parallélogramme** : faire partir A et B de l'origine et construire un parallélogramme avec A et B comme deux premiers côtés. Le vecteur C devient la diagonale du parallélogramme.

![[vector_add.png | center]]

### Soustraction
Soient deux vecteurs A et B. On souhaite obtenir le vecteur C qui vaut la soustraction de A avec B ($C = A - B$). Il suffit de soustraire les composantes et A et B une à une pour obtenir C.

Les deux méthodes visuelles de l'addition fonctionnent ici. Il suffit juste d'inverser le sens du vecteur B car $A - B <=> A + (-B)$ et la négation d'un vecteur inverse le sens de sa flèche.

![[vector_sub.png | center]]

## Représentation informatique
On peut représenter des vecteurs de toutes tailles avec la librairie **glm** (type *glm::vecX*). Il faut noter qu'un objet de type vecteur peut à la fois représenter un vecteur ou un point ! La différentiation pourra être réalisée avec la valeur de leur composante *w* (si $w = 0.0$, alors c'est un vecteur, sinon c'est un point).


# Matrices

## Matrice inverse
Si la matrice M est une matrice de transformation, la matrice $M^{-1}$, la matrice inverse de M, annule la transformation appliquée par M.

L'inverse d'une matrice de transformation qui effectue une translation (4ème colonne) revient à rendre la translation négative.

## Matrice orthogonale
Si une matrice est orthogonale, alors son inverse est égal à sa transposée. Dans ce cas là, autant calculer sa transposée car il est moins coûteux de calculer la transposée plutôt que l'inverse.

## Déterminant
Le déterminant, dans le cas 2D, représente l'aire du carré / parallélogramme défini par les 2 vecteurs i et j de la base après application de la matrice de transformation.

Le déterminant, dans le cas 3D, représente l'aire du cube / parallélépipède défini par les 3 vecteurs i, j et k de la base après application de la matrice de transformation.

Exemple : 
![[determinant.png | center]]
Sur ce schéma, toutes les aires après application de la matrice de transformation sont 6 fois plus grandes : on est passé d'une aire égale à 1 à une aire égale à 6.

Signification de la valeur du déterminant selon son signe : 
- $det(M) > 0$ : cas classique (l'orientation de l'espace est conservée)
- $det(M) = 0$ : le parallélogramme devient une ligne
- $det(M) < 0$ : la valeur absolue du déterminant négatif est l'aire du parallélogramme, le signe négatif indique que l'orientation de l'espace a été inversé (= symétrie)
	- cas 2D : l'angle orienté Oij est CCW, après la transformation, il est CW
	- cas 3D : passage d'un système de coordonnées right-hand à left-hand ou inversement