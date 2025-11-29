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
    {'p','p','p','p','p','p','p','p'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

// Déclaré dans pieces.c
extern char possibleMove[8][8];


// ------------------------------------------------
// Conversion des coordonnées
// ------------------------------------------------

/*
    Convertit un caractère ASCII en index 0–7
    A–H / a–h → 0–7
    1–8 → 0–7
*/
int charToInt(char input) {
    if (input >= '1' && input <= '8')
        return input - '1';

    if (input >= 'A' && input <= 'H')
        return input - 'A';

    if (input >= 'a' && input <= 'h')
        return input - 'a';

    return -1;
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


// ------------------------------------------------
// Interaction utilisateur
// ------------------------------------------------


void userInteraction(int color) {

    char input_initial[3];
    char input_final[3];

    while (1) {

        // --------------------------
        // 1. Choix de la pièce
        // --------------------------

        printf("Give origin coordinate (A1): ");
        scanf("%2s", input_initial);

        int x1 = charToInt(input_initial[0]);
        int y1 = charToInt(input_initial[1]);

        if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) {
            printf("Invalid coordinate.\n");
            continue;
        }

        if (isSomoneHere(x1, y1, color) != 1) {
            printf("No valid piece here.\n");
            continue;
        }

        // Affiche les coups possibles
        pieces(x1, y1, color);


        // --------------------------
        // 2. Choix de la destination
        // --------------------------

        printf("Give destination coordinate (A1): ");
        scanf("%2s", input_final);

        int x2 = charToInt(input_final[0]);
        int y2 = charToInt(input_final[1]);

        if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7) {
            printf("Invalid coordinate.\n");
            continue;
        }

        int result = action(x2, y2);

        switch (result) {
            case -1:
                printf("Destination not in possible moves.\n");
                continue;

            case 3:
                printf("Destination blocked by ally piece.\n");
                continue;

            case 0:
                printf("Invalid move.\n");
                continue;

            case 1:
                printf("Move successful.\n");
                break;

            case 2:
                printf("Enemy piece captured!\n");
                break;
        }


        // --------------------------
        // 3. On applique le mouvement
        // --------------------------

        board[y2][x2] = board[y1][x1];
        board[y1][x1] = ' ';

        printBoard();
    }
}
