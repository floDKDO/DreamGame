\# Documentation



\## atan2



\### Explications



<p align="center">

&#x20; <img src="https://github.com/floDKDO/DreamGame/blob/main/doc/atan2.png" alt="atan2" width="500"/>

</p>



\### Différences entre atan et atan2



<p align="center">

&#x20; <img src="https://github.com/floDKDO/DreamGame/blob/main/doc/atan\_vs\_atan2.png" alt="atan Vs atan2" width="500"/>

</p>





\## Angles d'Euler (yaw + pitch)

La formule pour calculer le front\_vector d'une caméra avec les angles d'Euler (yaw et pitch) est la suivante :

```

glm::vec3 front\_vector = glm::vec3(

&#x09;cos(glm::radians(euler\_angles\_.yaw\_)) \* cos(glm::radians(euler\_angles\_.pitch\_)), //x

&#x09;sin(glm::radians(euler\_angles\_.pitch\_)),					 //y

&#x09;sin(glm::radians(euler\_angles\_.yaw\_)) \* cos(glm::radians(euler\_angles\_.pitch\_))  //z

);
```



\### Schémas



<p align="center">

&#x20; <img src="https://github.com/floDKDO/DreamGame/blob/main/doc/yaw.png" alt="Yaw" width="400"/>

&#x20; <img src="https://github.com/floDKDO/DreamGame/blob/main/doc/pitch.png" alt="Pitch" width="400"/>

</p>





\### Explications

&#x20;

\- Yaw : selon le schéma, on a :

&#x09;- pour le composant x : cos(yaw) = longueur du côté adjacent à l'angle yaw = composant x

&#x09;- pour le composant z : sin(yaw) = longueur du côté opposé à l'angle yaw = composant z



\- Pitch : selon le schéma, on a :

&#x09;- pour le composant y : sin(pitch) = longueur du côté opposé à l'angle pitch = composant y

&#x09;- pour les composants x et z (plan x/z) : cos(pitch) = longueur du côté adjacent à l'angle pitch = composants x et z



\- Pourquoi on multiplie pour les composants x et z / pourquoi on utilise le plan x/z pour le schéma du pitch à la place d'uniquement le composant z ?

&#x09;- Si on ne multiplie pas le composant x (c'est-à-dire avoir uniquement cos(yaw)), on aurait le problème suivant :

&#x09;	- soit pitch = 90 degrés et yaw = 0 degré (la caméra regarde vers le haut donc vec3(0, 1, 0)), on aurait :

&#x09;		- le composant x qui vaut cos(0) = 1 (=> devrait valoir 0 car cos(0) \* cos(90) = 1 \* 0 = 0)

&#x09;		- le composant y qui vaut sin(90) = 1

&#x09;		- le composant z qui vaut sin(0) \* cos(90) = 0

&#x09;	=> vec3(1, 1, 0) qui est faux (la caméra regardrait vers la droite alors que notre yaw était à 0 degré)

&#x09;- Si on ne multiplie pas le composant z (c'est-à-dire avoir uniquement sin(yaw)), on aurait le problème suivant :

&#x09;	- soit pitch = 0 degré et yaw = 90 degrés (la caméra regarde vers l'avant donc vec3(0, 0, 0)), on aurait :

&#x09;		- le composant x qui vaut cos(90) \* cos(0) = 0

&#x09;		- le composant y qui vaut sin(0) = 0

&#x09;		- le composant z qui vaut sin(90) = 1 (=> devrait valoir 0)

&#x09;	=> vec3(0, 0, 1) qui est faux

&#x20; => pour conclure, la valeur de yaw et pitch agissent l'une sur l'autre



\- Pourquoi mettre -90.0f en intialisation à l'angle yaw ?

&#x09;- Si on met 0 (pitch = 0, yaw = 0), la caméra pointe vers les valeurs positives sur l'axe x (= regarde vers la droite) car :

&#x09;	- composant x : cos(0) \* cos(0) = 1

&#x09;	- composant y : sin(0) = 0

&#x09;	- composant z : sin(0) \* cos(0) = 0

&#x20;	 => vec3(1, 0, 0) pointe bien sur les valeurs positives de x.

&#x09;- Si on veut que la caméra pointe vers les valeurs négatives sur l'axe z (= regarde vers l'avant), il faut donner la valeur -90.0f à l'angle yaw (pitch = 0, yaw = -90.0f) car :

&#x09;	- composant x : cos(-90) \* cos(0) = 0

&#x09;	- composant y : sin(0) = 0

&#x09;	- composant z : sin(-90) \* cos(0) = -1

&#x20;	 => vec3(0, 0, -1) pointe bien sur les valeurs négatives de z.





\## Eclairage de Phong



\### Soustraction entre deux points donne un vecteur



<p align="center">

&#x20; <img src="https://github.com/floDKDO/DreamGame/blob/main/doc/vecteur\_soustraction.png" alt="Soustraction vecteur" width="500"/>

</p>



\### Calculer "diffuse"

Diffuse dépend de la couleur et de l'intensité de la source de lumière, de la distance entre la source de lumière et le fragment, et de l'angle avec lequel la lumière émise par la source de lumière atteint le fragment (angle entre lumière et normal du fragment).

Les étapes pour calculer "diffuse" sont les suivantes :

1\) Obtenir la position du fragment en world coordinates. On utilise les world coordinates car tous les vecteurs/positions utilisés dans les étapes suivantes sont en world coordinates.

2\) Calculer le vecteur de direction du fragment vers la source de lumière en réalisant une soustraction entre ces deux positions (voir le schéma ci-dessus)

3\) Calculer le produit scalaire entre la normale du fragment (= attribut) et le vecteur de direction de la source de lumière : on obtient un scalaire appartenant à \[-1 ; 1] (en réalité, on ignore les valeurs négatives donc on a l'intervalle \[0 ; 1]).



Explications pour 3) : comme l'impact de la lumière est plus important si l'angle entre elle est le fragment est petit (<=> le vecteur de la lumière est (ou est proche d'être) parallèle à la normale du fragment <=> on met une lampe torche directement au-dessus de la surface => lumière concentrée), la valeur de diffuse doit être plus grande que lorsque l'angle est grand (<=> le vecteur de la lumière est (ou est proche d'être) perpendiculaire à la normale du fragment <=> on met une lampe torche qui pointe "penché" sur la surface => lumière "éparpillée"). Cela est parfaitement représenté par le produit scalaire réalisé en 3) où le scalaire retourné est plus grand si l'angle entre les deux est petit (proche de la parallèle, valeur proche de 1) et inversement (proche de la perpendiculaire, valeur proche de 0).



\### Calculer "specular"

En plus des dépendances de diffuse, specular dépend de la position de la caméra, du vecteur de reflet et de l'angle entre ces deux vecteurs.

Les étapes pour calculer "specular" sont les suivantes :

1\) Calculer le vecteur de direction du fragment vers la caméra

2\) Calculer le vecteur de reflet obtenu après que le vecteur "source de lumière vers fragment" ait reflété sur le fragment (= vecteur "fragment vers caméra").

Pour l'obtenir, on utilise la fonction `reflect()` de GLSL. Celle-ci prend en paramètre un "incident vector" (= vecteur "source de lumière vers fragment") et la normale du fragment et retourne le vecteur "de reflet" (<=> reflet de incident vector sur le fragment).

3\) Calculer le produit scalaire entre le vecteur de reflet et le vecteur "fragment vers caméra". Comme avec diffuse, on ignore les valeurs négatives (scalaire appartient à \[0 ; 1]) : plus le scalaire est proche de 1, plus l'angle entre le vecteur de reflet et le vecteur "fragment vers caméra" est petit (= tâche de lumière plus visible car la lumière nous reflète directement dans les yeux) et inversement. On a donc que plus la caméra (= notre vue) est en face de la lumière reflétée sur le fragment, plus on voit la tâche de lumière sur le fragment.

4\) Choisir une valeur (puissance de deux) et élever le scalaire obtenu à l'étape 3) à cette puissance.

Plus la valeur de la puissance est grande, plus la tâche de lumière sera concentrée. Moins la valeur de la puissance est grande, plus la tâche de lumière sera éparpillée.

A noter que comme la valeur du scalaire est inférieure ou égale à 1, augmenter la valeur de la puissance va réduire la valeur du scalaire, et donc réduire la taille de la tâche de lumière.

