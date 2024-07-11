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
    if(take == -1)
      printf("Take shared memory request failed\n");
    else { // take == 0
      struct crypto_op* request = (struct crypto_op*)va;
      if(!(request->state == CRYPTO_OP_STATE_INIT && (request->type == CRYPTO_OP_TYPE_ENCRYPT || request->type == CRYPTO_OP_TYPE_DECRYPT))){
          asm volatile ("fence rw,rw" : : : "memory");
          request->state = CRYPTO_OP_STATE_ERROR;
      }
      else{ // request is valid
        uchar* data = &(request->payload[request->key_size]);

        int j = 0;
        for(int i = 0; i < request->data_size; i++){
          if(j >= request->key_size)
            j = 0;
          data[i] = data[i] ^ (request->payload[j]);
          j++;
        }
        
        asm volatile ("fence rw,rw" : : : "memory");
        request->state = CRYPTO_OP_STATE_DONE;

        int remove = remove_shared_memory_request(va, size);
        if(remove == -1)
          printf("Remove shared memory request failed\n");
      }
    }
  }
  exit(0);
}
