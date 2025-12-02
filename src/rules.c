#include "rules.h"
#include "board.h"
#include "pieces.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ------------------------------------------------
// Conversion ASCII → index 0–7
// ------------------------------------------------

int checkError(int x,int y,int color,int mode) {
    int condition = 0;
    int somone_here;

    switch (mode) {

    case 0:  // vérification de sélection de pièce
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            printf("Not in the board \n");
            condition = 1;
            break;
        }

        somone_here = isSomoneHere(x, y, color);

        if (somone_here != 1) {
            switch (somone_here) {
                case 0:
                    printf("Empty \n");
                    break;
                case 2:
                    printf("Wrong side \n");
                    break;
                default:
                    break;
            }
            condition = 1;
        }
        break;

    case 1:  // vérification de mouvement (si tu veux l'utiliser)
    {
        int result = action(x, y);
        switch (result) {
            case -1:
                printf("Destination not in possible moves.\n");
                condition = 1;
                break;
            case 3:
                printf("Destination blocked by ally.\n");
                condition = 1;
                break;
            case 0:
                printf("Invalid move.\n");
                condition = 1;
                break;
            case 1:
                printf("Move OK.\n");
                condition = 2;
                break;
            case 2:
                printf("Enemy captured!\n");
                condition = 2;
                break;
        }
        break;
    }
    default:
        break;
    }

    return condition;
}



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

int switchColor(int c) {
    return 1 - c;
}

// ------------------------------------------------
// Vérifie si le roi adverse existe encore
// ------------------------------------------------

int win(int c) {
    char target = (c == 1 ? 'k' : 'K');

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == target) {
                return 0;   // roi ennemi encore vivant
            }
        }
    }

    return 1;
}

// ------------------------------------------------
// Interaction utilisateur 2 (corrigée)
// ------------------------------------------------

int userInteraction(int color) {

    char input_initial[5];
    char input_final[5];
    int x1, x2, y1, y2;
    int check_error;
    int running = 1;

    do
    {
        resetPossibleMove();
        printBoard();

        printf("Select a piece on the board? ('save' or 's' to access saves menu)\n");
        printf("Piece: ");
        scanf("%4s", input_initial);

        if (strcmp(input_initial, "save") == 0 || strcmp(input_initial, "s") == 0) {
            printf("feature not available\n");
            continue;
        }

        x1 = charToInt(input_initial[0]);
        y1 = charToInt(input_initial[1]);

        check_error = checkError(x1, y1, color, 0);

        if (check_error == 1) {
            continue;
        }

        pieces(x1, y1, color);

        if (canWeMove() == 1) {
            printf("That piece cannot move \n");
            continue;
        }

        printBoard();

        while (1) {

            printf("Where do you want to put it now? ('undo' or 'u' to get back)\n");
            printf("Destination: ");
            scanf("%4s", input_final);

            if (strcmp(input_final, "undo") == 0 || strcmp(input_final, "u") == 0) {
                printf("go back in time\n");
                break;
            }

            x2 = charToInt(input_final[0]);
            y2 = charToInt(input_final[1]);

            check_error = checkError(x2, y2, color, 1);

            if (check_error == 1) {
                continue;
            }

            if (check_error == 2) {
                board[y2][x2] = board[y1][x1];
                board[y1][x1] = ' ';
                running = 0;  // <= correction : assignation, pas comparaison
                break;
            }
        }
        if (win(color) == 1) {
            running = 0;
            return 1;
            break;
        }
    }while (running);

    return 0;
}

// ------------------------------------------------
// Boucle de jeu principale
// ------------------------------------------------

void game(void) {
    int currentColor = 0;
    int end = 0;
    char color;

    while (1) {
        currentColor = switchColor(currentColor);

        if(currentColor == 1) {
            printf ("You play the White \n");
            color = 'white';
        }
        else {
            printf ("You play the black \n");
            color = 'black';
        }

        end = userInteraction(currentColor);

        if (end == 1) {
            printf("Win for %s!\n", currentColor == 1 ? "white" : "black");
            break;
        }
    }
}
