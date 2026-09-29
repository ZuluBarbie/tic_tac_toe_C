#include "../game.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned long paths;
static void explore(Board board, char turn, char ai)
{
    int winner = checkWinner(&board);
    if (winner || isFull(&board)) {
        assert(winner == 0 || winner == (ai == 'X' ? 1 : 2));
        paths++;
        return;
    }
    if (turn == ai) {
        Board before = board;
        int move = bestMove(&board, ai);
        assert(memcmp(&before, &board, sizeof board) == 0);
        assert(makeMove(&board, move, turn));
        explore(board, turn == 'X' ? 'O' : 'X', ai);
    } else {
        for (int i = 0; i < 9; i++) {
            Board child = board;
            if (makeMove(&child, i, turn)) explore(child, ai, ai);
        }
    }
}

int main(void)
{
    Board b;
    resetBoard(&b);
    assert(!checkWinner(&b) && !isFull(&b));
    assert(!makeMove(&b, -1, 'X') && !makeMove(&b, 9, 'X'));
    assert(!makeMove(&b, 0, 'Z'));
    assert(makeMove(&b, 0, 'X') && !makeMove(&b, 0, 'O'));
    const int lines[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int p = 1; p <= 2; p++) {
        for (int l = 0; l < 8; l++) {
            resetBoard(&b);
            for (int j = 0; j < 3; j++) b.cells[lines[l][j]] = p == 1 ? 'X' : 'O';
            assert(checkWinner(&b) == p);
            assert(bestMove(&b, 'X') == -1);
        }
    }
    memcpy(b.cells, "XOXXOOOXX", 9);
    assert(isFull(&b) && !checkWinner(&b) && bestMove(&b, 'O') == -1);
    memcpy(b.cells, "XX3OO6789", 9);
    assert(bestMove(&b, 'X') == 2);
    memcpy(b.cells, "XX34O6789", 9);
    assert(bestMove(&b, 'O') == 2);
    resetBoard(&b);
    assert(bestMove(&b, 'Z') == -1);
    explore(b, 'X', 'O');
    explore(b, 'X', 'X');
    printf("Engine tests passed; AI never loses across %lu complete opponent paths (both symbols).\n", paths);
    return 0;
}
