#ifndef PIECES_H
#define PIECES_H

extern char possibleMove[8][8]; 

/*
    Action sur la case :
    2 capture
    1 normal
    3 bloqué par allié
    0 invalide
    -1 pas dans la liste
*/
int action(int x,int y);

void pieces(int x,int y,int color);

/* color=0 noir, 1 blanc */
int isSomoneHere(int x,int y,int color);

#endif