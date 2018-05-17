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