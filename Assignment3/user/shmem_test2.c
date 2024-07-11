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
        sleep(2);
        printf("%s\n", memory);
        wait(0);
    }
    else{ // child
        printf("Size of child process before mapping: %d\n", sbrk(0));
        uint64 dst_va = map_shared_pages(parent_id, (uint64)memory, 4096);
        if(dst_va == -1){
            printf("Mapping from parent to child failed\n");
            return -1;
        }

        printf("Size of child process after mapping: %d\n", sbrk(0));

        const char *str = "Hello daddy";
        strcpy((char*)dst_va, str);

        int child_pid = getpid();
        if(unmap_shared_pages(child_pid, dst_va, 50) != 0){
            printf("Unmapping failed\n");
            return -1;
        }
        printf("Size of child process after unmapping: %d\n", sbrk(0));
        memory = (char*)malloc(50);
        printf("Size of child process after calling malloc(): %d\n", sbrk(0));
        exit(0);
    }
    free(memory);
    exit(0);
}