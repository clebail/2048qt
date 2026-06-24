# Suivi du solveur IA (branche `ia`)

Notes de travail sur l'évolution du solveur `CSolveur`, pour reprendre le fil
après une pause de test. Objectif affiché dès le départ : **ne pas recréer un
style de jeu humain, mais une IA qui score plus haut qu'un humain** (recherche
profonde + heuristiques objectives, pas de règles "à la main" type coin/serpent).

## Architecture actuelle

- **`CBitBoard`** : représentation du plateau en `uint64_t` (16 cases × 4 bits,
  valeur = exposant log2, 0 = case vide). Remplace les copies de `CGame`/
  `CDeplacement` (structs + virtuel), bien trop lentes pour une recherche
  profonde.
- **Tables précalculées** (65536 entrées, une par ligne 16 bits possible) :
  résultat d'un glissement gauche/droite, monotonie gauche/droite, lissage,
  nombre de fusions. Les déplacements verticaux réutilisent les mêmes tables
  via transposition du plateau. Tout est calcul entier (exposants déjà
  stockés), pas de `log2()`/`pow()` à l'exécution.
- **Recherche expectimax** avec **iterative deepening** (`solve()`) : profondeur
  1, 2, 3... tant qu'il reste du temps (`TEMPS_MAX_MS`, actuellement 1000 ms).
  Une profondeur n'est retenue que si elle a fini de se calculer entièrement,
  sinon jetée (évite de comparer des directions sur des bases inégales).
- **Table de transposition** : beaucoup de séquences de coups différentes
  mènent au même plateau. Résultat mémorisé par `(plateau, profondeur
  restante)`. A permis de passer de profondeur 4 à profondeur 5/6 en ~1s.
  Implémentée en tableau plat à adressage direct (hash multiplicatif type
  Fibonacci, écrasement en cas de collision, pas de chaînage), et non plus en
  `unordered_map` : l'ancienne version allouait un nœud par insertion et
  libérait chaque nœud au vidage entre deux appels à `solve()`, coûteux à
  chaque coup. Le tableau plat se vide en un `std::fill` et n'alloue qu'une
  fois (à la construction de `CSolveur`). Taille par tranche de profondeur
  réglable via `CACHE_BITS` dans `common.h` (actuellement 2^16 entrées).
- **`transpose()` en O(1)** (`CBitBoard.cpp`) : remplace la boucle sur les 16
  nibbles par un échange de blocs en 2 étapes (12 puis 24 bits), technique
  reprise des IA de référence (nneonneo). Fonction très chaude (appelée à
  chaque déplacement vertical et deux fois par `analyse()`). Validée par
  comparaison exhaustive avec l'ancienne implémentation sur 200000 plateaux
  aléatoires avant intégration.
- **Échantillonnage des cases vides** (`chanceNode`) : pour limiter le
  branchement, seules `MAX_VIDE` cases vides sont testées par nœud de hasard
  (pas toutes). Elles sont désormais tirées **aléatoirement** (Fisher-Yates
  tronqué, générateur `thread_local`) plutôt que les premières dans l'ordre du
  plateau, pour ne pas biaiser systématiquement vers le coin haut-gauche.
- **Recherche parallèle** : les 4 directions de tête (haut/droite/bas/gauche)
  sont recherchées en parallèle via `std::async`, chacune avec **son propre
  cache de transposition** (pas de verrou, pas de cache partagé entre
  threads — le partage inter-direction existait mais était moins fréquent que
  le partage intra-direction).

## Heuristique d'évaluation (`evaluate()`)

Alignée sur les IA de référence publiques (nneonneo, macroxue) : recherche
profonde **+** heuristiques de rangement, l'une ne suffit pas sans l'autre.

```
score = maxValeur
      + nbCasesVides   * POID_VIDE
      + monotonie      * POID_MONO
      - lissage        * POID_LISSE
      + fusions        * POID_FUSION
```

- **Monotonie** : calculée **globalement par axe** (somme sur toutes les
  lignes/colonnes avant de choisir la meilleure direction une seule fois),
  pas ligne par ligne — une version ligne-par-ligne provoquait un effet
  "zigzag" (chaque ligne pouvait choisir une direction différente).
- **Lissage** : pénalise les écarts de valeur entre cases voisines (complique
  les fusions futures).
- **Fusions** : nombre de fusions immédiatement disponibles.

Réglages actuels dans `common.h` :
```
MAX_DEPTH    = 10
MAX_VIDE     = 4
POID_VIDE    = 10
POID_MONO    = 1
POID_LISSE   = 1
POID_FUSION  = 10
TEMPS_MAX_MS = 1000
```

## Autres fonctionnalités ajoutées

- **Palette de couleurs** (`CWGame.cpp`) étendue de 12 à 17 couleurs (jusqu'à
  65536) avec un indexage qui ne peut plus retomber sur le gris des cases
  vides (bug : `4096 % 12 == 0` retombait sur la couleur de fond).
- **Capture d'écran pour vidéo** : case à cocher "Enregistrer (PNG)" dans la
  fenêtre principale. Une frame PNG est sauvegardée dans `frames/` à **chaque
  coup joué par l'IA** (pas à chaque frame d'animation — testé un temps à
  24 fps, beaucoup trop volumineux pour une partie d'1h). Assemblage vidéo :
  `ffmpeg -framerate <fps> -i frames/frame_%06d.png video.mp4`.

## Pistes explorées puis abandonnées

- Bonus de coin (grosse valeur dans un coin) + renforcement de la monotonie
  couplé à la position de la plus grosse valeur : jugé trop "stratégie
  humaine", et une version basée sur la position instantanée du max plafonnait
  à 128 (position quasi aléatoire en début de partie → verrouillage prématuré
  d'une direction). Abandonné via `git checkout`.
- Augmenter le budget temps au lieu d'optimiser : explicitement écarté, le
  vrai problème était la représentation du plateau (structs + virtuel), pas
  le budget.

## Pistes non implémentées (idées pour la suite)

- **Élagage façon alpha-bêta** sur les nœuds max pour gagner en profondeur à
  budget temps constant.
- **`MAX_VIDE` adaptatif** : le réduire ferait gagner en profondeur mais
  dégraderait la précision de l'espérance (déjà partiellement compensé par le
  tirage aléatoire des cases testées).
- Point relevé mais non traité : dans `CDeplacement::deplacement()`, le flag
  anti-cascade de fusion (`fusion`) n'est positionné que si `anim=true`. Or
  tous les coups pilotés par l'IA (`CMainWindow::onCoupChoisi`) appellent avec
  `anim=false` — cette protection ne s'active donc jamais pour les coups de
  l'IA. À vérifier si ça peut provoquer des fusions en cascade incorrectes.

## Contraintes de collaboration

- L'utilisateur build lui-même le projet (`qmake`/`make`) — ne pas lancer ces
  commandes à sa place.
- Pas de changement de `.pro` récent (dernier ajout : `CBitBoard.cpp`/`.h`) ;
  les derniers changements (parallélisation, checkbox PNG) ne nécessitent
  qu'un `make` (le `.ui` est déjà listé dans `FORMS`).
