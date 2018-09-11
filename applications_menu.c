#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void call_menu(pid_t bs, pid_t es, pid_t ts, pid_t fs) {
  printf(" <<< Applications Menu >>>\n1) Web Browser %d\n2) Text Editor %d\n3) Terminal %d\n4) Finalizar processo %d\n5) Quit\n", bs, es, ts, fs);
}
int main() {
 
 short int option = 0;
 call_menu(0,0,0,0);
 
 while(option != 5) {
    
    scanf("%hd", &option);
    pid_t pid;
    pid_t bs;
    pid_t es;
    pid_t ts = 0;
    pid_t fs = 0;

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
		call_menu(bs,es,ts,fs);
		sleep(0.1);	
		execlp("/usr/lib/firefox/firefox", "firefox", "--new-window", url, NULL);		
	}
	break;
    
     case 2:
	if((pid=fork()) < 0){
		printf("Couldn't create new process to call the browser");	
	}
	else if(pid == 0) {		
		es = getpid();
		call_menu(bs,es,ts,fs);
		sleep(0.1);	
		execlp("gedit","gedit", NULL);		
	}
      break;
      
     case 3:
      break;
      
     case 4:
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
