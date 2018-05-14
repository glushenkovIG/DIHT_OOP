#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int myComp(const void *a, const void *b) {
    return strcmp(*(char *const *) a, *(char *const *) b);
}

void *create_shared_memory(size_t size) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(NULL, size, protection, visibility, 0, 0);
}

void sortPart(int left, int right, char **lines) {
    pid_t pid = fork();
    if (!pid){
        qsort(lines + left, right - left, sizeof(char *), myComp);
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
        while(strcmp(lines[part_it], sorted_parts[sorted_parts_it]) <= 0 && part_it < to){
            new_sorted_parts[new_sorted_parts_it++] = lines[part_it++];
        }
        new_sorted_parts[new_sorted_parts_it++] = sorted_parts[sorted_parts_it++];
    }
    
    if(sorted_parts_it == from){
        //остаток новой части скопировать в конец new_sorted_parts
        memcpy(new_sorted_parts + new_sorted_parts_it, lines + part_it, (to - part_it) * sizeof(char*));
    } else {
        //остаток отсортированных частей копируем в конец new_sorted_parts
        memcpy(new_sorted_parts + new_sorted_parts_it, sorted_parts + sorted_parts_it, (from - sorted_parts_it) * sizeof(char*));
    }
    free(sorted_parts);
    return new_sorted_parts;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Amount of arguments should be 2, %d given.", argc - 1);
        exit(2);
    }
    char *name_of_file = argv[1];
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
    in = fopen(name_of_file, "r");
    int i = 0;
    while((symb_amount = getline(&lines[i], &(lines_sizes[i]), in)) != -1){
        i++;
    }
    
    for (int from = 0, to = lines_amount % son_amount + lines_amount / son_amount - 1;
         to < lines_amount; from = to, to += lines_amount / son_amount) {
        sortPart(from, to, lines);
    }
    
    char** sorted_parts = calloc(lines_amount, sizeof(char*));
    for (int from = 0, to = lines_amount % son_amount + lines_amount / son_amount - 1;
         to < lines_amount; from = to, to += lines_amount / son_amount) {
        wait(0);
        sorted_parts = MergeSortedPart(from, to, lines, sorted_parts, lines_amount);
    }
    
    for (int i = 0; i < lines_amount - 1; i++) {
        fprintf(out, "%s", sorted_parts[i]);
    }
    return 0;
}
