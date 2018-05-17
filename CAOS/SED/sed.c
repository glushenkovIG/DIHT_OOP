#include <stdio.h>
#include <stdlib.h>

int check_pattern(char* pattern, 

void find_and_replace(char* pattern, char* replace, char* file_name){
  int curr_symb;
  FILE* file = fopen(file_name, "r");
  while((curr_symb = fgetc(file)) != EOF){
    if(curr_symb == pattern[0]){
	while(
    printf("%c | ", curr_symb);
  }
}

int main(int argc, char** argv){
  if(argc < 3){
    perror("Too few arguments, requires at least 3");
  }
  char* pattern = argv[1];
  char* replace = argv[2];
  char file_name;
  
  int i = 3;
  while (i < argc){
    find_and_replace(argv[i]);
    i++;
  }  
}
