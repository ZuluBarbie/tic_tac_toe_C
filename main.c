#include <stdio.h>

char board[9] = {
    '1', '2', '3', 
    '4', '5', '6', 
    '7', '8', '9'
};

void displayBoard()
{
    printf("\n");
    printf("          TIC-TAC-TOE\n\n");

    printf("           %c | %c | %c\n", board[0], board[1], board[2]);
    printf("          ---+---+---\n");
    printf("           %c | %c | %c\n", board[3], board[4], board[5]);
    printf("          ---+---+---\n");
    printf("           %c | %c | %c\n", board[6], board[7], board[8]);

}

void getPlayerMove(int player)
{
    int move;
    printf("\nPlayer %d, enter your move (1-9): ", player);
    scanf("%d", &move);

    // Validate the move
    if (move < 1 || move > 9 || 
        board[move - 1] == 'X' || 
        board[move - 1] == 'O') {
        printf("Invalid move. Try again.\n");
        getPlayerMove(player); // Recursively ask for a valid move
    } else {
        board[move - 1] = (player == 1) ? 'X' : 'O';
    }
}

int checkWinner()
{
    // Check rows, columns, and diagonals for a winner
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2]) {
            return (board[i * 3] == 'X') ? 1 : 2;
        }
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6]) {
            return (board[i] == 'X') ? 1 : 2;
        }
    }
    if (board[0] == board[4] && board[4] == board[8]) {
        return (board[0] == 'X') ? 1 : 2;
    }
    if (board[2] == board[4] && board[4] == board[6]) {
        return (board[2] == 'X') ? 1 : 2;
    }
    return 0; // No winner yet
}

int main(void)
{   
    int player = 1;
    int winner = 0;
    int moves = 0;

    displayBoard();
    while (winner == 0 && moves < 9) {
        getPlayerMove(player);
        displayBoard();
        winner = checkWinner();
        if (winner != 0) {
            printf("Player %d wins!\n", winner);
            break;
        }
        player = (player == 1) ? 2 : 1; // Switch player
        moves++;
    }

    if (winner == 0) {
        printf("It's a draw!\n");
    }

    return 0;
}