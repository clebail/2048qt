//-----------------------------------------------------------------------------
#ifndef __COMMON_H__
#define __COMMON_H__
//-----------------------------------------------------------------------------
#define COTE                   4
#define CASE                   (COTE*COTE)
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
