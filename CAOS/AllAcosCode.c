//MergeSort
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
  if (pid) {
    wait(0);
  } else {
    qsort(lines + left, right - left, sizeof(char *), myComp);
    puts("Sorted");
    exit(10);
  }
}

void MergeSortedPart(int from, int to, char**lines, char* sorted_parts, char** new_sorted_parts){
  int main_it = 0;
  int part_it = from;
  while(main_it < from && part_it < to){
    if(myComp(lines[main_it] < ))
  }
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

  printf("sorted\n");
  for (int i = 0; i < lines_amount; i++) {
    printf("%s", lines[i]);
  }



  return 0;
}

=========================================================================
//eratosphen

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

struct MyArguments{
    char** buf_;
    unsigned long long** primes_;
    unsigned long long a_;
    unsigned long long b_;
    unsigned long long* size_;
    unsigned long long i_;

    unsigned long long* prime_amount;
};

int isNull(char* a){
    if(a == NULL){
        perror("-----Null_ptr-----");
    }
    return 1;
}

void Push(unsigned long long curr_symb, unsigned long long** curr_word, unsigned long long i, unsigned long long* size){
    if(*size <= i + 1){
        *size *= 2;
        isNull(*curr_word);
        // изменять размер сменного массива!!!!!
        *curr_word = realloc(*curr_word, *size * sizeof(unsigned long long));
    }
    (*curr_word)[i] = curr_symb;
}

void* cross_not_primes(void* myArg){
    struct MyArguments* myArg_ = (struct MyArguments*) myArg;
    unsigned long long left = myArg_->a_;
    //printf("\n == cross [%d, %d] ===", myArg_->a_, myArg_->b_);
    unsigned long long prime_iterator = 0;
    unsigned long long num_buf_iterator = left;
    unsigned long long curr_prime = 0;

    while(prime_iterator < *myArg_->prime_amount){
        curr_prime = (*myArg_->primes_)[prime_iterator];

        num_buf_iterator = left + (curr_prime - left % curr_prime) % curr_prime;
        //printf("\n%d: ", curr_prime);

        while(num_buf_iterator <= myArg_->b_){
            (*myArg_->buf_)[num_buf_iterator] = 1;
            num_buf_iterator += curr_prime;
        }
        prime_iterator++;
    }
    //printf("\n");
    return NULL;
}

void* eratosphen(void* myArg){
    struct MyArguments* myArg_ = (struct MyArguments*) myArg;
    unsigned long long cross_it = myArg_->a_;
    for(unsigned long long i = myArg_->a_; i < myArg_->b_; i++){
        if((*myArg_->buf_)[i] == '\0' && i != 0 && i != 1){
            cross_it = i;
            unsigned long long p = i;
            //printf("This is new prime number: %d \n", p);
            *myArg_->prime_amount += 1;

            while(cross_it < myArg_->b_){
                cross_it += p;
                (*myArg_->buf_)[cross_it] = 1;
            }
        }
    }
    return NULL;
}

int main(int argc, char** argv){
    unsigned long long k = 10000LL;
    if(argc >= 2){
        k = atoll(argv[1]);
    }

    unsigned long long n = 10000000000LL;
    if(argc == 3) {
        n = atoll(argv[2]);
    }
    //printf("%d %llu %llu \n", argc, k, n);

    char* num_buf = calloc(n + 10, 1);
    if(num_buf == NULL){
        puts("primes initialization failed");
        exit(10);
    }

    //prime numbers buffer
    unsigned long long prime_size = 5;
    unsigned long long prime_amount = 0;
    unsigned long long sqrt_n = (unsigned long long) sqrt(n);
    unsigned long long* primes = calloc(prime_size, sizeof(unsigned long long));

    struct MyArguments arguments = {.buf_ = &num_buf, .primes_ = &primes, .a_ = 2, .b_ = sqrt_n,
        .prime_amount = &prime_amount};
    eratosphen(&arguments);

    unsigned long long local_prime_it = 0;
    unsigned long long i = 0;
    while(local_prime_it != prime_amount){
        if(num_buf[i] == 0 && i != 0 && i != 1){
            Push(i, &primes, local_prime_it, &prime_size);
            //printf("Added to primes %d: %d \n", local_prime_it, i);
            local_prime_it++;
        }
        i++;
    }

    pthread_t* threads = (pthread_t*) calloc(k, sizeof(pthread_t));
    if(threads == NULL){
        puts("threads == NULL");
        exit(10);
    }

    struct MyArguments* args_ = (struct MyArguments*) calloc(k, sizeof(struct MyArguments));
    if(args_ == NULL){
        puts("args_ == NULL");
        exit(10);
    }

    unsigned long long left = sqrt_n;
    unsigned long long thread_it = 0;
    unsigned long long default_part = (n - sqrt_n) / k;
    unsigned long long right = left + default_part + (n - sqrt_n) % k;
    while(left != n) {
        //printf("[ %llu, %llu ] \n", left, right);
        args_[thread_it] = (struct MyArguments){.buf_ = &num_buf, .primes_ = &primes, .a_ = left,
            .b_ = right, .prime_amount = &prime_amount};

        if(pthread_create(&threads[thread_it], NULL, cross_not_primes, &(args_[thread_it]))){
            puts("pthread_error");
        }
        thread_it++;
        if(left + default_part > n){
            left = n;
        } else {
            left = right;
            right += default_part;
        }
    }

    for(unsigned long long i = 0; i < k; i++){
        if(pthread_join(threads[i], NULL)){
            puts("pthread_join error");
        }
    }

    for(unsigned long long i = 2; i < n; i++){
        if(num_buf[i] == 0) {
            printf("%llu\n", i);
        }
    }
}
=========================================================================
//r_find

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

void r_find(char* dir, char* name, struct passwd* user_id){
  if(dir == NULL){
    perror("dir == NULL");
    return;
  }
  DIR* dir_ = opendir(dir);
  if(dir_ == NULL){
    return;
  }
  struct dirent* curr_dir;
  char* new_dir;

  while((curr_dir = readdir(dir_)) != NULL){

    new_dir = calloc(strlen(dir) + strlen(curr_dir->d_name) + 3, sizeof(char));

    sprintf(new_dir, "%s/%s", dir, curr_dir->d_name);
    if(new_dir == NULL){
      perror("new_dir == NULL \n");
      return;
    }
    struct stat this_dir_info;
    int a = lstat(new_dir, &this_dir_info);
    if(a == -1){
      perror("Obtaining information about this directory failed \n");
      return;
    }
    if(!strcmp(curr_dir->d_name, name) && this_dir_info.st_uid == user_id->pw_uid){
      printf("%s\n", new_dir);
    }

    char* b = ".";
    char* c = "..";

    if(curr_dir->d_type == DT_DIR && strcmp(curr_dir->d_name, b) && strcmp(curr_dir->d_name, c)){
      if(!access(new_dir, R_OK)){
        r_find(new_dir, name, user_id);
      } else {
        printf("-----------------------------------\n ACCESS TO %s IS DENIED FOR THIS USER! \n-----------------------------------\n", new_dir);
      }
    }
  }
  closedir(dir_);
}

int main(int argc, char** argv){
  char* where = argv[1];
  char* name = argv[2];
  char* user = argv[3];
  struct passwd* user_id = getpwnam(user);
  if(user_id == NULL){
    perror("Invalid user");
    exit(10);
  }

  r_find(where, name, user_id);
  return 0;
}
=========================================================================
//my_ls


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>


struct path_size {
    char* path_;
    long size_;
};


int myComp(const void* a, const void* b){
    if(((struct path_size*) a)->size_ < ((struct path_size*) b)->size_){
        return 1;
    } else if (((struct path_size*) a)->size_ > ((struct path_size*) b)->size_){
        return -1;
    }
    return 0;
}

int isNull(void* a){
    if(a == NULL){
        perror("-----Null_ptr-----");
    }
    return 1;
}


void Push(struct path_size file, struct path_size** files, size_t* i, size_t* size){
    if(*size <= *i + 1){
        *size *= 2;
        isNull(files);
        *files = realloc(*files, *size * sizeof(struct path_size));
    }
    (*files)[*i] = file;
    *i = *i + 1;
}

void my_ls(char* dir, struct path_size** files, size_t* i, size_t* elem_count){
    if(dir == NULL){
        perror("dir == NULL");
    }
    DIR* dir_ = opendir(dir);
    if(dir_ == NULL){
        return;
    }

    struct dirent* curr_dir;
    char* new_dir;

    while((curr_dir = readdir(dir_)) != NULL){

        new_dir = calloc(strlen(dir) + strlen(curr_dir->d_name) + 5, sizeof(char));

        sprintf(new_dir, "%s/%s", dir, curr_dir->d_name);
        struct stat stat1;
        int a = lstat(new_dir, &stat1);
        struct path_size new_file = {new_dir, stat1.st_size};

        if(stat1.st_size == -1){
            printf("st_size == -1 \n");
        }

        Push(new_file, files, i, elem_count);

        if(new_dir == NULL){
            perror("new_dir == NULL");
        }

        char* b = ".";
        char* c = "..";
        if(curr_dir->d_type == DT_DIR && strcmp(curr_dir->d_name, b) && strcmp(curr_dir->d_name, c)){
            my_ls(new_dir, files, i, elem_count);
        }
    }
    closedir(dir_);
}

int main(int argc, char** argv){
    char* where = argv[1];

    size_t elem_count = 10;
    size_t i = 0;
    struct path_size* files = calloc(elem_count, sizeof(struct path_size));

    my_ls(where, &files, &i, &elem_count);
    qsort(files, i, sizeof(struct path_size), myComp);
    for(int j = 0; j < i; j++){
        printf(" %lu %s \n", files[j].size_, files[j].path_);
    }
    return 0;
}
=========================================================================
//pipes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_fork(char* f, int r, int w, int c){
  pid_t p = fork();
  if(!p){
    if(r != 0){
      dup2(r, 0);
      close(r);
    }
    if(w != 1){
      dup2(w, 1);
      close(w);
    }
    if(c){
      close(c);
    }
    execlp(f, f, NULL);
    perror("execlp failed");
    exit(10);
  }
}


int main(int argc, char** argv){
  size_t i = 1;
  int p[2];

  int read_ = 0;
  int write_;
  int c = 0;

  while(i < argc - 1) {
    pipe(p);
    write_ = p[1];
    c = p[0];

    do_fork(argv[i], read_, write_, c);
    close(write_);
    close(read_);
    read_ = p[0];
    i++;
  }

  do_fork(argv[i], read_, 1, -1);
  close(read_);

  int j = 0;
  while(j < argc - 1){
    wait(0);
    j++;
  }

  return 0;
}
=========================================================================
//strings
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
=========================================================================
//wc
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
=========================================================================
=========================================================================
//examples
=========================================================================
//create_shared_memory
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char ** argv){
  	int len;
        if (argc<2){
		fprintf(stderr,"Usage: %s string\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	len=strlen(argv[1])+1;
	int fd = shm_open("mem", O_RDWR | O_CREAT, 0600);
	if (fd == -1){
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
	ftruncate(fd, len);
	char* mem = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == MAP_FAILED) {
		perror("mmap");
		close(fd);
		exit(EXIT_FAILURE);
	}
	strcpy(mem,argv[1]);
	if (munmap(mem, len) == -1) {
		perror("unmmap");
		close(fd);
		exit(EXIT_FAILURE);
	}
}
//seminars
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
//#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char ** argv){
  int len;

  int fd = shm_open("mem", O_RDWR, 0);
  if (fd == -1){
    perror("shm_read");
    exit(EXIT_FAILURE);
  }
  char* mem = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED) {
    perror("mmap");
    close(fd);
    exit(EXIT_FAILURE);
  }
  while(1);
  printf("%s \n", mem);
}
=========================================================================
//pthreads

#include <pthread.h>
#include <stdio.h>
int a = 0;
int threadsNum=10;

void * f( void *p ) {
    int k = *(int *)p;
    int temp = 0;
    for( int i = k ; i < threadsNum * 500000000; i += threadsNum ) {
        a++;
    }

    return NULL;
}

int main(int argc, const char * argv[]) {
    int *k = (int*)malloc(threadsNum * sizeof(int));
    for( int i = 0; i < threadsNum; ++i ) {
        k[i] = i;
    }

    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * threadsNum);
    for( int i = 0; i < threadsNum; ++i ) {

        pthread_create(threads + i, NULL, f, k + i);
    }

    void *p;
    for( int i = 0; i < threadsNum; ++i ) {
        pthread_join(threads[i], &p);
    }
    printf("%d\n", a);
    return 0;
}
//seminars
#include <pthread.h>
#include <stdio.h>
long long a = 0;
int threadsNum=10;

pthread_mutex_t mutex_;

void * f( void *p ) {
    int k = *(int *)p;
    long long temp = 0;
    for( long long i = k ; i < threadsNum * 500000000L; i += threadsNum ) {
      ++temp;
    }
    pthread_mutex_lock(&mutex_);
    a += temp;
    pthread_mutex_unlock(&mutex_);
    return NULL;
}

int main(int argc, const char * argv[]) {

    int b = pthread_mutex_init(&mutex_, NULL);

    int *k = (int*)malloc(threadsNum * sizeof(int));
    for( int i = 0; i < threadsNum; ++i ) {
        k[i] = i;
    }

    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * threadsNum);
    for( int i = 0; i < threadsNum; ++i ) {

        pthread_create(threads + i, NULL, f, k + i);
    }

    void *p;
    for( int i = 0; i < threadsNum; ++i ) {
        pthread_join(threads[i], &p);
    }
    printf("%lld\n", a);
    pthread_mutex_destroy(&mutex_);
    return 0;
}
=========================================================================
//chat
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    int d  = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    int c;
    ssize_t s;
    char * buffer = malloc(1024*sizeof(char));
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);
    if(bind(d, (struct sockaddr *)&servaddr, sizeof(servaddr))<0){
        perror("bind");
        exit(1);
    }
    if(listen(d, 10)<0){
        perror("listen");
        exit(1);
    }
    int size = 1-1;
    int k = 10;
    while(k>0){
        c = accept(d, NULL, NULL);
        while(1){
            recv(c, buffer, size, 0);
            while(strcmp(buffer, "quit\n")){
            }
        }
        close(c);
        k--;
    }
    free(buffer);
    return 0;
}
=========================================================================
//cat
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
   	if(argc == 1){
		int c = 0;
		while((c = getchar()) != EOF){
			printf("%c", c);
		}
	} else {
		FILE* f;
	 	int i = 1;
	 	for(;i < argc;++i){
    		f = fopen(argv[i], "r");
		if(f == NULL){
			perror("This is SPARTA!!!");
			exit(10);
		}
		int c = 0;
		while((c = fgetc(f)) != EOF){
			printf("%c", c);
		}
		fclose(f);
	    }
	}
}
=========================================================================
//catch signal
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void myHandler(int a){
  printf("\n %d \n", getpid());
  exit(10);
}

int main(int argc, char** argv){

  struct sigaction new_ = {myHandler, 1, 1};
  struct sigaction* old_;

  int a = sigaction(SIGINT, &new_, NULL);
  while(1);
}
//father_son_signal
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void myHandler1(int a){
  printf("\n father->son %d \n", getpid());
  exit(10);
}


void myHandler2(int a){
  printf("\n son->father %d \n", getpid());
  exit(10);
}

int main(int argc, char** argv){
  struct sigaction new1_ = {myHandler1, 1, 1};
  int a = sigaction(SIGUSR1, &new1_, NULL);

  struct sigaction new2_ = {myHandler2, 1, 1};
  int a1 = sigaction(SIGUSR2, &new2_, NULL);

  pid_t pid = fork();
  if(pid){
    sleep(2);
    kill(getpid(), SIGUSR2);
  } else {
    kill(pid, SIGUSR1);

  }
}
=========================================================================
