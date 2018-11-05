#include <stdio.h>
#include <stdbool.h>
#define SIZE 3

// This function prints the ttt board in it's current state.
void print_board(char board[SIZE][SIZE]) {

	for(int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%c ", board[i][j]);
   		}
    		printf("\n");
	}
}

bool check_win(char board[SIZE][SIZE]) {
	//Verify if any wins on all the board lines.
	for(int i = 0; i < SIZE; i++) {
		if(board[i][0] != '-' && board[i][0] == board[i][1] && board[i][0] == board[i][3]){
			return true;
		}
	}
	
	//Verify if any wins on all the board columns.
	for(int i = 0; i < SIZE; i++) {
		if(board[i][0] != '-' && board[0][i] == board[1][i] && board[0][i] == board[3][i]){
			return true;
		}
	}
return false;
}

// The game logic and flow is written in this function.
void start_game(){
	char board[SIZE][SIZE];
	bool end = false;
	
	//Complete board with dash character.
	for(int i = 0; i < SIZE; i++) {
		for(int j = 0; j < SIZE; j++) {
			board[i][j] = '-';	
		}
	}

	while(!end){
		print_board(board);
		end = check_win(board);
	}
}

int main(){

// Introduction to the game.
puts("\n#############################################");
puts("Welcome to the tic-tac-toe game!           ##\nOne side will be X and the other O.        ##\nType in the board position to make a play. ##");
puts("#############################################\n");

start_game();

return 0;
}
