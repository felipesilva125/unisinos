#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define SIZE 3
#define FILE_MAX_LENGTH 512

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

char* getActualDateAndTime()
{
    char* result = "";    
    char date[80];
    time_t mytime;
    struct tm* info;
    mytime = time(&mytime);
    info = localtime(&mytime);
    strftime(date, 80, "%x - %X", info);    
    sprintf(result, "%s", date);
    return result;
}

int write_log(char texto[FILE_MAX_LENGTH])
{   	 
	char *nome = "log.txt";
	int fd = open(nome, O_RDWR | O_CREAT, S_IRGRP | S_IWGRP | S_IWUSR | S_IRUSR | S_IROTH | S_IWOTH);
	if (fd < 0){
		perror("\nErro na escrita.\n");
		return -1;
	}

	int length = strlen(texto);

	lseek(fd, 0, SEEK_END);

	int status = write(fd, texto, length);

	if (status < 0){
		perror("\nFalha na escrita do arquivo.\n");
		return -1;
	}

	close(fd);
	return fd;
}

void make_play(TMessage* m) {

    char texto[FILE_MAX_LENGTH] = "";
    char *date = "";
    sprintf(texto, "Jogada feita pelo player %d! Coordenadas: %d e %d. %s", m->player, m->x, m->y, date);   	 
    if(m->player == 1) {
   	 if (board[m->x][m->y] == '-'){   	 
   		 //strcpy(date, getActualDateAndTime());
   		 strcat(texto, "Jogada válida.\n");   	 
   		 board[m->x][m->y] = 'X';   	 
   		 write_log(texto);
   	 }
   	 else{
   		 strcat(texto, "Jogada inválida.\n");    
   		 write_log(texto);
   		 printf("Field already filled.\n");
   	 }   	 
    }  

    if(m->player == 2){
   	 if (board[m->x][m->y] == '-'){   					 
   		 strcat(texto, "Jogada válida.\n");   				 
   		 board[m->x][m->y] = 'O';
   		 write_log(texto);
   	 }
   	 else{
   		 strcat(texto, "Jogada inválida.\n");    
   		 write_log(texto);
   		 printf("Field already filled.\n");
   	 }
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

int check_player(char mark){

	if(mark == 'X'){
    		return 1;
	}
	else{
    		return 2;
	}
}

bool check_win() {

	char texto[FILE_MAX_LENGTH] = "";
	char *date = "";    

	//Verify if any wins on all the board lines.
	for(int i = 0; i < SIZE; i++) {
		if(board[i][0] != '-' && board[i][0] == board[i][1] && board[i][1] == board[i][2]){
			int player = check_player(board[i][0]);
			sprintf(texto, "Última jogada feita pelo player %d! Jogada vencedora.", player);    
			write_log(texto);
			printf("The winner filled the %d line! Player %d wins!\n", i, player);
			return true;
		}
	}

	//Verify if any wins on all the board columns.
	for(int i = 0; i < SIZE; i++) {
		if(board[0][i] != '-' && board[0][i] == board[1][i] && board[0][i] == board[2][i]){
			int player = check_player(board[0][i]);
			sprintf(texto, "Última jogada feita pelo player %d! Jogada vencedora.", player);
			write_log(texto);     
			printf("The winner filled the %d column! Player %d wins!\n", i, player);
			return true;
		}
	}   

	//Verify main diagonal
	if(board[0][0] != '-' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
		int player = check_player(board[1][1]);
		sprintf(texto, "Última jogada feita pelo player %d! Jogada vencedora.", player);    
		write_log(texto);
		printf("The winner filled the main diagonal! Player %d wins!\n", player);
		return true;
	}

	//Verify secondary diagonal
	if(board[0][2] != '-' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
    		int player = check_player(board[1][1]);
		sprintf(texto, "Última jogada feita pelo player %d! Jogada vencedora.", player);
		write_log(texto);     
		printf("The winner filled the secondary diagonal! Player %d wins!\n", player);
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
    puts("Welcome to the tic-tac-toe game!       	##\nOne side will be X and the other O.    	##\nType in the board position to make a play. ##");
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

    //Verifica se algum jogador ganhou o jogo
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


