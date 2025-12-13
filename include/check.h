#ifndef CHECK_H
#define CHECK_H

// Check if a king of given color is in check
int isKingInCheck(int color);

// Check if a king of given color is in checkmate
int isCheckmate(int color);

// Check if a move would put the moving player's king in check
int wouldMovePutKingInCheck(int fromX, int fromY, int toX, int toY, int color);

// Find the position of the king for a given color
void findKing(int color, int *kingX, int *kingY);

#endif

