#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    MINED,
    HIDDEN,
    FLAGGED,
    REVEALED
} CELL_TYPE;

struct cell {
    CELL_TYPE type;
    int neighbors;
    bool flagged;
};

struct board {
    int width;
    int height;
    struct cell ***squares;
};

struct board ms_initialize() {
    struct board game = {10, 10, NULL};
    game.squares = malloc(sizeof(*game.squares) * game.height);

    for (int i = 0; i < game.height; i++) { // set mines
        game.squares[i] = malloc(sizeof(*game.squares[i]) * game.width);
        for (int j = 0; j < game.width; j++) {
            struct cell *square = malloc(sizeof(*square));
            if (rand() % 10 == 0) {
                square->type = MINED;
            } else {
                square->type = HIDDEN;
            }
            square->neighbors = 0;
            game.squares[i][j] = square;
        }
    }
    return game;
}
void ms_calculate_neighbors(struct board game) {
    for (int i = 0; i < game.height; i++) {
        for (int j = 0; j < game.width; j++) {
            for (int ii = i - 1; ii <= i + 1; ii++) {
                for (int jj = j - 1; jj <= j + 1; jj++) {
                    if (ii >= 0 && ii < game.height 
                            && jj >= 0 && jj < game.width
                            && game.squares[ii][jj]->type == MINED) {
                        game.squares[i][j]->neighbors++;
                    }
                }
            }
        }
    }
}

void ms_display_board(struct board game) {
    printf("+  ");
    for (int i = 0; i < game.width; i++) {
        printf("%d ", i);
    }
    printf("\n \n");
    for (int i = 0; i < game.height; i++) {
        printf("%d  ", i);
        for (int j = 0; j < game.width; j++) {
            if (game.squares[i][j]->flagged) {
                printf("F ");
            } else if (game.squares[i][j]->type == MINED || game.squares[i][j]->type == HIDDEN) { 
                printf(". ");
            } else {
                printf("%d ", game.squares[i][j]->neighbors);
            }
        }
        printf("\n");
    }
}

/*
static char *cell_display = "mf.";
cell_display[squares[y][x]->type]
*/
void ms_print_all(struct board game) { 
    for (int i = 0; i < game.height; i++) {
        for (int j = 0; j < game.width; j++) {
            if (game.squares[i][j]->type == MINED) {
                printf("M ");
            } else {
                printf("%d ", game.squares[i][j]->neighbors);
            }
        }
        printf("\n");
    }
}

void ms_destroy(struct board game) {
    for (int i = 0; i < game.height; i++) {
        for (int j = 0; j < game.width; j++) {
            free(game.squares[i][j]);
        }
        free(game.squares[i]);
    }
    free(game.squares);
}

void ms_reveal_neighbors(struct board game, int x, int y) { // maybe we only need one function?
    if (y < 0 || y >= game.height || x < 0 || x >= game.width) {
        return;
    }
    if (game.squares[y][x]->neighbors > 0) {
        game.squares[y][x]->type = REVEALED;
        return;
    }
    if (game.squares[y][x]->type == REVEALED) {
        return;
    }
    game.squares[y][x]->type = REVEALED;
    for (int i = y - 1; i <= y + 1; i++) {
        for (int j = x - 1; j <= x + 1; j++) {
            ms_reveal_neighbors(game, j, i);
        }
    }
}
void ms_make_move(struct board game, int x, int y) {
    if (game.squares[y][x]->type == HIDDEN) {
        ms_reveal_neighbors(game, x, y);
    }
}

bool ms_check_mined(struct board game, int x, int y) {
    return game.squares[y][x]->type == MINED;
}

void ms_flag_square(struct board game, int x, int y) {
    if (game.squares[y][x]->type == REVEALED) return;
    game.squares[y][x]->flagged = !game.squares[y][x]->flagged;
}

bool ms_check_win(struct board game) {
    for (int y = 0; y < game.height; y++) {
        for (int x = 0; x < game.width; x++) {
            if (game.squares[y][x]->type == HIDDEN) {
                return false;
            }
        }
    }
    return true;
}

int main(void) {
    // srand(time(NULL));
    struct board game = ms_initialize();
    ms_calculate_neighbors(game);

    while (true) {
        ms_display_board(game);
        int x, y;
        char action;
        printf("\nEnter your move coordinates (e.g. x y): ");
        scanf("%d %d", &x, &y);
        printf("Reveal or flag %d %d (R/F): ", x, y);
        scanf("\n%c", &action);
        if (action == 'R') {
            if (ms_check_mined(game, x, y)) {
                printf("%d %d was a mine!\n", x, y);
                break;
            } else {
                ms_make_move(game, x, y);
            }
        } else if (action == 'F') {
            ms_flag_square(game, x, y);
        } else {
            printf("Bad input, try again");
        }
        printf("\n");

        if (ms_check_win(game)) {
            printf("You won!\n");
            break;
        }
    }
    
    ms_print_all(game);
    
    ms_destroy(game);
    return 0;
}

