#include "board.h"
#include <stdio.h>

char board[8][8] = {
    {'r','n','b','q','k','b','n','r'},
    {'p','p','p','p','p','p','p','p'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'P','P','P','P','P','P','P','P'},
    {'R','N','B','Q','K','B','N','R'}
};

void printBoard() {

    printf("    A   B   C   D   E   F   G   H     \n");
    printf("  +---+---+---+---+---+---+---+---+   \n");
    
    for (int i = 0;i<8; i++) {
        int num = i;
        printf("%i  |",num++);
        for (int j = 0;j<8;j++) {
            printf(" %c |",board[i][j]);
        }
        printf("  %i\n",num);
        printf("  +---+---+---+---+---+---+---+---+   \n");
    }
    printf("    A   B   C   D   E   F   G   H     \n");
}