#include <stdio.h>

void call_menu(char *bs, char *es, char *ts, char *fs) {
  printf(" <<< Applications Menu >>> \n 1) Web Browser %s\n 2) Text Editor %s\n 3) Terminal %s\n 4) Finalizar processo %s\n 5)quit \n", bs, es, ts, fs);
}
int main() {
 
 unsigned char option = 0;
  
 while(option != 5) {
    call_menu("","","","");
    scanf("%c", option);
  
    switch(option) {
     case 1:
       break;
    
     case 2:
      break;
      
     case 3:
      break;
      
     case 4:
      break;
      
     case 5:
      break;
      
      defaut:
      break;
    }
 } 
}
