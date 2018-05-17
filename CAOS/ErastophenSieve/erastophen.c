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
/*
    for(unsigned long long i = 2; i < n; i++){
        if(num_buf[i] == 0) {
            printf("%llu\n", i);
        }
    }
    */
    free(threads);
    free(primes);
    free(num_buf);
}
