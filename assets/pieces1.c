#include "pieces.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"

#define INVALID -1

int tempLock[2] = {0, 0};

typedef struct {
    int x;
    int y;
    int occupied;   // 1 if a piece is on the square, 0 if empty
    int valid;      // 1 if the move is allowed, 0 if not
} Coordinate;

Coordinate tempValidMove[52];

int movePreview[52][2];

void error(void) {
    printf("error");
}
#include "pieces.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"

#define INVALID -1

int tempLock[2] = {0, 0};

typedef struct {
    int x;
    int y;
    int occupied;   // 1 if a piece is on the square, 0 if empty
    int valid;      // 1 if the move is allowed, 0 if not
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

void resetPossibleMove(void) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            possibleMove[i][j] = ' ';
}

void markPossibleMoves(void) {
    resetPossibleMove();

    for (int i = 0; i < 52; i++) {
        if (tempValidMove[i].x != INVALID && tempValidMove[i].y != INVALID) {
            int x = tempValidMove[i].x;
            int y = tempValidMove[i].y;

            if (tempValidMove[i].valid) {
                if (tempValidMove[i].occupied)
                    possibleMove[x][y] = '*'; // capture
                else
                    possibleMove[x][y] = '%'; // normal move
            } else if (tempValidMove[i].occupied) {
                possibleMove[x][y] = 'X';     // blocked by ally
            }
        }
    }
}

void printPossibleMoves(void) {
    printf("Possible Moves:\n");
    for (int y = 7; y >= 0; y--) {
        for (int x = 0; x < 8; x++) {
            printf("%c ", possibleMove[x][y]);
        }
        printf("\n");
    }
}

/* Check if a square is occupied by ally/enemy/empty for black pieces */
int isSomoneHereBlack(int x,int y) {
    if (board[x][y] != ' ') {
        if (islower(board[x][y]))
            return 1; // ally
        return 2;     // enemy
    }
    return 0;         // nobody
}

/* Check if a square is occupied by ally/enemy/empty for white pieces */
int isSomoneHereWhite(int x,int y) {
    if (board[x][y] != ' ') {
        if (isupper(board[x][y]))
            return 1; // ally
        return 2;     // enemy
    }
    return 0;         // nobody
}

/* General function: 0 = black, 1 = white */
int isSomoneHere(int x, int y, int color) {
    if (color == 0)
        return isSomoneHereBlack(x, y);
    else if (color == 1)
        return isSomoneHereWhite(x, y);
    return -1;
}

/* Reset a single move */
void resetPosition(int position) {
    tempValidMove[position].x = INVALID;
    tempValidMove[position].y = INVALID;
    tempValidMove[position].occupied = INVALID;
    tempValidMove[position].valid = INVALID;
}

/* Reset all moves */
void resetAll(void) {
    for (int i = 0; i < 52; i++) {
        resetPosition(i);
    }
}

/* Store a move at position, check occupancy */
int legalMove(int position, int color) {
    tempValidMove[position].x = tempLock[0];
    tempValidMove[position].y = tempLock[1];

    int status = isSomoneHere(tempLock[0], tempLock[1], color);

    switch (status) {
        case 0: // empty
            tempValidMove[position].occupied = 0;
            tempValidMove[position].valid = 1;
            break;
        case 1: // ally
            tempValidMove[position].occupied = 1;
            tempValidMove[position].valid = 0;
            break;
        case 2: // enemy
            tempValidMove[position].occupied = 1;
            tempValidMove[position].valid = 1;
            break;
    }

    return 1;
}

/* Movement helpers */
void forward(int x, int y, int step) { tempLock[0] = x; tempLock[1] = y + step; }
void backward(int x, int y, int step) { tempLock[0] = x; tempLock[1] = y - step; }
void left(int x, int y, int step) { tempLock[0] = x - step; tempLock[1] = y; }
void right(int x, int y, int step) { tempLock[0] = x + step; tempLock[1] = y; }
void diagonalLeftTop(int x, int y, int step) { tempLock[0] = x - step; tempLock[1] = y + step; }
void diagonalRightTop(int x, int y, int step) { tempLock[0] = x + step; tempLock[1] = y + step; }
void diagonalRightBottom(int x, int y, int step) { tempLock[0] = x + step; tempLock[1] = y - step; }
void diagonalLeftBottom(int x, int y, int step) { tempLock[0] = x - step; tempLock[1] = y - step; }

/* Pawns */
void blackPawn(int x, int y) {

    backward(x, y, 1);
    legalMove(0, color);

    diagonalLeftBottom(x, y, 1);
    legalMove(1, color);

    diagonalRightBottom(x, y, 1);
    legalMove(2, color);
}

void whitePawn(int x, int y) {

    forward(x, y, 1);
    legalMove(0, color);

    diagonalLeftTop(x, y, 1);
    legalMove(1, color);

    diagonalRightTop(x, y, 1);
    legalMove(2, color);
}

/* Knights */
void blackAndWhiteKnight(int x, int y) {
    int color = isupper(board[x][y]) ? 1 : 0;

    forward(x, y, 2); left(x, y, 1); legalMove(0, color);
    forward(x, y, 2); right(x, y, 1); legalMove(1, color);
    backward(x, y, 2); left(x, y, 1); legalMove(2, color);
    backward(x, y, 2); right(x, y, 1); legalMove(3, color);
    right(x, y, 2); backward(x, y, 1); legalMove(4, color);
    right(x, y, 2); forward(x, y, 1); legalMove(5, color);
    left(x, y, 2); backward(x, y, 1); legalMove(6, color);
    left(x, y, 2); forward(x, y, 1); legalMove(7, color);
}

/* Bishops */
void blackAndWhiteBishop(int x, int y) {
    int color = isupper(board[x][y]) ? 1 : 0;
    int pos = -1;
    for (int i = 1; i <= 7; i++) {
        diagonalLeftBottom(x, y, i); legalMove(++pos, color);
        diagonalLeftTop(x, y, i); legalMove(++pos, color);
        diagonalRightBottom(x, y, i); legalMove(++pos, color);
        diagonalRightTop(x, y, i); legalMove(++pos, color);
    }
}

/* Rooks */
void blackAndWhiteRook(int x, int y) {
    int color = isupper(board[x][y]) ? 1 : 0;
    int pos = -1;
    for (int i = 1; i <= 7; i++) {
        forward(x, y, i); legalMove(++pos, color);
        backward(x, y, i); legalMove(++pos, color);
        right(x, y, i); legalMove(++pos, color);
        left(x, y, i); legalMove(++pos, color);
    }
}

/* Queens */
void blackAndWhiteQueen(int x, int y) {
    int color = isupper(board[x][y]) ? 1 : 0;
    int pos = -1;
    for (int i = 1; i <= 7; i++) {
        diagonalLeftBottom(x, y, i); legalMove(++pos, color);
        diagonalLeftTop(x, y, i); legalMove(++pos, color);
        diagonalRightBottom(x, y, i); legalMove(++pos, color);
        diagonalRightTop(x, y, i); legalMove(++pos, color);

        forward(x, y, i); legalMove(++pos, color);
        backward(x, y, i); legalMove(++pos, color);
        right(x, y, i); legalMove(++pos, color);
        left(x, y, i); legalMove(++pos, color);
    }
}

/* Kings */
void blackAndWhiteKing(int x, int y) {
    int color = isupper(board[x][y]) ? 1 : 0;
    int pos = 0;

    diagonalLeftBottom(x, y, 1); legalMove(pos++, color);
    diagonalLeftTop(x, y, 1); legalMove(pos++, color);
    diagonalRightBottom(x, y, 1); legalMove(pos++, color);
    diagonalRightTop(x, y, 1); legalMove(pos++, color);
    forward(x, y, 1); legalMove(pos++, color);
    backward(x, y, 1); legalMove(pos++, color);
    right(x, y, 1); legalMove(pos++, color);
    left(x, y, 1); legalMove(pos++, color);
}

/* Discard moves outside 0–7 board limits */
void isInTheMatrix(void) {
    for (int i = 0; i < 52; i++) {
        if (tempValidMove[i].x < 0 || tempValidMove[i].y < 0 ||
            tempValidMove[i].x > 7 || tempValidMove[i].y > 7) {
            resetPosition(i);
        }
    }
}

/* Check if coordinates exist in tempValidMove */
int isIn(int x, int y) {
    for (int i = 0; i < 52; i++) {
        if (x == tempValidMove[i].x && y == tempValidMove[i].y)
            return i;
    }
    return -1;
}

/* Determine action on a square */
int action(int x, int y) {
    int position = isIn(x, y);
    if (position != -1) {
        if (tempValidMove[position].valid) {
            if (tempValidMove[position].occupied) return 2;
            return 1;
        } else if (tempValidMove[position].occupied) return 3;
        return 0;
    }
    return -1;
}

/* Public functions */
int pawn(int x, int y) {
    resetAll();
    if (isupper(board[x][y])) whitePawn(x, y);
    else blackPawn(x, y);
    isInTheMatrix();
    return action(x, y);
}

int knight(int x, int y) {
    resetAll();
    blackAndWhiteKnight(x, y);
    isInTheMatrix();
    return action(x, y);
}

int Bishop(int x, int y) {
    resetAll();
    blackAndWhiteBishop(x, y);
    isInTheMatrix();
    return action(x, y);
}

int Rook(int x, int y) {
    resetAll();
    blackAndWhiteRook(x, y);
    isInTheMatrix();
    return action(x, y);
}

int Queen(int x, int y) {
    resetAll();
    blackAndWhiteQueen(x, y);
    isInTheMatrix();
    return action(x, y);
}

int King(int x, int y) {
    resetAll();
    blackAndWhiteKing(x, y);
    isInTheMatrix();
    return action(x, y);
}
