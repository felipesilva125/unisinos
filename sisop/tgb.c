#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

#define SIZE 3
const char* queue_name = "/queue_name1";
char board[SIZE][SIZE];

typedef struct Message {
	int player;
	int x;
	int y;
	
} TMessage;

ssize_t get_msg_buffer_size(mqd_t queue);
void make_play(TMessage* m);

// This function prints the ttt board in it's current state.
void print_board() {

	for(int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%c ", board[i][j]);
   		}
    		printf("\n");
	}
}

void make_play(TMessage* m) {
	if(m->player == 1) {
		board[m->x][m->y] = 'X';
	}
	
	if(m->player == 2){
		board[m->x][m->y] = 'O';
	}
	
	print_board();
}

ssize_t get_msg_buffer_size(mqd_t queue) {
	struct mq_attr attr;

	/*Determina max. msg size; allocate buffer to receive msg */
	if (mq_getattr(queue, &attr) != -1) {
		printf("max msg size: %ld\n", attr.mq_msgsize);
		return attr.mq_msgsize;
	}

	perror("aloca_msg_buffer");
	exit(3);
}

void check_player(char mark){
	
	if(mark == 'X')
		puts("Player 1 wins!");
	else
		puts("Player 2 wins!");
}

bool check_win() {
	//Verify if any wins on all the board lines.
	for(int i = 0; i < SIZE; i++) {
		if(board[i][0] != '-' && board[i][0] == board[i][1] && board[i][1] == board[i][2]){
			check_player(board[i][0]);
			printf("The winner filled the %d line!", i);
			return true;
		}
	}
	
	//Verify if any wins on all the board columns.
	for(int i = 0; i < SIZE; i++) {
		if(board[0][i] != '-' && board[0][i] == board[1][i] && board[0][i] == board[2][i]){
			check_player(board[0][i]);
			printf("The winner filled the %d column!", i);
			return true;
		}
	}
	
	//Verify main diagonal
	if(board[0][0] != '-' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
		check_player(board[1][1]);
		puts("The winner filled the main diagonal!");
		return true;
	}

	//Verify secondary diagonal
	if(board[0][2] != '-' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
		check_player(board[1][1]);
		puts("The winner filled the secondary diagonal!");
		return true;
	}

	return false;
}

// The game logic and flow is written in this function.
void initialize_board(){
	//Complete board with dash character.
	for(int i = 0; i < SIZE; i++) {
		for(int j = 0; j < SIZE; j++) {
			board[i][j] = '-';	
		}
	}
	print_board();
}

int main(){

// Introduction to the game.
puts("\n#############################################");
puts("Welcome to the tic-tac-toe game!           ##\nOne side will be X and the other O.        ##\nType in the board position to make a play. ##");
puts("#############################################\n");

initialize_board();

//Declaração da fila
mqd_t queue;

//Declaração do buffer
char* buffer = NULL;

//Declaração do tamanho do buffer
ssize_t tam_buffer;
ssize_t nbytes;

//Obter descritor (mq_open+O_RDONLY)
queue = mq_open(queue_name, O_RDONLY | O_CREAT, 0660, NULL);

if (queue == (mqd_t) -1) {
	perror("mq_open");
	exit(2);
}

while(check_win() == false) {

	//Alocar buffer para receber msg
	tam_buffer = get_msg_buffer_size(queue);
	buffer = calloc(tam_buffer, 1);

	//Receber (mq_recv)
	nbytes = mq_receive(queue, buffer, tam_buffer, NULL);
	if (nbytes == -1) {
		perror("receive");
		exit(4);
	}	

	//Print da mensagem recebida

	make_play((TMessage*) buffer);
	
}

//Liberar descritor (mq_close)
mq_close(queue);
printf("Fim!\n");

return 0;
}
