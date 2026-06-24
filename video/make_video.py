#!/usr/bin/env python3
"""Assemble la video du solveur 2048, calee sur la narration enregistree.

Une frame PNG = un coup joue par l'IA. On fait defiler les frames a des
vitesses differentes selon le moment de la partie (ouverture coup par coup,
ralenti "zoom" autour de 2048/4096/8192, freinage avant la mort), puis on gele
la derniere image pour le dernier paragraphe.

Deux reglages par segment :
  - PAS : 1 = on montre chaque coup, 2 = un coup sur deux, etc. Sert a couvrir
    beaucoup de coups en peu de temps sans transformer l'image en stroboscope.
  - CADENCE : images affichees par seconde (et non coups par seconde : le
    nombre de coups par seconde vaut PAS x CADENCE).

Les cadences ci-dessous sont calculees pour tomber PILE sur les trois incises
de synchro de l'audio (voir script-video.md) :
    2048 -> 2:02.1    4096 -> 4:24.6    8192 -> 8:33.1
et pour que le coup fatal tombe au debut du dernier paragraphe (10:39.1), le
gel couvrant la fin. Total = 11:03.7, soit la duree du projet Audacity.

Ces timecodes sont ceux de la narration REELLE, relevee dans audio.aup3, et
non ceux du plan de montage de script-video.md (qui visait 14:45 a 145 mots
par minute ; la lecture est partie a 180). Si tu refais une prise, les quatre
reperes ci-dessus sont a relire dans le projet Audacity et les cadences des
croisieres a resoudre a nouveau : ce sont elles qui absorbent l'ecart, les
ralentis restant a 3 img/s.

Sortie : video/2048-solveur.mp4 (30 fps constant, lisible partout).
"""

import os
import subprocess
import sys

# --- Reglages -------------------------------------------------------------

FRAMES_DIR = os.path.join(os.path.dirname(__file__), "..", "frames")
FRAME_FMT = "frame_{:06d}.png"
N_FRAMES = 5577  # frame_000000 .. frame_005576

# Chaque segment : (frame_debut inclus, frame_fin EXCLU, pas, cadence).
# Les bornes doivent couvrir 0 .. N_FRAMES-1 sans trou. Paliers reels :
#   2048 @ frame 935, 4096 @ 1885, 8192 @ 3725, mort @ 5576.
# Les segments a cadence 3.0 sont les ralentis : ils encadrent chaque palier
# (65 frames avant, 65 apres) pour qu'on voie la fusion arriver et retomber.
SEGMENTS = [
    (0,    150,  1,  3.000),   # ouverture : coup par coup, on suit tout
    (150,  870,  2,  7.140),   # croisiere 1
    (870,  1000, 1,  3.000),   # ralenti "zoom" 2048
    (1000, 1820, 1,  8.266),   # croisiere 2
    (1820, 1950, 1,  3.000),   # ralenti "zoom" 4096
    (1950, 3660, 1,  8.337),   # croisiere 3
    (3660, 3790, 1,  3.000),   # ralenti "zoom" 8192
    (3790, 5430, 2, 12.085),   # croisiere endgame (fonce vers la mort)
    (5430, 5576, 1,  4.000),   # freinage avant la mort
    (5576, 5577, 1, 30.000),   # derniere image (le gel prend le relais)
]

FREEZE_FINALE_S = 24.63  # gel de la derniere image, sur le dernier paragraphe
OUTPUT_FPS = 30          # fps constant de sortie
CRF = 18                 # qualite libx264 (plus bas = mieux)

# Reperes de l'audio, uniquement pour le controle affiche au lancement :
# (frame, timecode vise en secondes, libelle).
ANCRES = [
    (935,  122.09, "2048"),
    (1885, 264.63, "4096"),
    (3725, 513.08, "8192"),
    (5576, 639.10, "mort"),
]
DUREE_AUDIO_S = 663.74

OUT_DIR = os.path.dirname(os.path.abspath(__file__))
LIST_PATH = os.path.join(OUT_DIR, "frames_concat.txt")
OUT_PATH = os.path.join(OUT_DIR, "2048-solveur.mp4")

# --- Construction de la liste ---------------------------------------------


def frames_affichees():
    """Rend (indice de frame, duree d'affichage) dans l'ordre de la video."""
    couvert = 0
    for debut, fin, pas, fps in SEGMENTS:
        if debut != couvert:
            raise ValueError(f"trou ou recouvrement entre {couvert} et {debut}")
        couvert = fin
        for i in range(debut, fin, pas):
            yield i, 1.0 / fps
    if couvert != N_FRAMES:
        raise ValueError(f"les segments s'arretent a {couvert}, attendu {N_FRAMES}")


def build_list():
    frames_abs = os.path.abspath(FRAMES_DIR)
    total = 0.0
    dates = {}
    lines = ["ffconcat version 1.0"]
    for i, dur in frames_affichees():
        dates[i] = total
        total += dur
        lines.append(f"file '{os.path.join(frames_abs, FRAME_FMT.format(i))}'")
        lines.append(f"duration {dur:.5f}")
    # Le demuxer concat ignore la duree de la derniere image si elle n'est
    # pas re-listee : on la repete pour ne pas perdre la derniere frame.
    # Le gel final n'est PAS gere ici (double comptage) mais via tpad a
    # l'encodage.
    lines.append(f"file '{os.path.join(frames_abs, FRAME_FMT.format(N_FRAMES - 1))}'")
    with open(LIST_PATH, "w") as f:
        f.write("\n".join(lines) + "\n")
    return total + FREEZE_FINALE_S, dates


def mmss(s):
    return f"{int(s) // 60}:{s - 60 * (int(s) // 60):04.1f}"


def main():
    total, dates = build_list()
    print(f"Liste concat : {LIST_PATH}")
    for frame, cible, nom in ANCRES:
        # Une frame sautee par le pas s'affiche avec la premiere frame gardee
        # qui la suit : on cherche la date de celle-la.
        reel = next(dates[i] for i in sorted(dates) if i >= frame)
        ecart = reel - cible
        print(f"  {nom:>4} : {mmss(reel)}  (audio {mmss(cible)}, ecart {ecart:+.2f}s)")
    print(f"Duree totale : {mmss(total)} ({total:.1f} s) "
          f"pour {mmss(DUREE_AUDIO_S)} d'audio "
          f"(ecart {total - DUREE_AUDIO_S:+.2f}s)")

    cmd = [
        "ffmpeg", "-y",
        "-f", "concat", "-safe", "0", "-i", LIST_PATH,
        "-vf", (f"pad=ceil(iw/2)*2:ceil(ih/2)*2,fps={OUTPUT_FPS},"
                f"tpad=stop_mode=clone:stop_duration={FREEZE_FINALE_S},"
                f"format=yuv420p"),
        "-c:v", "libx264", "-crf", str(CRF), "-preset", "medium",
        "-movflags", "+faststart",
        OUT_PATH,
    ]
    if "--dry-run" in sys.argv:
        print("Commande ffmpeg :\n  " + " ".join(cmd))
        return
    print("Encodage...")
    subprocess.run(cmd, check=True)
    print(f"OK -> {OUT_PATH}")


if __name__ == "__main__":
    main()
