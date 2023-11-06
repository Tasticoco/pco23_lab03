# Gestion d'accès concurrents

Auteurs: Arthur Junod et Edwin Haeffner

## Description des fonctionnalités du logiciel

Ce logiciel simule une logistique entre des :

- Extracteurs de ressources
- Grossistes
- Usines

Et le but de ce laboratoire est de bien identifier les sections critiques du 
programme et les protéger avec des mutex. 


## Choix d'implémentation

Au début, nous nous sommes occupés de voir où seraient les sections critiques du 
programme et le constat est assez simple, tous les endroits où on édite l'argent 
ou bien le stock de marchandise doivent etre protégé.

Nous avons aussi fait plusieurs mutex différent par fonction, car dans le 
programme, chaque thread s'occupe d'un seul objet. Donc il ne fait pas sens



## Tests effectués

Quand on quitte le programme, on calcule l'argent et le cout des ressources de 
chaque entitées et on vérifie si le calcul est égal à l'argent avec lequel on a 
commencé la simulation. Et en faisant cela, c'est un test qui vérifie si l'accès 
concurant aux ressources est bien protégé et qu'il n'y a pas de perte. 

Sinon, on peut vérifier visuellement que chaque usine achète et produit bien des 
ressources et que le programme fonctionne bien comme il le faut.