## Projet technologique

Le jeu a pour but de placer le bon nombre de bouches d'incendie afin de sauver les maisons des écureuils. Le jeu a été codé en C et comprend des tests. Il existe également une version mobile. Il suffit de cliquer sur la case pour rajouter  l'élément, le supprimer ou mettre une marque. 

## Fonctionnalités

- Solveur : permet de résoudre la partie.
- Enregistrement : permet d'enregistrer la partie au format txt dans le dossier "build".
- Undo/redo : permet d'annuler ou de rétablir le coup précédent.

## Utilisation

1. Créez le Makefile :
   ```bash
   mkdir build; cd build ; cmake ..
   ```
2. Exécutez le jeu :
   ```bash
   make 
   ```
3. Lancer le jeu :
    ```bash
    ./game_sdl
    ```

## Images 

Début de la partie :
<br>
<img src="readme/game_start.png" width="300px">
<br>
Fin de la partie :
<br>
<img src="readme/game_end.png" width="300px"> 


## Auteurs

- Théo
- Valtenine
- Abdou 
- Gabriel