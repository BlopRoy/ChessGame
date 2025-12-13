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

int canWeMove(void);

void pieces(int x,int y,int color);

/* color=0 noir, 1 blanc */
int isSomoneHere(int x,int y,int color);

void resetPossibleMove(void);

void resetAll(void);

// Check if a square (x,y) is a valid move for the last piece that called pieces()
int isValidMove(int x, int y);

#endif