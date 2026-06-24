# Script narration — vidéo solveur 2048 (11 min 04 s, rythme variable)

> Voix off pédagogique, public mixte (accessible mais on garde les vrais termes).
> Les `[…]` sont des indications de montage / pauses.
> **Rythme variable** (voir `video/make_video.py`) : ouverture lente pour montrer
> l'enchaînement, ralentis « zoom » avant/après chaque palier (2048/4096/8192),
> ralenti avant la mort, puis gel de la dernière image pour la conclusion.
> Les `★` marquent les **incises de synchro** : elles DOIVENT tomber pile sur
> l'apparition de la tuile, quelle que soit la phrase technique en cours. La
> narration technique (chapitres 0→6) coule en continu ; les incises s'y glissent.

> ⚠️ **Les timecodes ci-dessous sont ceux de la bande enregistrée**, relevés
> dans `audio.aup3`, et non des cibles à atteindre. Le premier jet visait
> 14:45 à 145 mots/min ; la lecture est partie à **180 mots/min**, ce qui
> donne 10:51 de narration et 11:04 avec la queue du lit sonore. C'est la
> **vidéo** qui a été recalée sur la voix, pas l'inverse : les cadences des
> croisières dans `make_video.py` absorbent l'écart, les ralentis restant à
> 3 images/s. Si tu refais une prise, ces timecodes sont à relever à nouveau
> et les cadences à résoudre à nouveau.

---

## Table de synchronisation (bande enregistrée + vidéo calée dessus)

| Palier | Timecode | Frame | Placement narratif |
|-------:|:--------:|:-----:|:-------------------|
| 512    | 0:57     | 253   | (pas d'incise — trop tôt, laisser filer sur l'ouverture lente) |
| 1024   | 1:13     | 480   | (pas d'incise) |
| **2048** | **2:02** | 935 | ★ incise pendant le chap. 1 (Vitesse) |
| **4096** | **4:24** | 1885 | ★ incise en ouverture du chap. 3 (Astuces) |
| **8192** | **8:33** | 3725 | ★ incise en ouverture du chap. 6 (endgame) |
| mort   | 10:39    | 5576  | ralenti avant la mort 10:02 → 10:39, puis gel de 24,6 s |
| 16384  | jamais   | —     | tuile max de la partie = 8192 |

Plan de minutage global :

| Chapitre | Fenêtre |
|:---------|:--------|
| 0. Accroche | 0:00 → 0:39 — sur l'ouverture lente |
| 0 bis. Origine | 0:39 → 1:26 |
| 1. La vitesse | 1:26 → 3:04 — ★ 2048 @ 2:02 |
| 2. La recherche | 3:04 → 4:24 |
| 3. Les astuces | 4:24 → 6:29 — ★ 4096 @ 4:24 (ouverture) |
| 4. Bon plateau | 6:29 → 7:56 |
| 5. Abandonné | 7:56 → 8:33 |
| 6. Endgame à 8192 | 8:33 → 9:54 — ★ 8192 @ 8:33 (ouverture) |
| 7. Conclusion | 9:54 → 10:51 — le plateau vit encore ; freinage à 10:02, mort à 10:39 |
| Respiration finale | 10:51 → 11:04 — sur le gel |

---

## 0. Accroche — 0:00 → 0:39

[L'ouverture est volontairement ralentie : on voit bien l'enchaînement des coups,
les tuiles qui apparaissent et fusionnent une à une.]

Salut tout le monde, bienvenue sur **IA from Scratch** ! Ici on code des IA de
zéro — et aujourd'hui, on en lâche une sur un classique que vous connaissez
sûrement : le 2048.

Ce que vous voyez à l'écran, ce n'est pas un humain qui joue.
C'est un programme. Et il ne joue pas *comme* un humain — il joue *mieux*.

[Laisser tourner quelques coups au ralenti, en silence.]

Pas de stratégie apprise par cœur, pas de règle du genre « garde toujours ta
plus grosse tuile dans un coin ». À la place : une recherche qui explore des
milliers de futurs possibles à chaque coup, guidée par quelques mesures très
simples de ce qu'est un « bon » plateau. Dans le quart d'heure qui vient,
on va voir comment il fonctionne — et jusqu'où il monte.

---

## 0 bis. D'où vient le 2048 — 0:39 → 1:26

Mais d'abord, rendons à César ce qui est à César. Le 2048, je ne l'ai pas
inventé. Il a été créé en **mars 2014** par **Gabriele Cirulli**, un développeur
web italien qui n'avait alors que dix-neuf ans. Il l'a codé en **un seul
week-end**, presque comme un défi personnel — voir s'il était capable de faire
un jeu de zéro. Et il l'a mis en ligne **gratuitement, en open source**.

[Éventuellement à l'écran : « Gabriele Cirulli — 2048 — mars 2014 ».]

Le jeu s'inspirait lui-même de deux autres, *Threes* et un clone appelé *1024*.
En quelques jours, il est devenu un phénomène : des millions de joueurs à travers
le monde, et d'innombrables variantes. Ce que vous voyez ici, c'est ma propre
version, réécrite pour qu'une IA puisse y jouer — mais le jeu, l'idée, l'élégance
de cette mécanique si simple, tout ça revient à lui.

---

## 1. Le vrai problème : la vitesse — 1:26 → 3:04

Pour bien jouer au 2048, il faut anticiper. Pas seulement le coup suivant, mais
la suite : « si je vais à gauche, puis qu'une tuile apparaît ici, puis que je
descends… ». Chaque coup ouvre un éventail de possibilités, et cet éventail
grandit de façon vertigineuse plus on regarde loin.

[Montrer le plateau qui enchaîne des coups rapides.]

Le premier obstacle, ce n'est donc pas l'intelligence — c'est la **vitesse**.
Pour explorer profondément, il faut pouvoir simuler des centaines de milliers
de plateaux par seconde. Une représentation naïve du jeu, avec des objets, des
copies mémoire à chaque coup, ça s'effondre bien avant d'être utile.

> ### ★ INCISE 2048 — tombe à 2:02.1 (frame 935)
> [Marquer une courte pause dans l'explication ; le plateau vient de fusionner 2048.]
>
> Et tenez, justement — pendant que je vous parle de vitesse, regardez l'écran :
> il vient de faire **2048**. La tuile qui donne son nom au jeu, l'objectif que la
> plupart des joueurs n'atteignent jamais… lui l'a sortie en moins de trois minutes,
> sans qu'on s'y attarde. Pour cette IA, ce n'est qu'une étape.
>
> [Reprendre l'explication sans transition appuyée.]

La solution : le plateau tout entier tient dans **un seul nombre de 64 bits**.
Seize cases, quatre bits chacune. On ne stocke pas la valeur d'une tuile, mais
son exposant : un 2 c'est « 1 », un 4 c'est « 2 », un 8 c'est « 3 », et ainsi
de suite. Déplacer les tuiles, fusionner, compter les cases vides — tout
devient de la manipulation de bits, quasi instantanée.

[Optionnel à l'écran : petit schéma « 8192 = 2 puissance 13 ».]

Mieux : comme une ligne ne fait que seize bits, il n'existe qu'un nombre fini
de lignes possibles — soixante-cinq mille et quelques. On peut donc **tout
calculer d'avance**, une fois pour toutes, au démarrage : le résultat d'un
glissement vers la gauche, vers la droite, le nombre de fusions… À l'exécution,
le programme ne calcule plus rien, il *consulte une table*. C'est ça qui rend
la recherche profonde possible.

---

## 2. La recherche : imaginer tous les futurs — 3:04 → 4:24

Maintenant qu'on va vite, on peut explorer. La méthode s'appelle
l'**expectimax**. L'idée est de dérouler un arbre de tous les avenirs possibles.

[Laisser une belle séquence de jeu se dérouler.]

À chaque étage, deux types de décisions alternent. D'abord *nous* : on choisit
une des quatre directions — haut, bas, gauche, droite. Là, on veut le meilleur
coup. Ensuite *le hasard* : le jeu fait apparaître une nouvelle tuile, un 2 ou
un 4, sur une case vide au choix. Et ça, on ne le contrôle pas.

C'est toute la subtilité : contre le hasard, on ne peut pas espérer le meilleur
cas. On calcule la **moyenne** — l'espérance. Le programme se demande : « en
moyenne, sur toutes les tuiles qui pourraient apparaître, à quel point cette
direction me laisse-t-elle en bonne position ? » Puis il choisit la direction
dont la moyenne est la meilleure. Il ne parie pas sur la chance : il joue solide.

Jusqu'où regarder ? Ça dépend du temps disponible. À chaque coup, l'IA s'accorde
un budget de temps fixe. Elle explore d'abord à profondeur un, puis deux,
puis trois, de plus en plus loin, tant qu'il reste du temps. C'est
l'**approfondissement itératif**. Et une règle stricte : une profondeur n'est
retenue que si elle a été calculée *entièrement*. Une exploration à moitié finie
est jetée — sinon on comparerait des directions sur des bases inégales, et le
choix serait faussé.

---

## 3. Les astuces qui font gagner de la profondeur — 4:24 → 6:29

> ### ★ INCISE 4096 — tombe à 4:24.6 (frame 1885)
> [Ouvrir le chapitre exactement quand le 4096 apparaît.]
>
> Et voilà déjà le **4096** — le double du fameux 2048, et on n'est qu'à cinq
> minutes de jeu. Comment monte-t-il aussi vite ? Justement, grâce à une poignée
> d'astuces qui lui font gagner des niveaux entiers de profondeur.

Aller plus profond, c'est jouer mieux. Plusieurs astuces permettent de gratter
des niveaux entiers sans allonger le budget de temps.

[Le plateau continue, tuiles moyennes qui se construisent.]

**Première astuce : la mémoire.** Beaucoup de séquences de coups différentes
aboutissent exactement au même plateau. Gauche-puis-haut, ou haut-puis-gauche,
peuvent donner la même grille. Inutile de recalculer : on garde le résultat en
mémoire, dans une **table de transposition**. La première fois qu'on rencontre
un plateau, on l'évalue ; les fois suivantes, on lit la réponse. À elle seule,
cette table a fait passer la recherche de la profondeur quatre à la profondeur
cinq ou six, à budget de temps égal.

[Beat.]

**Deuxième astuce : la transposition instantanée.** Pour analyser les
déplacements verticaux, on fait pivoter le plateau — on échange lignes et
colonnes — pour réutiliser les mêmes tables que pour l'horizontal. Faire ça case
par case serait lent, et c'est une opération ultra-fréquente. Une manipulation de
bits astucieuse permet de retourner le
plateau entier en une poignée d'instructions. Avant de l'intégrer, elle a été
validée en la comparant à l'ancienne version sur deux cent mille plateaux
aléatoires — pas une seule différence.

[Beat.]

**Troisième astuce : ne pas tout tester.** Quand le hasard place une tuile, il
peut y avoir beaucoup de cases vides, et donc énormément de branches. Plutôt que
d'explorer chacune, l'IA n'en échantillonne que quelques-unes — et surtout, elle
les tire **au hasard**, pas toujours les premières dans l'ordre de lecture.
Sinon elle se mettrait à raisonner comme si les nouvelles tuiles apparaissaient
toujours en haut à gauche, ce qui biaiserait tous ses choix.

[Beat.]

**Et enfin : le parallélisme.** Les quatre directions de départ sont explorées
**en même temps**, sur plusieurs cœurs du processeur. Chacune avec sa propre
mémoire, sans se gêner, sans verrou à partager. Quatre recherches menées de front,
puis on compare les résultats.

---

## 4. Qu'est-ce qu'un « bon » plateau ? — 6:29 → 7:56

Il reste une question de fond. Explorer les futurs, très bien — mais tout au bout
de l'exploration, il faut bien *juger* un plateau. Dire : celui-ci est prometteur,
celui-là est un piège. C'est le rôle de la fonction d'évaluation, et c'est là que
tient la « personnalité » de l'IA.

[Montrer un plateau bien rangé, tuiles en escalier.]

Elle mélange quelques mesures objectives, sans aucune règle dictée à la main.

Les **cases vides**, d'abord : plus il y en a, mieux c'est. Un plateau plein,
c'est un plateau au bord de la mort. L'espace, c'est de la liberté.

La **monotonie** ensuite : les valeurs qui montent ou descendent régulièrement
le long des lignes et des colonnes, comme un escalier. Un plateau ordonné se
fusionne facilement ; un plateau en désordre se bloque. Détail important, cette
monotonie est mesurée **globalement**, sur l'ensemble d'un axe à la fois — pas
ligne par ligne. Autrement, chaque ligne pouvait « tirer » dans son sens, et le
plateau partait en zigzag.

Le **lissage** : on pénalise les grands écarts entre cases voisines. Un 2 collé
à un 2048, ça ne fusionnera jamais — c'est de la place perdue.

Et les **fusions** immédiatement disponibles : on récompense les plateaux où des
tuiles sont prêtes à se combiner.

[Beat.]

Recherche profonde *plus* rangement : ni l'une ni l'autre ne suffit seule. C'est
leur combinaison qui fait la force du solveur.

---

## 5. Ce qu'on a essayé… et abandonné — 7:56 → 8:33

Toutes les idées ne se valent pas, et quelques-unes ont été écartées en cours de
route. La plus tentante : forcer la plus grosse tuile à rester dans un coin, et
récompenser fortement ça.

[Le plateau continue de monter.]

Ça sonne malin — c'est d'ailleurs ce que font beaucoup de joueurs humains. Mais
justement : c'était trop « stratégie humaine », plaquée de force. Et en début de
partie, quand la plus grosse tuile change de place sans arrêt, ça verrouillait
prématurément une direction — l'IA plafonnait à 128. On l'a supprimé. Le principe
est resté le même du début à la fin : donner à l'IA des mesures objectives, et la
laisser trouver sa stratégie toute seule, par la recherche.

---

## 6. L'endgame : tenir à 8192 — 8:33 → 9:54

> ### ★ INCISE 8192 — tombe à 8:33.1 (frame 3725)
> [Ouvrir le chapitre exactement quand le 8192 apparaît ; sert de pont vers l'endgame.]
>
> Et pendant qu'on parlait, il l'a fait : **8192**. La plus grosse tuile qu'il
> atteindra dans cette partie. On entre maintenant dans le territoire où presque
> plus personne ne va — et où tout se joue.

[Ce chapitre court par-dessus toute la fin de partie, plateau dominé par le 8192.]

Un mot d'honnêteté au passage : cette partie, vous la voyez en accéléré. En vrai,
elle a tourné **une quinzaine d'heures** — condensées ici en un quart d'heure. Et
tout ce qui reste maintenant, c'est le plus difficile : ne pas mourir.

Parce qu'à ce stade, le plateau est saturé de grosses valeurs — 8192, 2048, 1024
côte à côte — et il ne reste qu'une ou deux cases libres. La moindre erreur est
fatale : une seule tuile mal placée et tout se fige. Et pourtant elle tient, coup
après coup, parce qu'à chaque instant elle a exploré des milliers de futurs et
choisi celui qui préserve le plus d'options.

[La fin de partie ralentit : les derniers coups avant la mort défilent au ralenti,
plateau presque plein.]

Et c'est là son plus gros avantage sur nous. Un humain, devant un plateau presque
plein de grosses valeurs, avec deux ou trois cases libres à peine, *stresse*. La
peur de l'erreur fatale, la tension du dernier espace qui se referme — et c'est
justement dans ces moments qu'on joue vite, mal, qu'on se trompe. L'IA, elle, ne
ressent rien. Que le plateau soit vide ou au bord de l'asphyxie, elle applique
exactement le même calcul, avec le même sang-froid. Pas de panique, pas de
précipitation — et donc beaucoup moins d'erreurs, précisément quand ça compte le
plus.

[Laisser respirer sur le plateau saturé, jusqu'au coup fatal.]

---

## 7. Conclusion — 9:54 → 10:51

[La dernière image est gelée à l'écran pour toute la conclusion.]

Voilà ce que ça donne quand, au lieu d'apprendre des règles à une machine, on lui
donne les moyens de raisonner par elle-même : une représentation assez rapide pour
explorer des centaines de milliers de plateaux par seconde, une recherche qui
imagine tous les avenirs et joue contre le hasard, et une poignée de mesures
simples pour reconnaître un bon plateau.

Un dernier mot, là encore par honnêteté. Tout le code de ce projet, je l'ai écrit
moi-même. Mais je ne serais pas descendu aussi profond dans la recherche sans un
coup de main : les optimisations les plus pointues — le plateau en soixante-quatre
bits, la transposition instantanée, la table de mémoire — je les dois à
**Claude Code**, l'assistant qui m'a accompagné tout au long. À deux, on est allés
bien plus loin que je ne l'aurais fait seul.

Aucune règle humaine dans la stratégie, donc. Juste de la vitesse, de la
profondeur, et du bon sens mathématique. Merci de l'avoir regardée jouer.

[Respiration finale sur l'image gelée — 10:51 → 11:04 — puis fin.]

---

### Notes de minutage
- Durée finale = **11 min 04 s** (rythme variable, gel final 24,6 s). Narration
  = 1 879 mots en 10 min 51 s, soit **180 mots/min** — et non les 145 visés au
  premier jet, d'où les 3 min 41 s d'écart avec le plan initial de 14:45.
- Les chapitres techniques 0→6 coulent en continu ; les **trois incises ★**
  (2048 @ 2:02, 4096 @ 4:24, 8192 @ 8:33) sont les seuls points de synchro durs.
  Vérifiées au PSNR sur le rendu : l'image bascule à moins d'une frame de
  sortie de la cible sur les trois, ainsi que sur la mort à 10:39.
- Un paragraphe par prise, posées bout à bout dans Audacity. **L'ordre des
  pistes ne suit pas leur nom** (prises refaites, paragraphe rajouté après
  coup) : pour retrouver quel clip porte quoi, se fier au rang dans la
  timeline, pas au libellé `Audio N`.
- Le 512 (0:57) et le 1024 (1:13) tombent pendant l'ouverture lente et
  l'origine : on les laisse filer, ils passent sans commentaire.
- Rythme réglable dans `video/make_video.py` (`SEGMENTS` + `FREEZE_FINALE_S`) ;
  si tu changes le rythme ou refais une prise, ces timecodes sont à recalculer.

### Deux phrases devenues fausses à l'enregistrement
La vidéo dure 11 minutes, mais la bande dit encore « le quart d'heure » à deux
endroits — séquelles du plan initial à 14:45 :
- chap. 0 : « Dans le **quart d'heure** qui vient, on va voir comment il
  fonctionne » ;
- chap. 6 : « elle a tourné une quinzaine d'heures — condensées ici en **un
  quart d'heure** ».

Rien de bloquant, personne ne chronomètre. Mais si tu refais un jour ces deux
prises, dis « dix minutes » — et il faudra alors recaler la vidéo, puisque la
durée des clips concernés changera.
