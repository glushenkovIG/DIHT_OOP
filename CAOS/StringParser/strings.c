//
//  main.c
//  strings_v2
//
//  Created by Ivan on 13.03.18.
//  Copyright © 2018 Ivan. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <ctype.h>

int myComp(const void* a, const void* b){
    return strcmp(*(char* const*) a, *(char* const*) b);
}

int isNull(char* a){
    if(a == NULL){
        perror("-----Null_ptr-----");
    }
    return 1;
}

void Push(int curr_symb, char** curr_word, int i, int* size){
    if(*size <= i + 1){
        *size *= 2;
        isNull(*curr_word);
        *curr_word = realloc(*curr_word, *size * sizeof(char));
    }
    (*curr_word)[i] = curr_symb;
}

int isLexem(int a){
    return a == '|' || a == '&' || a == ';';
}

void addNewWord(char*** words, int* words_size, int* words_it, char* curr_word, int* size){
    if(*words_size <= *words_it + 1){
        *words_size *= 2;
        *words = realloc(*words, *words_size * sizeof(char*));
    }
    (*words)[*words_it] = calloc(*size, sizeof(char));
    memcpy((*words)[*words_it], curr_word, *size);
    (*words_it)++;
//    for(int i = 0; i < *words_it; i++){
//        printf("\"%s\"\n", (*words)[i]);
//    }
}

int isQuotes(int a){
    if(a == '\'' || a == '"'){
        return a;
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    int words_size = 1;//размер массива слов
    char** words = calloc(words_size, sizeof(char*));//массив слов
    int words_it = 0;//итератор по массиву слов
    
    
    char* curr_word = calloc(2, sizeof(char));//текущее слово
    int i = 0;//итератор по текущему слову
    int size = 2;//размер текущего слова
    
    int yet_word = 0;//флаг нового слова
    int in_quotes = 0;//флаг нахождения в кавычках
    int already_pair = 0;//флаг что парную лексему уже добавили
    
    int prev_symb = 0;// предыдущий символ
    int curr_symb = 0;//текущий символ
    char k = 0;
    
    while((curr_symb = getchar()) != EOF){
        k = curr_symb;
        if(prev_symb != curr_symb){
            already_pair = 0;
        }
        if(in_quotes == 0){
            if(isspace(curr_symb)){
                if(yet_word){
                    Push(0, &curr_word, i, &size);
                    addNewWord(&words, &words_size, &words_it, curr_word, &size);
                    i = 0;
                    yet_word = 0;
                }
            } else {
                if(isLexem(curr_symb)){
                    if((prev_symb == curr_symb) && !already_pair){
                        int c = 2;
                        words_it--;
                        Push(curr_symb, &words[words_it], 1, &c);
                        Push(0, &words[words_it], 2, &c);
                        words_it++;
                        already_pair = 1;
                        yet_word = 0;
                    } else {
                        if(yet_word){
                            Push(0, &curr_word, i, &size);
                            addNewWord(&words, &words_size, &words_it, curr_word, &size);
                        }
                        Push(curr_symb, &curr_word, 0, &size);
                        Push(0, &curr_word, 1, &size);
                        addNewWord(&words, &words_size, &words_it, curr_word, &size);
                        i = 0;
                        yet_word = 0;
                        already_pair = 0;
                    }
                } else if(isQuotes(curr_symb)){
                    in_quotes = curr_symb;
                    yet_word = 1;
                } else {
                    if(!yet_word){
                        i = 0;
                        yet_word = 1;
                    }
                    Push(curr_symb, &curr_word, i, &size);
                    i++;
                }
            }
        } else if(!(curr_symb == in_quotes)){
                Push(curr_symb, &curr_word, i, &size);
                i++;
        } else {
            in_quotes = 0;
        }
    prev_symb = curr_symb;
    }
    if(in_quotes){
        perror("----Not pair quotes----");
        exit(10);
    }
    if(yet_word){
        Push(0, &curr_word, i, &size);
        addNewWord(&words, &words_size, &words_it, curr_word, &size);
    }
    qsort(words, words_it, sizeof(char*), myComp);
    for(int i = 0; i < words_it; i++){
        printf("\"%s\"\n", words[i]);
    }
    return 0;
}
