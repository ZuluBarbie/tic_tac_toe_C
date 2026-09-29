#include "game.h"
#include <ctype.h>
#include <stdio.h>

typedef struct { unsigned x, o, draws; } Score;

/* Consume complete lines, including overlong input, before retrying. */
static int readChoice(const char *prompt, int low, int high)
{
    for (;;) {
        char line[128];
        size_t n = 0;
        int c, bad = 0;
        printf("%s", prompt);
        fflush(stdout);
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c == 0) bad = 1;
            if (n + 1 < sizeof line) line[n++] = (char)c;
            else bad = 1;
        }
        if (c == EOF && n == 0 && !bad) return 0;
        size_t start = 0;
        while (start < n && isspace((unsigned char)line[start])) start++;
        while (n > start && isspace((unsigned char)line[n - 1])) n--;
        if (!bad && n == start + 1) {
            c = line[start];
            if (c == 'q' || c == 'Q') return 0;
            if (c >= '0' + low && c <= '0' + high) return c - '0';
        }
        printf("Please enter one number from %d to %d, or q to quit.\n", low, high);
    }
}

static void displayBoard(const Board *board)
{
    puts("");
    for (int row = 0; row < 3; row++) {
        printf("       %c | %c | %c\n", board->cells[row * 3],
               board->cells[row * 3 + 1], board->cells[row * 3 + 2]);
        if (row < 2) puts("      ---+---+---");
    }
    puts("");
}

static void displayScore(const Score *score, int mode)
{
    printf("Score | %s: %u | %s: %u | Draws: %u\n",
           mode == 1 ? "Player 1 (X)" : "You (X)", score->x,
           mode == 1 ? "Player 2 (O)" : "Computer (O)", score->o, score->draws);
}

static int playGame(int mode, Score *score)
{
    Board board;
    resetBoard(&board);
    char player = 'X';
    displayBoard(&board);
    for (;;) {
        int move;
        if (mode == 2 && player == 'O') {
            move = bestMove(&board, player);
            printf("Computer chooses %d.\n", move + 1);
        } else {
            char prompt[80];
            snprintf(prompt, sizeof prompt, "%s (%c), choose a square (1-9, q to quit): ",
                     mode == 2 ? "You" : player == 'X' ? "Player 1" : "Player 2", player);
            move = readChoice(prompt, 1, 9);
            if (!move) return 0;
            move--;
        }
        if (!makeMove(&board, move, player)) {
            puts("That square is occupied. Choose an empty square.");
            continue;
        }
        displayBoard(&board);
        int winner = checkWinner(&board);
        if (winner || isFull(&board)) {
            if (winner == 1) {
                score->x++;
                puts(mode == 1 ? "Player 1 (X) wins!" : "You win!");
            } else if (winner == 2) {
                score->o++;
                puts(mode == 1 ? "Player 2 (O) wins!" : "Computer (O) wins!");
            } else {
                score->draws++;
                puts("It's a draw!");
            }
            displayScore(score, mode);
            return 1;
        }
        player = player == 'X' ? 'O' : 'X';
    }
}

int main(void)
{
    Score scores[2] = {{0, 0, 0}, {0, 0, 0}};
    puts("\n  =============================\n          TIC-TAC-TOE\n  =============================");
    puts("Three in a row. Two ways to play.\nX starts each round. Enter q at any prompt to quit.");
    for (;;) {
        puts("\n1. Two players\n2. Play the computer (Minimax)");
        int mode = readChoice("Choose a mode (1-2): ", 1, 2);
        if (!mode) break;
        int action;
        do {
            if (!playGame(mode, &scores[mode - 1])) goto finished;
            puts("\n1. Play again\n2. Change mode");
            action = readChoice("Choose (1-2, q to quit): ", 1, 2);
            if (!action) goto finished;
        } while (action == 1);
    }
finished:
    puts("\nSession results (completed rounds only)");
    displayScore(&scores[0], 1);
    displayScore(&scores[1], 2);
    puts("Thanks for playing!");
    return 0;
}
