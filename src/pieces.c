#include "pieces.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"

#define INVALID -1

int tempLock[2] = {0, 0}; // (x,y)

typedef struct {
    int x;
    int y;
    int occupied;
    int valid;
} Coordinate;

Coordinate tempValidMove[52];

char possibleMove[8][8] = {
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '}
};

void error(void) {
    printf("error");
}
//return 1 if the piece chose is blocked 0 if she can move
int canWeMove(void) {
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (possibleMove[i][j] != ' ')
            {
                return 0;
            }
            
        }
        
    }
    return 1;
}

void resetPossibleMove(void) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            possibleMove[y][x] = ' ';
        }
    }
}

void markPossibleMoves(void) {
    resetPossibleMove();

    for (int i = 0; i < 52; i++) {
        if (tempValidMove[i].x != INVALID && tempValidMove[i].y != INVALID) {

            int x = tempValidMove[i].x;
            int y = tempValidMove[i].y;

            if (tempValidMove[i].valid) {
                if (tempValidMove[i].occupied)
                    possibleMove[y][x] = '%'; // capture
                else
                    possibleMove[y][x] = '*'; // move
            }
        }
    }
}

/*
    Vérifie la présence d’une pièce pour les Noirs
    Renvoie :
        0 = vide
        1 = allié
        2 = ennemi
*/
int isSomoneHereBlack(int x,int y) {
    if (board[y][x] != ' ') {
        if (islower(board[y][x])) {
            return 1;
        }
        return 2;
    }
    return 0;
}

/*
    Vérifie présence pour les Blancs
*/
int isSomoneHereWhite(int x,int y) {
    if (board[y][x] != ' ') {
        if (isupper(board[y][x])) {
            return 1;
        }
        return 2;
    }
    return 0;
}

/* color=0 noir, 1 blanc */
int isSomoneHere(int x,int y,int color) {
    if (color == 0) {
        return isSomoneHereBlack(x,y);
    }
    return isSomoneHereWhite(x,y);
}

int nobody(int x,int y,int color) {
    if (board[y][x] != ' ') {
        return isSomoneHere(x,y,color);
    }
    return 0;
}
/* Vide une position dans tempValidMove */
void resetPosition(int position) {
    tempValidMove[position].x = INVALID;
    tempValidMove[position].y = INVALID;
    tempValidMove[position].occupied = INVALID;
    tempValidMove[position].valid = INVALID;
}

/* Réinitialisation complète */
void resetAll(void) {
    for (int i = 0; i < 52; i++) {
        resetPosition(i);
    }
}

// Check if a square (x,y) is a valid move for the last piece that called pieces()
int isValidMove(int x, int y) {
    for (int i = 0; i < 52; i++) {
        if (tempValidMove[i].x == x && tempValidMove[i].y == y && 
            tempValidMove[i].valid) {
            return 1;
        }
    }
    return 0;
}

/*
    Enregistre un mouvement dans tempValidMove
    position = index
    color    = camp (0 noir, 1 blanc)

    Retour :
        1 si une pièce bloque la suite des déplacements (tour/fou/reine)
        0 sinon
*/
int legalMove(int position, int color) {
    tempValidMove[position].x = tempLock[0];
    tempValidMove[position].y = tempLock[1];

    int status = isSomoneHere(tempLock[0], tempLock[1], color);

    switch(status) {
        case 0: // vide
            tempValidMove[position].occupied = 0;
            tempValidMove[position].valid = 1;
            break;

        case 1: // allié => mouvement invalide
            tempValidMove[position].occupied = 1;
            tempValidMove[position].valid = 0;
            return 1;

        case 2: // ennemi => capture
            tempValidMove[position].occupied = 1;
            tempValidMove[position].valid = 1;
            return 1;
    }

    return 0;
}

/* ---- MOUVEMENTS ----
   x = colonne
   y = ligne
   Les lignes augmentent vers le BAS
*/

/* Vers le haut */
void forward(int x,int y,int s) {
    tempLock[0]=x;
    tempLock[1]=y-s;
}
/* Vers le bas */
void backward(int x,int y,int s) {
    tempLock[0]=x;
    tempLock[1]=y+s;
}
/* Gauche */
void left(int x,int y,int s) {
    tempLock[0]=x-s;
    tempLock[1]=y;
}
/* Droite */
void right(int x,int y,int s) {
    tempLock[0]=x+s;
    tempLock[1]=y;
}

void diagonalLeftTop(int x,int y,int s) {
    tempLock[0]=x-s;
    tempLock[1]=y-s;
}
void diagonalRightTop(int x,int y,int s) {
    tempLock[0]=x+s;
    tempLock[1]=y-s;
}
void diagonalRightBottom(int x,int y,int s) {
    tempLock[0]=x+s;
    tempLock[1]=y+s;
}
void diagonalLeftBottom(int x,int y,int s) {
    tempLock[0]=x-s;
    tempLock[1]=y+s;
}

/* ----- PIECES ----- */

void blackPawn(int x,int y,int color) {
    backward(x,y,1);
    if (isSomoneHereBlack(tempLock[0],tempLock[1]) == 0) { 
        legalMove(0,color);
    }

    diagonalLeftBottom(x,y,1);
    if (isSomoneHereBlack(tempLock[0],tempLock[1]) == 2) {
        legalMove(1,color);
    }

    diagonalRightBottom(x,y,1);
    if (isSomoneHereBlack(tempLock[0],tempLock[1]) == 2) {
        legalMove(2,color);
    }
}

void whitePawn(int x,int y,int color) {
    forward(x,y,1);
    if (isSomoneHereWhite(tempLock[0],tempLock[1]) == 0) {
        legalMove(0,color);
    }

    diagonalLeftTop(x,y,1);
    if (isSomoneHereWhite(tempLock[0],tempLock[1]) == 2) {
        legalMove(1,color);
    }

    diagonalRightTop(x,y,1);
    if (isSomoneHereWhite(tempLock[0],tempLock[1]) == 2) {
        legalMove(2,color);
    }
}

void blackAndWhiteKnight(int x,int y,int color){
    int pos=0;
    forward(x,y,2);
    left(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    forward(x,y,2);
    right(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    backward(x,y,2);
    left(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    backward(x,y,2);
    right(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    right(x,y,2);
    forward(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    right(x,y,2);
    backward(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    left(x,y,2);
    forward(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);

    left(x,y,2);
    backward(tempLock[0],tempLock[1],1);
    legalMove(pos++,color);
}

void blackAndWhiteBishop(int x,int y,int color){
    int pos=0;
    int stop[4]={0};
    for(int i=1;i<=7;i++){
        if(!stop[0]) {
            diagonalLeftTop(x,y,i); 
            stop[0]=legalMove(pos++,color); 
        }
        if(!stop[1]) { 
            diagonalRightTop(x,y,i); 
            stop[1]=legalMove(pos++,color); 
        }
        if(!stop[2]) { 
            diagonalRightBottom(x,y,i); 
            stop[2]=legalMove(pos++,color); 
        }
        if(!stop[3]) { 
            diagonalLeftBottom(x,y,i); 
            stop[3]=legalMove(pos++,color); 
        }
    }
}

void blackAndWhiteRook(int x,int y,int color){
    int pos=0;
    int stop[4]={0};
    for(int i=1;i<=7;i++){
        if(!stop[0]) { 
            forward(x,y,i); 
            stop[0]=legalMove(pos++,color); 
        }
        if(!stop[1]) { 
            backward(x,y,i); 
            stop[1]=legalMove(pos++,color); 
        }
        if(!stop[2]) { 
            right(x,y,i); 
            stop[2]=legalMove(pos++,color); 
        }
        if(!stop[3]) { 
            left(x,y,i); 
            stop[3]=legalMove(pos++,color); 
        }
    }
}

void blackAndWhiteQueen(int x,int y,int color){
    int pos=0;
    int stop[8]={0};
    for(int i=1;i<=7;i++){
        if(!stop[0]) { 
            diagonalLeftTop(x,y,i); 
            stop[0]=legalMove(pos++,color); 
        }
        if(!stop[1]) { 
            diagonalRightTop(x,y,i); 
            stop[1]=legalMove(pos++,color); 
        }
        if(!stop[2]) { 
            diagonalRightBottom(x,y,i); 
            stop[2]=legalMove(pos++,color); 
        }
        if(!stop[3]) { 
            diagonalLeftBottom(x,y,i); 
            stop[3]=legalMove(pos++,color); 
        }
        if(!stop[4]) { 
            forward(x,y,i); 
            stop[4]=legalMove(pos++,color); 
        }
        if(!stop[5]) { 
            backward(x,y,i); 
            stop[5]=legalMove(pos++,color); 
        }
        if(!stop[6]) { 
            right(x,y,i); 
            stop[6]=legalMove(pos++,color); 
        }
        if(!stop[7]) { 
            left(x,y,i); 
            stop[7]=legalMove(pos++,color); 
        }
    }
}

void blackAndWhiteKing(int x,int y,int color){
    int pos=0;
    diagonalLeftTop(x,y,1); 
    legalMove(pos++,color);

    diagonalRightTop(x,y,1); 
    legalMove(pos++,color);

    diagonalRightBottom(x,y,1); 
    legalMove(pos++,color);

    diagonalLeftBottom(x,y,1); 
    legalMove(pos++,color);

    forward(x,y,1); 
    legalMove(pos++,color);

    backward(x,y,1); 
    legalMove(pos++,color);

    right(x,y,1); 
    legalMove(pos++,color);

    left(x,y,1); 
    legalMove(pos++,color);
}

/*
    Supprime les mouvements hors plateau
*/
void isInTheMatrix(void){
    for(int i=0;i<52;i++) {
        int x=tempValidMove[i].x;
        int y=tempValidMove[i].y;
        if(x<0||x>7||y<0||y>7){
            resetPosition(i);
        }
    }
}

/*
    Cherche si (x,y) correspond à un mouvement stocké
*/
int isIn(int x,int y){
    for(int i=0;i<52;i++) {
        if(tempValidMove[i].x==x && tempValidMove[i].y==y) {
            return i;
        }
    }
    return -1;
}

/*
    Action sur la case :
    2 capture
    1 normal
    3 bloqué par allié
    0 invalide
    -1 pas dans la liste
*/
int action(int x,int y){
    int pos=isIn(x,y);
    if(pos==-1) {
        return -1;
    }

    if(tempValidMove[pos].valid){
        if(tempValidMove[pos].occupied) { 
            return 2;
        }
        return 1;
    }
    if(tempValidMove[pos].occupied) {
        return 3;
    }
    return 0;
}

/*
    Fonctions publiques
*/
void pawn(int x,int y,int color){
    resetAll();
    if(color==1) {
        whitePawn(x,y,color);
    }
    else {
        blackPawn(x,y,color);
    }
    isInTheMatrix();
    markPossibleMoves();
}

void knight(int x,int y,int color){
    resetAll(); 
    blackAndWhiteKnight(x,y,color);
    isInTheMatrix(); 
    markPossibleMoves();
}

void Bishop(int x,int y,int color){
    resetAll(); 
    blackAndWhiteBishop(x,y,color);
    isInTheMatrix(); 
    markPossibleMoves();
}

void Rook(int x,int y,int color){
    resetAll(); 
    blackAndWhiteRook(x,y,color);
    isInTheMatrix(); 
    markPossibleMoves();
}

void Queen(int x,int y,int color){
    resetAll(); 
    blackAndWhiteQueen(x,y,color);
    isInTheMatrix(); 
    markPossibleMoves();
}

void King(int x,int y,int color){
    resetAll(); 
    blackAndWhiteKing(x,y,color);
    isInTheMatrix(); 
    markPossibleMoves();
}

void BlackPiece(int x,int y,int color) {
    char pieces = board [y][x];
    switch (pieces)
    {
    case 'p':
        pawn(x,y,color);
        break;
    case 'q':
        Queen(x,y,color);
        break;
    case 'r':
        Rook(x,y,color);
        break;
    case 'b':
        Bishop(x,y,color);
        break;
    case 'n':
        knight(x,y,color);
        break;
    case 'k':
        King(x,y,color);
        break;
    default:
        error();
        break;
    }
}

void whitePiece(int x,int y,int color) {
        char pieces = board [y][x];
    switch (pieces)
    {
    case 'P':
        pawn(x,y,color);
        break;
    case 'Q':
        Queen(x,y,color);
        break;
    case 'R':
        Rook(x,y,color);
        break;
    case 'B':
        Bishop(x,y,color);
        break;
    case 'N':
        knight(x,y,color);
        break;
    case 'K':
        King(x,y,color);
        break;
    default:
        error();
        break;
    }
}


void pieces(int x,int y,int color) {
    if (!color) {
        BlackPiece(x,y,color);
    }
    if (color) {
        whitePiece(x,y,color);
    }
}