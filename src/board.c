#include "board.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "pieces.h"

// ------------------------------------------------
// Plateau d’échecs
// ------------------------------------------------

char board[8][8] = {
    {'r','n','b','k','q','b','n','r'},
    {'p','p','p','p',' ','p','p','p'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'P','P','P','P',' ','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

void newGame(void) {

    // Set up major pieces
    board[0][0] = board[0][7] = 'r';
    board[7][0] = board[7][7] = 'R';

    board[0][1] = board[0][6] = 'n';
    board[7][1] = board[7][6] = 'N';

    board[0][2] = board[0][5] = 'b';
    board[7][2] = board[7][5] = 'B';

    board[0][3] = 'q';
    board[0][4] = 'k';
    
    board[7][3] = 'Q';
    board[7][4] = 'K';

    // Pawns
    for (int i = 0; i < 8; i++) {
        board[1][i] = 'p';
        board[6][i] = 'P';
    }

    // Empty squares
    for (int i = 2; i <= 5; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = ' ';
        }
    }
}

// ------------------------------------------------
// Affichage du plateau
// ------------------------------------------------

void printBoard() {

    printf("    A   B   C   D   E   F   G   H\n");
    printf("  +---+---+---+---+---+---+---+---+\n");

    for (int y = 0; y < 8; y++) {

        printf("%d |", y + 1);

        for (int x = 0; x < 8; x++) {
            printf(" %c%c|", board[y][x], possibleMove[y][x]);
        }

        printf(" %d\n", y + 1);
        printf("  +---+---+---+---+---+---+---+---+\n");
    }

    printf("    A   B   C   D   E   F   G   H\n");
}