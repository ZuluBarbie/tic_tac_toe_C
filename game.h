#ifndef GAME_H
#define GAME_H

typedef struct { char cells[9]; } Board;
void resetBoard(Board *board);
/* Zero-based index. Returns 1 on success, 0 on invalid move. */
int makeMove(Board *board, int index, char player);
/* Original convention: 0 = none, 1 = X, 2 = O. */
int checkWinner(const Board *board);
int isFull(const Board *board);
/* Optimal empty index, or -1 for terminal board/invalid player.
   The supplied board is never modified. */
int bestMove(const Board *board, char player);
#endif
