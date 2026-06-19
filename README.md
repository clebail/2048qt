# 2048qt

Une implémentation du célèbre jeu **2048** en C++ avec le framework **Qt**.

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
- Affichage redimensionnable, taille de police adaptée automatiquement à la valeur
- Détection des conditions de victoire et de défaite
- Menu : nouvelle partie, quitter

## Contrôles

| Touche | Action |
|--------|--------|
| ↑ | Déplacer vers le haut |
| ↓ | Déplacer vers le bas |
| ← | Déplacer vers la gauche |
| → | Déplacer vers la droite |

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
| `CMainWindow` | Fenêtre principale, menus et gestion des touches |
| `CWGame` | Widget de jeu : grille, rendu et logique de partie |
| `CDeplacement` | Logique des déplacements et fusions (haut / bas / gauche / droite) |
| `common.h` | Définitions communes (taille de la grille, structure d'une case) |

## Licence

Projet personnel.
