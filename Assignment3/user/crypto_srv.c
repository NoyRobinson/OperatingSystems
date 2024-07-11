#include "kernel/types.h"
#include "user/user.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "kernel/fcntl.h"

#include "kernel/crypto.h"

int main(void) {
  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  printf("crypto_srv: starting\n");

  // TODO: implement the cryptographic server here 

  if(getpid() != 2)
    exit(0);

  while(1){
    void* va = (char*)malloc(4096);
    uint64 size = 4096;
    int take = take_shared_memory_request(&va, &size);
    if(take == -1){
      printf("Take shared memory request failed\n");
      exit(0);
    }
    else { // take == 0
      





      int remove = remove_shared_memory_request(&va, &size);
      if(remove == -1){
        printf("Remove shared memory request failed\n");
        exit(0);
      }
    }
  }

  exit(0);
}
