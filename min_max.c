/*
* Simple Implementation of the Tic Tac Toe game in C
* Compiler : Microsoft MSVC
* author: IcyRepublic9884
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// Definitions
#define CLS() (system("cls"))
#define PAUSE() (system("pause"))
#define SINGLE_PLAYER_MODE 1
#define TWO_PLAYER_MODE 2
#define OPPONENT 1
#define PLAYER 2

short board[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};
short PLAYER_ONE = 1;
short PLAYER_TWO = 2;
short *CURRENT_PLAYER = &PLAYER_ONE;

// Declarations
void color_print(const char *s, int color);
void display_board();
short is_free(short x, short y);
short move(short x, short y);
void switch_player();
void get_input(short *x, short *y);
short is_tie();
short has_won();
void show_welcome_message();
short get_game_mode();
void get_computer_choice(short *x, short *y);
int evaluate_board();
int min_max(int depth);
void find_best_move(short *x, short *y);

void color_print(const char *s, int color)
{
    /*
	Print colorized output to the console
	-----Color Codes-----
	0 = black 8 = gray
	1 = blue 9 = light blue
	2 = green 10 = light green
	3 = light green 11 = light green
	4 = red 12 = light red
	5 = purple 13 = light purple
	6 = yellow 14 = light yellow
	9 = white 15 = bright white */
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    printf(s);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}

void display_board()
{
    /* Display the board on to the console */
    color_print("+-----------+\n", 9);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == PLAYER_ONE)
            {
                color_print("|", 9);
                color_print(" X ", 12);
            }
            else if (board[i][j] == PLAYER_TWO)
            {
                color_print("|", 9);
                color_print(" O ", 10);
            }
            else
            {
                color_print("|   ", 9);
            }
        }
        color_print("|\n", 9);
    }
    color_print("+-----------+\n", 9);
}

short is_free(short x, short y)
{
    /* Check if a certain position on the game board is free for a certain player
	if the position is free, it will return 1, if not will return 0 */
    return (!board[x][y]) ? 1 : 0;
}

short move(short x, short y)
{
    /* Move the current player to the specified opsition on the board
	returns -1 if the position is already taken by another player */
    if (is_free(x, y) && x < 3 && y < 3 && x >= 0 && y >= 0)
    {
        board[x][y] = *CURRENT_PLAYER;
        return 0;
    }
    return -1;
}

void switch_player()
{
    if (CURRENT_PLAYER == &PLAYER_ONE)
    {
        CURRENT_PLAYER = &PLAYER_TWO;
    }
    else if (CURRENT_PLAYER == &PLAYER_TWO)
    {
        CURRENT_PLAYER = &PLAYER_ONE;
    }
}

void get_input(short *x, short *y)
{
    /* Get 2 inputs from the user for X and Y and set them to the provided short values.
	prints whose turn it is to play and switches the Player */
    if (CURRENT_PLAYER == &PLAYER_ONE)
    {
        printf("Player One's Turn, \n");
    }
    else
    {
        printf("Player Two's Turn, \n");
    }
    printf("Enter x : ");
    scanf_s("\n%hi", x);
    printf("Enter y : ");
    scanf_s("\n%hi", y);
    *x -= 1;
    *y -= 1;
}

short is_tie()
{
    /* Check if a tie situation has come to `play` */
    for (short row = 0; row < 3; row++)
    {
        for (short i = 0; i < 3; i++)
        {
            if (!board[row][i])
            {
                return 0;
            }
        }
    }
    return 1;
}

short has_won()
{
    /* Check the game for any winners */
    // Go through 1 and 2, the players
    for (short player = 1; player < 3; player++)
    {
        // Search the rows for winners
        for (short row = 0; row < 3; row++)
        {
            if (board[row][0] == player && board[row][1] == player && board[row][2] == player)
            {
                return player;
            }
        }
        // search each column
        for (short col = 0; col < 4; col++)
        {
            if (board[0][col] == player && board[1][col] == player && board[2][col] == player)
            {
                return player;
            }
        }
        // Search the cross sections
        if (board[1][1] == player)
        {
            if (board[0][0] == player && board[2][2] == player)
            {
                return player;
            }
            if (board[0][2] == player && board[2][0] == player)
            {
                return player;
            }
        }
    }
    return 0;
}

void show_welcome_message()
{
    /* A welcome message when the user starts the game, gives a little description of the game */
    color_print("Welcome to the Tic Tac Toe Game written in C\n", 2);
    printf("An ASCII board will be displayed in the console\n");
    printf("By default the first player will be Player One, represented on the screen by X\n");
    printf("Player Two will be represented by O\n");
    printf("You Will be prompted each time to enter a X and Y Coordinate\n");
    printf("X represents the row number and Y will represent the column\n");
    printf("\tFor Example, x = 2 and y = 2 will be the center opsition of the board.\n");
    printf("Happy Tic-Tac-Toe'ing\n");
    PAUSE();
    CLS();
}

short get_game_mode()
{
    /* Get the game mode from the user */
    printf("Enter Game Mode\n\t[1] Single player mode\n\t[2] Two Player Mode\n");
    printf(": ");
    short user_input = 0;
    scanf_s("\n%hi", &user_input);
    switch (user_input)
    {
    case SINGLE_PLAYER_MODE:
        return SINGLE_PLAYER_MODE;
    case TWO_PLAYER_MODE:
        return TWO_PLAYER_MODE;
    default:
        printf("Unrecognized Option, going with Defualt Single Player Mode\n");
        return SINGLE_PLAYER_MODE;
        break;
    }
}

int evaluate_board()
{
    /* Evaluate the board for the usage of the minmax function */
    switch (has_won())
    {
    case OPPONENT:
        return -10;
    case PLAYER:
        return 10;
    case 0:
        return 0;
    default:
        exit(EXIT_FAILURE);
    }
}

int min_max(int depth)
{
    /* considers all the possible ways the game can go and return the value of the board */
    int score = evaluate_board();
    if (score == 10 && score == -10)
        return score;
    if (is_tie())
        return 0;

    if (CURRENT_PLAYER == &PLAYER_TWO)
    {
        int best = -1000;
        for (short row = 0; row < 3; row++)
        {
            for (short col = 0; col < 3; col++)
            {
                if (is_free(row, col))
                {
                    move(row, col);
                    best = max(best, min_max(depth + 1));
                    board[row][col] = 0;
                }
            }
        }
        return best;
    }

    else
    {
        int best = 1000;
        for (short row = 0; row < 3; row++)
        {
            for (short col = 0; col < 3; col++)
            {
                if (is_free(row, col))
                {
                    move(row, col);
                    best = min(best, min_max(depth + 1));
                }
            }
        }
        return best;
    }
}

void find_best_move(short *x, short *y)
{
    /* Find the optimal move for the computer player, using the min_max algorithm and recursion */
    int bestVal = -1000;
    *x = -1;
    *y = 1;
    // move through every position of the board and try the spots
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (is_free(row, col))
            {
                move(row, col);
                int moveVal = min_max(0);
                board[row][col] = 0;

                if (moveVal > bestVal)
                {
                    *x = row;
                    *y = col;
                    bestVal = moveVal;
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    short winner = 0;
    short x = 0;
    short y = 0;

    show_welcome_message();
    short game_mode = get_game_mode();
    CLS();

    if (game_mode == TWO_PLAYER_MODE)
    {
        // Logic if the game mode is Two Player
        while (!winner)
        {
            display_board();

            if (is_tie())
            {
                break;
            }
            get_input(&x, &y);

            if (move(x, y))
            {
                CLS();
                color_print("Cannot Move there !. Try Again\n", 4);
                continue;
            }
            winner = has_won();
            switch_player();
            CLS();
        }

        display_board();
        if (winner == PLAYER_ONE)
        {
            color_print("Winner Is Player One !\n\tCongrats !\n\n\n", 4);
        }
        else if (winner == PLAYER_TWO)
        {
            color_print("Winner Is Player Two !\n\tCongrats !\n\n\n", 2);
        }
        else
        {
            color_print("It's A Tie !\n\n\n", 2);
        }
        PAUSE();
    }

    else
    {
        // Logic if the Mode is Single Player
        while (!winner)
        {
            display_board();

            if (is_tie())
            {
                break;
            }

            if (CURRENT_PLAYER == &PLAYER_ONE)
            {
                get_input(&x, &y);
            }
            else
            {
                find_best_move(&x, &y);
            }

            if (move(x, y))
            {
                CLS();
                if (CURRENT_PLAYER == &PLAYER_ONE)
                {
                    color_print("Cannot Move there !. Try Again\n", 4);
                }
                continue;
            }

            winner = has_won();
            switch_player();
            CLS();
        }

        display_board();
        if (winner == PLAYER_ONE)
        {
            color_print("You Won !\n\tCongrats !\n\n\n", 2);
        }
        else if (winner == PLAYER_TWO)
        {
            color_print("You Lost\n\tTry Again !\n\n\n", 4);
        }
        else
        {
            color_print("It's A Tie !\n\n\n", 2);
        }
        PAUSE();
    }
    return EXIT_SUCCESS;
}
