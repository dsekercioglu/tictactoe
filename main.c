#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define PLAYER int8_t
#define BOARD_SIZE 3

#define MIN_EVAL -2
#define LOSS -1
#define O -1
#define NONE 0
#define X 1

typedef struct TicTacToe {
    PLAYER board[BOARD_SIZE][BOARD_SIZE];
    bool turn;
} TicTacToe;

static struct TicTacToe newTicTacToe() {
    TicTacToe ticTacToe;
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
        for (uint8_t j = 0; j < BOARD_SIZE; j++) {
            ticTacToe.board[i][j] = NONE;
        }
    }
    ticTacToe.turn = true;
    return ticTacToe;
}

static void makeMove(TicTacToe *ttt, uint8_t x, uint8_t y) {
    PLAYER sideToMove;
    if (ttt->turn) {
        sideToMove = X;
    } else {
        sideToMove = O;
    }
    ttt->turn = !ttt->turn;
    ttt->board[x][y] = sideToMove;
}

static void unmakeMove(TicTacToe *ttt, uint8_t x, uint8_t y) {
    ttt->turn = !ttt->turn;
    ttt->board[x][y] = NONE;
}

static PLAYER outcome(TicTacToe *ttt) {
    for (int col = 0; col < BOARD_SIZE; col++) {
        PLAYER start = ttt->board[col][0];
        if (start != NONE) {
            bool win = true;
            for (int row = 1; row < BOARD_SIZE; row++) {
                PLAYER value = ttt->board[col][row];
                if (value != start) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return start;
            }
        }
    }
    for (int row = 0; row < BOARD_SIZE; row++) {
        PLAYER start = ttt->board[0][row];
        if (start != NONE) {
            bool win = true;
            for (int col = 1; col < BOARD_SIZE; col++) {
                PLAYER value = ttt->board[col][row];
                if (value != start) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return start;
            }
        }
    }

    PLAYER startDiag = ttt->board[0][0];
    if (startDiag != NONE) {
        bool win = true;
        for (uint8_t i = 1; i < BOARD_SIZE; i++) {
            if (ttt->board[i][i] != startDiag) {
                win = false;
                break;
            }
        }
        if (win) {
            return startDiag;
        }
    }
    startDiag = ttt->board[BOARD_SIZE - 1][0];
    if (startDiag != NONE) {
        bool win = true;
        for (uint8_t i = 1; i < BOARD_SIZE; i++) {
            if (ttt->board[BOARD_SIZE - 1 - i][i] != startDiag) {
                win = false;
                break;
            }
        }
        if (win) {
            return startDiag;
        }
    }
    return NONE;
}

static PLAYER negamax(TicTacToe *ttt, uint32_t *numGames) {

    PLAYER state = outcome(ttt);
    if (state != NONE) {
        *numGames += 1;
        return LOSS;
    }

    PLAYER maxScore = MIN_EVAL;
    bool gameEnded = true;
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
        for (uint8_t j = 0; j < BOARD_SIZE; j++) {
            if (ttt->board[i][j] == NONE) {
                gameEnded = false;
                makeMove(ttt, i, j);
                PLAYER score = (int8_t) (-negamax(ttt, numGames));
                unmakeMove(ttt, i, j);
                if (score > maxScore) {
                    maxScore = score;
                }
            }
        }
    }
    if (gameEnded) {
        PLAYER out = outcome(ttt);
        *numGames += 1;
        if (out == NONE) {
            return NONE;
        }
        return LOSS;
    }
    return maxScore;
}

int main() {
    TicTacToe ttt = newTicTacToe();
    uint32_t numGames = 0;
    clock_t start, end;
    start = clock();
    PLAYER out = negamax(&ttt, &numGames);
    end = clock();
    printf("solved in %ld\n", end - start);
    printf("optimal outcome: %i\n", out);
    printf("number of possible games: %i\n", numGames);

    return 0;
}
