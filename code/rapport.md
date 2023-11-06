**IMPORTANT ! Ce chablon est donné à titre d'exemple ! Vous êtes entièrement libres de rajouter des sections ou des informations qui vous semblent pertinentes dans le rapport. N'oubliez pas de rendre seulement un PDF nommé rapport.pdf.**


# Gestion d'accès concurrents

Auteurs: Arthur Junod et Edwin Haeffner

## Description des fonctionnalités du logiciel

Ce logiciel simule une logistique entre des :

- Extracteurs de ressources
- Grossistes
- Usines

Et le but de ce laboratoire est de bien identifier les sections critiques du 
programme et les protégerer avec des mutex. 


## Choix d'implémentation

Au début, nous nous sommes occupés 



## Tests effectués

Quand on quitte le programme, on calcule l'argent  et le cout des ressources de 
chaque entitées et on vérifie si le calcul est égal à l'argent avec lequel on a 
commencé la simulation. Et en faisant cela, c'est un test qui vérifie si l'accès 
concurant aux ressources est bien protégé et qu'il n'y a pas de perte. 

Sinon, on peut vérifier visuellement que chaque usine achète et produit bien des 
ressources, que 