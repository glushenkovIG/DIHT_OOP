#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int myComp(const void *a, const void *b) {
  return strcmp(*(char *const *) a, *(char *const *) b);
}

void *create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  return mmap(NULL, size, protection, visibility, 0, 0);
}

void FatherExitHandler(int a) {
  printf("\n father->son %d \n", getpid());
  exit(10);
}

void SonExitHandler(int a) {
  printf("\n son->father %d \n", getpid());
  exit(10);
}

void sortPart(int left, int right, char **lines) {
  pid_t pid = fork();
  if (!pid){
    qsort(lines + left, right - left, sizeof(char *), myComp);
    puts("Sorted");
    exit(10);
  }
}

int isNull(char* a){
    if(a == NULL){
        perror("-----Null_ptr-----");
    }
    return 1;
}

void Push(char* curr_line, char** lines, int i, int* size){
    if(*size <= i + 1){
        *size *= 2;
        isNull(*lines);
        // изменять размер сменного массива!!!!!
        *lines = realloc(*lines, *size * sizeof(char*));
    }
    (*lines)[i] = curr_line;
}

char** MergeSortedPart(int from, int to, char** lines, char** sorted_parts, int lines_amount){
  int sorted_parts_it = 0;
  int part_it = from;
  int new_sorted_parts_it = 0;
  char** new_sorted_parts = calloc(lines_amount, sizeof(char*));

  while(sorted_parts_it < from && part_it < to){
    //printf("lines[part_it] = \nsorted_parts[sorted_parts_it]");
    while(strcmp(lines[part_it], sorted_parts[sorted_parts_it]) <= 0 && part_it < to){
      //printf("new_sorted_parts1[%d] = %s", new_sorted_parts_it, lines[part_it]);
      new_sorted_parts[new_sorted_parts_it++] = lines[part_it++];
    }
    //printf("part_it = %d\n", part_it);
    //printf("new_sorted_parts2[%d] = %s", new_sorted_parts_it, sorted_parts[sorted_parts_it]);
    new_sorted_parts[new_sorted_parts_it++] = sorted_parts[sorted_parts_it++];
  }

  if(sorted_parts_it == from){
    //остаток новой части скопировать в конец new_sorted_parts
    //puts("    //остаток новой части скопировать в конец new_sorted_parts\n");

    memcpy(new_sorted_parts + new_sorted_parts_it, lines + part_it, (to - part_it) * sizeof(char*));
  } else {
    //puts("    //остаток отсортированных частей копируем в конец new_sorted_parts\n");
    //остаток отсортированных частей копируем в конец new_sorted_parts
    memcpy(new_sorted_parts + new_sorted_parts_it, sorted_parts + sorted_parts_it, (from - sorted_parts_it) * sizeof(char*));
  }
  free(sorted_parts);
  /*
  printf("==========new_sorted_parts===========\n");
  for(int i = 0; i < to; i++){
    printf("%s", new_sorted_parts[i]);
  }
  */
  return new_sorted_parts;
}

int main(int argc, char **argv) {
  char **test = calloc(10, sizeof(char *));
  for (char i = 0; i < 10; i++) {
    test[i] = calloc(10, sizeof(char));
    test[i][0] = 'a' + (i * i) % 10;
    //printf("%s\n", test[i]);
  }
  qsort(test, 10, sizeof(char *), myComp);
  for (char i = 0; i < 10; i++) {
    //printf("%s\n", test[i]);
  }
  if (argc != 3) {
    printf("Amount of arguments should be 2, %d given.", argc - 1);
    exit(2);
  }
  char *name_of_file = argv[1];
  printf("name_of_file %s\n", name_of_file);
  int son_amount = atoll(argv[2]);

  FILE *in = fopen(name_of_file, "r");
  if (in == NULL) {
    perror("input file error");
  }

  FILE *out = fopen("output.txt", "w");
  if (out == NULL) {
    perror("output file error");
  }

  size_t lines_amount = 1;
  int curr_symb;
  while ((curr_symb = getc(in)) != EOF) {
    if (curr_symb == '\n') {
      lines_amount++;
    }
  }

  char **lines = create_shared_memory(lines_amount * sizeof(char *));
  for (int i = 0; i < lines_amount; i++) {
    lines[i] = calloc(1, sizeof(char *));
  }

  size_t *lines_sizes = calloc(lines_amount, sizeof(size_t));
  int symb_amount = 0;
  in = fopen("input.txt", "r");
  for (int i = 0; (symb_amount = getline(&lines[i], &(lines_sizes[i]), in)) != -1; i++) {
    printf("%s", lines[i]);
  }
  printf("son_amount = %d\nlines_amount %d\n from = 0 \n to = %d\n lines_amount/son_amount = %d\n", son_amount,
         lines_amount, lines_amount % son_amount + lines_amount / son_amount - 1, lines_amount / son_amount);

  for (int from = 0, to = lines_amount % son_amount + lines_amount / son_amount - 1;
       to < lines_amount; from = to, to += lines_amount / son_amount) {
    printf("sort [%d, %d]\n", from, to);
    sortPart(from, to, lines);
  }

  for(int i = 0; i < son_amount; i++){
    wait(0);
  }
/*
  printf("sorted Before Merge\n");
  for (int i = 0; i < lines_amount; i++) {
    printf("%s", lines[i]);
  }
  //exit(12);

  char** sorted_parts = calloc(lines_amount, sizeof(char*));
  int j = 1;
  for (int from = 0, to = lines_amount % son_amount + lines_amount / son_amount - 1;
       to < lines_amount; from = to, to += lines_amount / son_amount) {
    sorted_parts = MergeSortedPart(from, to, lines, sorted_parts, lines_amount);
/*
    printf("sorted after %d Merge\n", j);
    j++;
    for (int i = 0; i < to; i++) {
      printf("%s", sorted_parts[i]);
    }
    */
  }


  printf("Sorted\n");
  for (int i = 0; i < lines_amount - 1; i++) {
    printf("%s", sorted_parts[i]);
  }



  return 0;
}
