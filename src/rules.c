#include "rules.h"
#include "board.h"
#include "pieces.h"
#include <stdio.h>

// ------------------------------------------------
// Conversion ASCII → index 0–7
// ------------------------------------------------

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
// Inversion couleur (0 ↔ 1)
// ------------------------------------------------

int switchColor(int c) {
    return 1 - c;
}

// ------------------------------------------------
// Vérifie si le roi adverse existe encore
// c = couleur du JOUEUR qui vient de jouer
// ------------------------------------------------

int win(int x, int y, int c) {
    char target = (c == 1 ? 'k' : 'K');  // si joueur est blanc -> cible roi noir

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == target) {
                return 0;   // roi ennemi encore vivant
            }
        }
    }

    // roi ennemi absent → victoire
    return (x * 10) + y;
}

// ------------------------------------------------
// Interaction utilisateur
// ------------------------------------------------

int userInteraction(int playerColor) {

    char input_initial[3];
    char input_final[3];
    int x1, x2, y1, y2;

    printBoard();

    while (1) {

        // --------------------------
        // 1. Choix de la pièce
        // --------------------------

        printf("Give origin coordinate (A1): ");
        scanf("%2s", input_initial);

        x1 = charToInt(input_initial[0]);
        y1 = charToInt(input_initial[1]);

        if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) {
            printf("Invalid coordinate.\n");
            continue;
        }

        if (isSomoneHere(x1, y1, playerColor) != 1) {
            printf("No valid piece here.\n");
            continue;
        }

        pieces(x1, y1, playerColor); // affiche les coups possibles


        // --------------------------
        // 2. Choix destination
        // --------------------------
        while (1) {

            printf("Give destination coordinate (A1): ");
            scanf("%2s", input_final);

            x2 = charToInt(input_final[0]);
            y2 = charToInt(input_final[1]);

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
                    printf("Destination blocked by ally.\n");
                    continue;
                case 0:
                    printf("Invalid move.\n");
                    continue;
                case 1:
                    printf("Move OK.\n");
                    break;
                case 2:
                    printf("Enemy captured!\n");
                    break;
            }
            break;
        }

        // --------------------------
        // 3. Vérifie victoire + applique mouvement
        // --------------------------

        board[y2][x2] = board[y1][x1];
        board[y1][x1] = ' ';

        resetPossibleMove();
        printBoard();
        if (win(x2, y2, playerColor) == (x2 * 10 + y2)) {
            return 1;
            break;
        }

        break;
    }

    return 0;
}

// ------------------------------------------------
// Boucle de jeu principale
// ------------------------------------------------

void game(void) {
    int currentColor = 0; // 1 = blanc par convention
    int end = 0;

    while (1) {
        currentColor = switchColor(currentColor);  // alterne joueur

        end = userInteraction(currentColor);

        if (end == 1) {
            printf("Win for %s!\n", currentColor == 1 ? "white" : "black");
            break;
        }
    }
}
