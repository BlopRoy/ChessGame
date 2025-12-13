#include "rules.h"
#include "board.h"
#include "pieces.h"
#include "check.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
#define access _access
#define F_OK 0
#else
#include <unistd.h>
#endif

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
// Save/Load functionality
// ------------------------------------------------

// Ensure saves directory exists
void ensureSavesDirectory(void) {
    #ifdef _WIN32
    if (access("saves", F_OK) != 0) {
        system("mkdir saves 2>nul");
    }
    #else
    if (access("saves", F_OK) != 0) {
        system("mkdir -p saves 2>/dev/null");
    }
    #endif
}

// Save current game state
int saveGame(int currentColor, const char *filename) {
    ensureSavesDirectory();
    
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "saves/%s", filename);
    
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error: Could not create save file.\n");
        return 0;
    }
    
    // Write current player
    fprintf(file, "%d\n", currentColor);
    
    // Write board state
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            fprintf(file, "%c", board[y][x]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Game saved to %s\n", filepath);
    return 1;
}

// Load game state
int loadGame(int *currentColor, const char *filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "saves/%s", filename);
    
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error: Could not open save file.\n");
        return 0;
    }
    
    // Read current player
    if (fscanf(file, "%d", currentColor) != 1) {
        printf("Error: Invalid save file format.\n");
        fclose(file);
        return 0;
    }
    
    // Read board state
    char line[10];
    int ch = fgetc(file); // consume newline
    if (ch == EOF || ch != '\n') {
        // If no newline, rewind (might be first char of board)
        if (ch != EOF) {
            ungetc(ch, file);
        }
    }
    
    for (int y = 0; y < 8; y++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Error: Invalid save file format.\n");
            fclose(file);
            return 0;
        }
        // Validate line length (should be at least 8 characters + newline)
        size_t lineLen = strlen(line);
        if (lineLen < 8) {
            printf("Error: Invalid save file format (line too short).\n");
            fclose(file);
            return 0;
        }
        for (int x = 0; x < 8; x++) {
            board[y][x] = line[x];
        }
    }
    
    fclose(file);
    printf("Game loaded from %s\n", filepath);
    return 1;
}

// List all saved games
void listSaves(void) {
    ensureSavesDirectory();
    
    #ifdef _WIN32
    system("dir /b saves\\*.txt 2>nul");
    #else
    system("ls saves/*.txt 2>/dev/null | xargs -n1 basename");
    #endif
}

// Save menu
int saveMenu(int currentColor) {
    char filename[256];
    
    printf("\n=== Save Menu ===\n");
    printf("Enter save name (or 'cancel' to go back): ");
    scanf("%255s", filename);
    
    if (strcmp(filename, "cancel") == 0 || strcmp(filename, "c") == 0) {
        return 0;
    }
    
    // Add .txt extension if not present
    if (strstr(filename, ".txt") == NULL) {
        size_t len = strlen(filename);
        if (len < 252) {  // Leave room for ".txt" and null terminator
            strcat(filename, ".txt");
        } else {
            printf("Error: Filename too long.\n");
            return 0;
        }
    }
    
    return saveGame(currentColor, filename);
}

// Load menu
int loadMenu(int *currentColor) {
    char filename[256];
    
    printf("\n=== Load Menu ===\n");
    printf("Available saves:\n");
    listSaves();
    printf("\nEnter save name to load (or 'cancel' to go back): ");
    scanf("%255s", filename);
    
    if (strcmp(filename, "cancel") == 0 || strcmp(filename, "c") == 0) {
        return 0;
    }
    
    // Add .txt extension if not present
    if (strstr(filename, ".txt") == NULL) {
        size_t len = strlen(filename);
        if (len < 252) {  // Leave room for ".txt" and null terminator
            strcat(filename, ".txt");
        } else {
            printf("Error: Filename too long.\n");
            return 0;
        }
    }
    
    return loadGame(currentColor, filename);
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
            saveMenu(color);
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
                // Check if this move would put own king in check
                if (wouldMovePutKingInCheck(x1, y1, x2, y2, color)) {
                    printf("Invalid move: This would put your king in check!\n");
                    continue;
                }
                
                board[y2][x2] = board[y1][x1];
                board[y1][x1] = ' ';
                running = 0;  // <= correction : assignation, pas comparaison
                break;
            }
        }
        // Check for checkmate
        int opponentColor = switchColor(color);
        if (isCheckmate(opponentColor)) {
            printf("Checkmate! %s wins!\n", color == 1 ? "White" : "Black");
            running = 0;
            return 1;
        }
        
        // Check if opponent is in check
        if (isKingInCheck(opponentColor)) {
            printf("Check! %s king is in check.\n", opponentColor == 1 ? "White" : "Black");
        }
        
        if (win(color) == 1) {
            running = 0;
            return 1;
        }
    }while (running);

    return 0;
}

// ------------------------------------------------
// Main Menu
// ------------------------------------------------

int mainMenu(void) {
    int choice;
    
    printf("\n=== Chess Game ===\n");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    printf("3. Quit\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -1;
    }
    
    return choice;
}

// ------------------------------------------------
// Boucle de jeu principale
// ------------------------------------------------

void game(void) {
    int menuChoice = mainMenu();
    int currentColor = 0;
    int end = 0;
    
    if (menuChoice == 3) {
        printf("Goodbye!\n");
        return;
    }
    
    if (menuChoice == 2) {
        if (!loadMenu(&currentColor)) {
            printf("Loading cancelled or failed. Starting new game instead.\n");
            newGame();
            currentColor = 0;
        }
    } else {
        newGame();
        currentColor = 0;
    }

    while (1) {
        currentColor = switchColor(currentColor);

        if(currentColor == 1) {
            printf ("You play the White \n");
        }
        else {
            printf ("You play the Black \n");
        }

        end = userInteraction(currentColor);

        if (end == 1) {
            printf("Win for %s!\n", currentColor == 1 ? "White" : "Black");
            break;
        }
    }
}
