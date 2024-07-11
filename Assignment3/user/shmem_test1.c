#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    int parent_id = getpid();
    
    void* memory = (char*)malloc(4096);
    if(memory == 0){
        printf("Memory allocation failed\n");
        return -1;
    }

    int pid = fork();
    if(pid == -1){
        printf("Fork failed\n");
        return -1;
    }

    if(pid != 0){ // parent
        const char *str = "Hello child";
        strcpy(memory, str);
        wait(0);
    }
    else{ // child
        uint64 dst_va = map_shared_pages(parent_id, (uint64)memory, 4096);;
        if(dst_va == -1){
            printf("Mapping from parent to child failed\n");
            return -1;
        }
        printf("%s\n", (char*)dst_va);
        exit(0);
    }

    free(memory);
    exit(0);
}