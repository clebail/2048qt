//-----------------------------------------------------------------------------
#ifndef __COMMON_H__
#define __COMMON_H__
//-----------------------------------------------------------------------------
#define COTE                   4
#define CASE                   (COTE*COTE)
#define MAX_DEPTH              10
#define MAX_VIDE               4
// Nb de bits d'index de la table de transposition (2^CACHE_BITS entrées par
// tranche de profondeur restante, par direction, par type max/chance).
#define CACHE_BITS             16
#define POID_VIDE              10
#define POID_MONO              1
#define POID_LISSE             1
#define POID_FUSION            10
#define TEMPS_MAX_MS           10000
//-----------------------------------------------------------------------------
typedef struct _SCase {
    bool nouveau;
    bool fusion;
    int valeur;
}SCase;
//-----------------------------------------------------------------------------
typedef SCase TCases[CASE];
//-----------------------------------------------------------------------------
#endif // __COMMON_H__
//-----------------------------------------------------------------------------
