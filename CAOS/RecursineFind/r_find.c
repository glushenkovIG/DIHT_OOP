
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

