# Tic-Tac-Toe in C

**A small board. A complete game. An unbeatable opponent.**

A portable terminal game built from my first two-player C implementation into a structured project with input validation, replay, session scores, and a Minimax computer opponent. No third-party C libraries required.

## Features

- **Two players:** share a keyboard as Player 1 (X) and Player 2 (O).
- **Play the computer:** you are X; the computer is O and searches every possible continuation.
- **Reliable rules:** all rows, columns, and diagonals; wins checked before draws, including on the ninth move.
- **Safe input:** rejects letters, mixed input, out-of-range numbers, occupied squares, and overlong lines without consuming a turn.
- **Replay and mode switching:** start a fresh board while retaining separate scores for each mode.
- **Graceful exit:** enter `q` at any prompt, or close standard input.
- **Automated tests:** engine checks, exhaustive opponent paths, terminal regression tests, and a GitHub Actions setup template.

Scores last for the current process only. Abandoned rounds do not count. X always starts; in computer mode, the human always plays X.

## Build and play

Requirements: a C11 compiler such as GCC or Clang. Python 3 is optional and used only for terminal tests. Run commands from the repository root.

### Windows / PowerShell (GCC on PATH)

```powershell
New-Item -ItemType Directory -Force build | Out-Null
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c game.c -o build/tictactoe.exe
.\build\tictactoe.exe
```

### Linux / macOS

```sh
mkdir -p build
cc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c game.c -o build/tictactoe
./build/tictactoe
```

### VS Code

Open this folder and use the integrated terminal with the commands above. Compile **both** `main.c` and `game.c`; a default “build active file” task will not link the complete game. Editor settings are intentionally kept local.

## How to play

Select `1` for two players or `2` for the computer. Choose an empty square using the printed numbers:

```text
       1 | 2 | 3
      ---+---+---
       4 | 5 | 6
      ---+---+---
       7 | 8 | 9
```

Three matching marks in a row, column, or diagonal win. A full board without a winner is a draw. After each round, choose replay, change mode, or `q` to exit.

### Example: Player 1 wins

Choose two-player mode, then enter `1, 4, 2, 5, 3` on successive turns:

```text
       X | X | X
      ---+---+---
       O | O | 6
      ---+---+---
       7 | 8 | 9

Player 1 (X) wins!
Score | Player 1 (X): 1 | Player 2 (O): 0 | Draws: 0

1. Play again
2. Change mode
Choose (1-2, q to quit):
```

## Project structure

```text
main.c                    Terminal UI, input, rounds, replay, scores
game.h                    Board type and documented engine interface
game.c                    Rules and Minimax search
tests/test_game.c         Engine checks and exhaustive AI play paths
tests/test_cli.py          End-to-end input and session tests
ci/github-actions.yml     Optional GitHub Actions workflow template
.gitignore                Excludes binaries and local editor settings
README.md                 This guide
```

## Run the tests

Build the game first, then on Windows:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 tests/test_game.c game.c -o build/test_game.exe
.\build\test_game.exe
python tests/test_cli.py ./build/tictactoe.exe
```

On Linux / macOS:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 tests/test_game.c game.c -o build/test_game
./build/test_game
python3 tests/test_cli.py ./build/tictactoe
```

The engine suite checks all eight winning lines for both symbols, invalid moves, draws, immediate wins, forced blocks, and board preservation. It explores **613 complete opponent paths** against the deterministic AI playing either X or O and asserts the AI never loses. This covers every legal opponent response against this AI's selected moves, rather than every possible AI strategy. The seven terminal tests cover both winners, draws, ninth-move wins, replay scores, mode switching, malformed input, occupied squares, quit, and end-of-file.

## Learning notes

### From the original code

The first commit preserves my original `main.c`. The finished version retains the numbered board, `displayBoard`, and the `checkWinner` convention (`0`: none, `1`: X, `2`: O). A `Board` struct replaces global mutable state, and `playGame` separates a round from the menu loop.

### Input validation

Reading an integer with unchecked `scanf` can leave invalid text waiting in the input stream. Recursively asking again can also grow the call stack. Here, an iterative reader consumes a full line, trims surrounding whitespace, and accepts exactly one permitted digit or `q`. Excessively long lines are drained and rejected. EOF exits cleanly.

### Minimax

The AI tries each legal move and recursively examines the opponent's replies until a win or draw. This implementation uses the **Negamax** form of Minimax: the opponent's score is negated when evaluating a move. Terminal wins score positively, losses negatively, and draws zero. Remaining empty squares favor earlier wins and postpone unavoidable losses. Equal scores prefer center, corners, then edges.

Every simulated move is undone before trying the next one; the public AI function works on a copy so it cannot change the caller's board. The nine-square search is small enough to solve without caching or pruning. Perfect play from both sides produces a draw.

### Separation of concerns

The engine knows the board and rules. The terminal layer knows prompts and scores. This makes the engine testable without simulating keyboard input, while separate process tests verify the actual user experience.

## Possible extensions

Persistent scores, selectable human symbols, difficulty levels, and a graphical front end are natural next steps.

## Enable GitHub Actions

The publishing credential did not include GitHub's workflow permission, so the CI template is provided in ci/github-actions.yml. To enable automated Linux checks, copy it to .github/workflows/ci.yml and commit using a credential with workflow permission, or use the GitHub web editor. The template builds with strict warnings and UndefinedBehaviorSanitizer, then runs both test suites. Local Windows tests have passed; hosted CI has not yet run.

