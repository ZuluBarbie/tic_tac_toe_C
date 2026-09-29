#include "game.h"

void resetBoard(Board *board)
{
    for (int i = 0; i < 9; i++) board->cells[i] = (char)('1' + i);
}

int makeMove(Board *board, int index, char player)
{
    if (index < 0 || index >= 9 || (player != 'X' && player != 'O') ||
        board->cells[index] == 'X' || board->cells[index] == 'O' || checkWinner(board)) return 0;
    board->cells[index] = player;
    return 1;
}

int checkWinner(const Board *board)
{
    static const int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}
    };
    for (int i = 0; i < 8; i++) {
        char mark = board->cells[lines[i][0]];
        if ((mark == 'X' || mark == 'O') && mark == board->cells[lines[i][1]] &&
            mark == board->cells[lines[i][2]]) return mark == 'X' ? 1 : 2;
    }
    return 0;
}

int isFull(const Board *board)
{
    for (int i = 0; i < 9; i++)
        if (board->cells[i] != 'X' && board->cells[i] != 'O') return 0;
    return 1;
}

/* Negamax form of Minimax: the opponent's best score is our worst.
   Remaining empty squares favor faster wins and delay forced losses. */
static int minimax(Board *board, char player)
{
    int winner = checkWinner(board);
    if (winner) {
        int empty = 0;
        for (int i = 0; i < 9; i++)
            if (board->cells[i] != 'X' && board->cells[i] != 'O') empty++;
        return winner == (player == 'X' ? 1 : 2) ? 1 + empty : -1 - empty;
    }
    if (isFull(board)) return 0;
    int best = -100;
    for (int i = 0; i < 9; i++) {
        char saved = board->cells[i];
        if (saved == 'X' || saved == 'O') continue;
        board->cells[i] = player;
        int score = -minimax(board, player == 'X' ? 'O' : 'X');
        board->cells[i] = saved;
        if (score > best) best = score;
    }
    return best;
}

int bestMove(const Board *board, char player)
{
    if ((player != 'X' && player != 'O') || checkWinner(board) || isFull(board)) return -1;
    Board trial = *board;
    int best = -100, move = -1;
    /* Deterministic ties: center, corners, edges. */
    static const int order[9] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
    for (int j = 0; j < 9; j++) {
        int i = order[j];
        char saved = trial.cells[i];
        if (saved == 'X' || saved == 'O') continue;
        trial.cells[i] = player;
        int score = -minimax(&trial, player == 'X' ? 'O' : 'X');
        trial.cells[i] = saved;
        if (score > best) { best = score; move = i; }
    }
    return move;
}
