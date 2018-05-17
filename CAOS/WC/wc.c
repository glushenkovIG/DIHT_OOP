#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void get_inf_f(char* file_name, int* a, int* b, int* c){
  FILE* file = fopen(file_name, "r");
  int str_count = 0;
  int words_count = 0;
  int symb_count = 0;
  int prev_symb = 0;
  int isWord = 0;
  int curr_symb = 0;
  while((curr_symb = fgetc(file)) != EOF){
    symb_count++;
    if(isspace(curr_symb) && !isspace(prev_symb)){
      words_count++;
      isWord = 0;
    }
    isWord = 1;
    if(curr_symb == '\n'){
      str_count++;
    }
    prev_symb = curr_symb;
  }
  if(isWord){
    words_count++;
  }
  *a += str_count;
  *b += words_count;
  *c += symb_count;
  printf("%d  %d  %d  %s\n", str_count, words_count, symb_count, file_name);
}


void get_inf(){
  int str_count = 0;
  int words_count = 0;
  int symb_count = 0;
  int prev_symb = 0;
  int isWord = 0;
  int curr_symb = 0;
  while((curr_symb = getchar()) != EOF){
    symb_count++;
    if(isspace(curr_symb) && !isspace(prev_symb)){
      words_count++;
      isWord = 0;
    }
    isWord = 1;
    if(curr_symb == '\n'){
      str_count++;
    }
    prev_symb = curr_symb;
  }
  if(isWord){
    words_count++;
  }
  printf("\n %d  %d  %d    input\n", str_count, words_count, symb_count);
}

int main(int argc, char** argv){
  if(argc == 1){
    get_inf();
    return 0;
  }
  int i = 1;
  int str_count = 0;
  int words_count = 0;
  int symb_count = 0;
  struct stat* stat1;
  while (i < argc){
    struct stat stat2;
    int a = stat(argv[i], &stat2);
    if (a != -1 && S_ISREG(stat2.st_mode)) {
      get_inf_f(argv[i], &str_count, &words_count, &symb_count);
    } else {
      printf(" '%s' is NOT a file \n", argv[i]);
    }
    i++;
  }
  printf("%d  %d  %d    total \n", str_count, words_count, symb_count);
  return 0;
}

