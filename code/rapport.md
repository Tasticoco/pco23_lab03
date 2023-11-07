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

Au début, nous nous sommes concentrés sur l'identification des sections critiques
dans le programme, qui concernent évidemment les endroits où l'argent et les stocks
de marchandises sont modifiés. Ces sections doivent donc être protégées.

Initialement, nous avions implémenté plusieurs mutex distincts dans les différentes
fonctions de chaque classe, un pour le trading, un pour le building, etc, mais nous
nous sommes rendu compte que cette approche pouvait conduire à
des accès concurrents sur les mêmes ressources, provoquant potentiellement des
problèmes de création/suppression d'argent ou de matériel. Le problème est que nous
n'avons pris conscience de cela que tardivement, car les accès concurrents sont assez
rares.

En effet, pour qu'un tel problème survienne, il faut qu'un objet crée quelque chose
pendant qu'un autre essaie de faire de trade avec lui. Et vu les nombreuses pauses
simulant les interactions dans notre programme, ce cas de figure se produit très
rarement, et donc il est très complexe pour nous de le voir lorsqu'on quitte le
programme. Il nous est arrivé qu'une seule fois que le programme nous indique
qu'il y avait eu une duplication de l'argent lors de tous nos essais...

Finalement, nous avons utilisé un seul mutex déclaré dans la classe parente Seller,
dont toutes les autres classes héritent. C'est l'endroit le plus approprié pour le
placer.

## Tests effectués

Lorsque l'on quitte le programme, on calcule l'argent et la valeur des ressources de
chaque entité, et on vérifie que ce total est égal à l'argent de départ avec lequel
la simulation a commencé. Ce test permet de s'assurer que l'accès concurrent aux
ressources est bien protégé et qu'il n'y a pas de perte.

Mais comme dit précédemment, ce test est très faillible, car l'accès concurrent 
est très rare dans notre cas de figure. 

On peut également vérifier visuellement pendant l'exécution que chaque usine achète
et produit bien les ressources attendues, et que le programme se comporte
correctement.

## Conclusion 

Le but de ce laboratoire est atteint on le pense mais 