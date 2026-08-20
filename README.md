# 2048qt

Une implémentation du célèbre jeu **2048** en C++ avec le framework **Qt**,
accompagnée d'un **solveur expectimax** capable d'y jouer tout seul — et
d'atteindre la tuile **8192**.

🎥 **Le solveur en action :** https://youtu.be/8Eq9VEvhObY

Le but du jeu est de déplacer et fusionner les tuiles portant des puissances de 2
afin d'atteindre la tuile **2048**.

## Aperçu

Le jeu se joue sur une grille de 4 × 4. À chaque déplacement (haut, bas, gauche,
droite), toutes les tuiles glissent dans la direction choisie ; deux tuiles de
même valeur qui se rencontrent fusionnent en une seule valant leur somme. Une
nouvelle tuile apparaît après chaque coup. La partie est gagnée lorsqu'une tuile
atteint 2048, et perdue lorsque plus aucun déplacement n'est possible.

## Fonctionnalités

- Grille 4 × 4 classique avec apparition aléatoire de nouvelles tuiles
- Fusion des tuiles avec mise à jour du score
- **Solveur expectimax** jouant la partie à votre place (bouton `IA`)
- Affichage redimensionnable, taille de police adaptée automatiquement à la valeur
- Palette de 17 couleurs, jusqu'à la tuile 65536
- Capture d'une image PNG par coup joué, pour montage vidéo (case `Enregistrer (PNG)`)
- Détection des conditions de victoire et de défaite
- Menu : nouvelle partie, quitter

## Le solveur

Le solveur ne contient **aucune règle de stratégie écrite à la main** — pas de
bonus de coin, pas de parcours en serpent. Il ne fait que chercher loin, et
juger les positions avec quelques mesures objectives.

### Recherche

- **Expectimax** : l'arbre alterne des nœuds *max* (nos quatre directions, on
  prend la meilleure) et des nœuds de *hasard* (la tuile qui apparaît, on prend
  l'espérance). On ne parie donc jamais sur la chance.
- **Approfondissement itératif** : la recherche repart à la profondeur 1, puis 2,
  etc. tant qu'il reste du budget (`TEMPS_MAX_MS`). Une profondeur n'est retenue
  que si elle a été explorée **entièrement**, sinon on garde le palier précédent —
  sans quoi les directions seraient comparées sur des bases inégales.
- **Recherche parallèle** : les quatre directions de tête sont explorées
  simultanément, chacune avec sa propre table de transposition, donc sans verrou.
- **Échantillonnage des cases vides** : au plus `MAX_VIDE` cases sont testées par
  nœud de hasard, tirées **au hasard** (Fisher-Yates tronqué) et non dans l'ordre
  de lecture du plateau — sinon le solveur raisonnerait comme si les nouvelles
  tuiles apparaissaient toujours en haut à gauche.

### Représentation et vitesse

- **Bitboard** : le plateau entier tient dans un `uint64_t`, seize cases de
  quatre bits, où l'on stocke l'exposant (`3` pour un 8) plutôt que la valeur.
  Déplacer, fusionner et compter les cases vides devient de la manipulation de
  bits ; aucun appel à `log2()` ou `pow()` à l'exécution.
- **Tables précalculées** : une ligne ne faisant que seize bits, il n'existe que
  65 536 lignes possibles. Le glissement, la monotonie, le lissage et le nombre
  de fusions sont tous tabulés au démarrage. Les déplacements verticaux
  réutilisent les mêmes tables via transposition du plateau.
- **Transposition en O(1)** : échange de blocs en deux étapes plutôt qu'une
  boucle sur les seize cases. La fonction est appelée à chaque déplacement
  vertical et deux fois par évaluation.
- **Table de transposition** : beaucoup de séquences de coups différentes mènent
  au même plateau, le résultat est donc mémorisé par (plateau, profondeur
  restante). Implémentée en tableau plat à adressage direct (hash multiplicatif
  de Fibonacci, écrasement en cas de collision, vidage par un simple `fill`), et
  non en `unordered_map` dont chaque insertion allouait un nœud.

### Évaluation

```
score = maxValeur
      + nbCasesVides   * POID_VIDE
      + monotonie      * POID_MONO
      - lissage        * POID_LISSE
      + fusions        * POID_FUSION
```

La **monotonie** est mesurée globalement par axe, et non ligne par ligne : une
version ligne-par-ligne laissait chaque ligne choisir son propre sens et le
plateau partait en zigzag.

### Réglages

Tout se règle dans `common.h` :

| Constante | Valeur | Rôle |
|-----------|--------|------|
| `MAX_DEPTH` | 10 | Profondeur maximale explorée |
| `MAX_VIDE` | 4 | Cases vides testées par nœud de hasard |
| `CACHE_BITS` | 16 | Taille de la table de transposition (2^n entrées par tranche) |
| `TEMPS_MAX_MS` | 10000 | Budget de temps par coup, en millisecondes |
| `POID_VIDE` | 10 | Poids des cases vides |
| `POID_MONO` | 1 | Poids de la monotonie |
| `POID_LISSE` | 1 | Poids du lissage (soustrait) |
| `POID_FUSION` | 10 | Poids des fusions disponibles |

## Contrôles

| Touche | Action |
|--------|--------|
| ↑ | Déplacer vers le haut |
| ↓ | Déplacer vers le bas |
| ← | Déplacer vers la gauche |
| → | Déplacer vers la droite |

Le bouton **IA** lance ou arrête le solveur. La case **Enregistrer (PNG)** écrit
une image dans `frames/` à chaque coup joué par l'IA ; la vidéo s'assemble
ensuite avec `video/make_video.py`. Le résultat est visible ici :
https://youtu.be/8Eq9VEvhObY

## Prérequis

- **Qt 5** (modules `core`, `gui`, `widgets`)
- Un compilateur C++ supportant **C++11** (g++, clang…)
- `qmake` et `make`

## Compilation

```bash
qmake
make
```

L'exécutable `2048qt` est alors généré à la racine du projet.

## Exécution

```bash
./2048qt
```

## Structure du projet

| Fichier | Rôle |
|---------|------|
| `main.cpp` | Point d'entrée de l'application |
| `CMainWindow` | Fenêtre principale, menus, bouton IA et capture PNG |
| `CWGame` | Widget de jeu : grille, rendu et logique de partie |
| `CGame` | État d'une partie, indépendant de l'affichage |
| `CDeplacement` | Logique des déplacements et fusions (haut / bas / gauche / droite) |
| `CBitBoard` | Plateau en `uint64_t` et tables précalculées, utilisés par le solveur |
| `CSolveur` | Recherche expectimax, dans son propre thread |
| `common.h` | Définitions communes et réglages du solveur |

## Licence

Projet personnel.
