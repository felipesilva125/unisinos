#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

pid_t bs;
pid_t es;
pid_t ts;
pid_t fs = 0;

void call_menu() {
  printf(" <<< Applications Menu >>>\n1) Web Browser %d\n2) Text Editor %d\n3) Terminal %d\n4) Finalizar processo %d\n5) Quit\n", bs, es, ts, fs);
}
int main() {
 
	 short int option = 0;
	 int n = 0;
	 pid_t pid;	 
	 call_menu(bs,es,ts,fs);
	 
	 while(option != 5) {
	    
	    scanf("%hd", &option);    
	    switch(option) {
	     case 1:
			printf("Which URL would you like to open?\n");
			char url[256];
			scanf("%s", url);

		if((pid=fork()) < 0){
			printf("Couldn't create new process to call the browser");	
		}
		else if(pid == 0) {		
			bs = getpid();
			call_menu();
			sleep(0.1);	
			execlp("/usr/lib/firefox/firefox", "firefox", "--new-window", url, NULL);		
		}
		break;
	    
	     case 2:
		if((pid1=fork()) < 0){
			printf("Couldn't create new process to call the browser");	
		}
		else if(pid1 == 0) {		
			es = getpid();
			call_menu();
			sleep(0.1);	
			execlp("gedit","gedit", NULL);		
		}
	      break;
	      
	     case 3:
		if((pid2=fork()) < 0){
			printf("Couldn't create new process to call the browser");	
		}
		else if(pid2 == 0) {		
			ts = getpid();
			call_menu(bs,es,ts,fs);
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
				   if (kill(bs, SIGTERM) == 0)
			  	   {
					printf("Processo Web Browser finalizado");
				   }			
				   break;
		
				case 2:
				   if (kill(es, SIGTERM) == 0)
			  	   {
					printf("Processo Text Editor finalizado");
				   }						
				   break;

				case 3:
				   if (kill(ts, SIGTERM) == 0)
			  	   {
					printf("Processo Terminal finalizado");
				   }									
				   break;

				default:
				   printf("Opção Inválida.\n");
				   break;
			}
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
