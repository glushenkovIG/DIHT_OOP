
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


