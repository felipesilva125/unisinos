#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

//Nome da fila
const char* queue_name = "/queue_name1";

//Estrutura de dados para a mensagem
typedef struct Message {
	char* name;
	int x;
	int y;
} TMessage;

int main(int argc, char* argv[]) {

	if (argc != 3)
	{
		printf("Valor inválido de parâmetros.\n");
		return 0;
	}	
	
	//Declaração da fila
	mqd_t queue;
	//Declaração da mensagem
	TMessage m;
	//Texto a ser enviado na mensagem - entrada por linha de comando
	char* name = argv[0];
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);	

	if (x < 0 || x > 2)
	{
		printf("O valor da primeira coordenada deve estar entre 0 e 2.\n");
		return 0;
	}

	if (y < 0 || y > 2)
	{
		printf("O valor da segunda coordenada deve estar entre 0 e 2.\n");
		return 0;
	}

	//Obter descritor (mq_open+O_WRONLY+O_CREAT)
	queue = mq_open(queue_name, O_WRONLY | O_CREAT, 0770, NULL);
	if (queue == (mqd_t) -1) {
		perror("mq_open");
		exit(2);
	}

	//Montar a mensagem	
	m.name = name;
	m.x = x;
	m.y = y;

	//Enviar (mq_send)
	if (mq_send(queue, (const char*) &m, sizeof(TMessage), 29) != 0) {
		perror("send #29");
	}

	//Liberar descritor (mq_close)
	mq_close(queue);

	printf("Jogada executada com sucesso!\n");
	exit(EXIT_SUCCESS);
}
