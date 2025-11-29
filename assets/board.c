#include "board.h"
#include <stdio.h>
#include <ctype.h>
#include "pieces.h"

extern int color = 0;// 0 white 1 black

char board[8][8] = {
    {'p',' ',' ',' ',' ',' ',' ',' '},
    {'P','p',' ',' ',' ',' ',' ',' '},
    {' ','p',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '}
};

void piecesMove(int color,int pos[2]) {
    int x = pos[0];
    int y = pos[1];
 
    switch (color)
    {
    case 0:
        switch (board[x][y])
        {
        case 'p':
            
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
}

int charToInt(char input) {
    if (isdigit(input)) {
        return (int)input - 49;
    }
    else if (isupper(input))
    {
        return (int)input - 65;
    }
    else if (islower(input))
    {
        return (int)input - 97;
    }
    else {
        return -1;
    }
    
}

void printBoard() {

    printf("    A   B   C   D   E   F   G   H     \n");
    printf("  +---+---+---+---+---+---+---+---+   \n");
    
    for (int i = 0;i<8; i++) {
        int num = i;
        printf("%i |",++num);
        for (int j = 0;j<8;j++) {
            printf(" %c%c|",board[i][j],possibleMove[i][j]);
        }
        printf("  %i\n",num);
        printf("  +---+---+---+---+---+---+---+---+   \n");
    }
    printf("    A   B   C   D   E   F   G   H     \n");
}

void userInteraction() {
    char input_initial[2];
    char input_final[2];

    while (1)
    {
        printf("Give origin Coordinate (A8):");
        scanf("%s",input_initial);
        //int pos[2] = {charToInt(input_initial[0]),charToInt(input_initial[1])};
        //piecesMove(color,pos);

        printf("Give destination Coordinate (A8):");
        scanf("%s",input_final);
        //int pos[2] = {charToInt(input_final[0]),charToInt(input_final[1])};
    }
    
}