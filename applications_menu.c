#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

pid_t bs;
pid_t es;
pid_t ts;
char bs_status[15];
char es_status[15];
char ts_status[15];

void set_status(int process, char *status){	
	if(process == 1)	
		strncpy(bs_status, status, 15);

	if(process == 2)	
		strncpy(es_status, status, 15);

	if(process == 3)	
		strncpy(ts_status, status, 15);

}

void call_menu() {
  printf(" <<< Applications Menu >>>\n1) Web Browser %s %d\n2) Text Editor %s %d\n3) Terminal %s %d\n4) Finalizar processo\n5) Quit\n", bs_status, bs, es_status, es, ts_status, ts);
}
int main() {
 
 short int option = 0;
 call_menu();
 
 while(option != 5) {
    
    scanf("%hd", &option);
    int n = 0;


    switch(option) {
     case 1:
		printf("Which URL would you like to open?\n");
		char url[256];
		scanf("%s", url);

	if((bs=fork()) < 0){
		printf("Couldn't create new process to call the browser");
		set_status(1, "- Falhou");	
	}
	else if(bs == 0) {	
		set_status(1, "- Executando");	
		call_menu();
		execlp("/usr/lib/firefox/firefox", "firefox", "--new-window", url, NULL);
		sleep(0.1);
		bs = getpid();	
				
	}
	break;
    
     case 2:
	if((es=fork()) < 0){
		printf("Couldn't create new process to call gedit");	
		set_status(2, "- Falhou");
	}
	else if(es == 0) {
		es = getpid();	
		set_status(2, "- Executando");	
		call_menu();
		sleep(0.1);	
		execlp("gedit","gedit", NULL);		
	}
      break;
      
     case 3:
	if((ts=fork()) < 0){
			printf("Couldn't create new process to call the browser");	
			set_status(3, "- Falhou");
		}
	else if(ts == 0) {	
			set_status(3, "- Executando");	
			ts = getpid();
			call_menu();	
			sleep(0.1);
			execlp("gnome-terminal","gnome-terminal", NULL);			
		}
      break;
      
     case 4:
			printf("Selecione um processo para fechar:\n");
			printf("1) Web Browser\n");
			printf("2) Text Editor\n");
			printf("3) Terminal:\n");	
			scanf("%d", &n);

			switch(n)
			{
				case 1:
				   set_status(1, "- Abortado");
				   printf("Processo Web Browser finalizado\n");
				   kill(bs, SIGKILL);
			  	  		
				   break;
		
				case 2:
				   set_status(2, "- Abortado");
				   printf("Processo Editor finalizado\n");
				   kill(es, SIGKILL);			   					
				   break;

				case 3:
				   set_status(3, "- Abortado");
				   printf("Processo Terminal finalizado\n");
				   kill(ts, SIGKILL);								
				   break;

				default:
				   printf("Opção Inválida.\n");
				   break;
			}
	call_menu();
      break;

     case 5:	
	printf("Exiting application.\n");
	break;

     default:
	printf("Invalid option, please choose from the menu.\n");
	break;
    }
 }
return 0; 
}
