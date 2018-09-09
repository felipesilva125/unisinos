#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void call_menu(char *bs, char *es, char *ts, char *fs) {
  printf(" <<< Applications Menu >>> \n 1) Web Browser %s\n 2) Text Editor %s\n 3) Terminal %s\n 4) Finalizar processo %s\n 5) Quit \n", bs, es, ts, fs);
}
int main() {
 
 short int option = 0;
 call_menu("","","","");
 
 while(option != 5) {
    
    scanf("%hd", &option);
    pid_t pid;

    switch(option) {
     case 1:
	if((pid=fork()) < 0){
		printf("Couldn't create new process to call the browser");	
	}
	else if(pid == 0) {
		execlp("/usr/lib/firefox/firefox", "firefox", "--new-window", NULL);
		// Gravar o PID do fork em uma variável e passar no call_menu()
	}

	break;
	
       break;
    
     case 2:
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
