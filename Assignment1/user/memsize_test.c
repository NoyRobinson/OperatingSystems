#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int memorySize = memsize();
    printf("The running process is using %d bytes of memory\n", memorySize);

    void* allocate_space = malloc(20000);
    memorySize = memsize();
    printf("The running process is using %d bytes of memory after the allocation\n", memorySize);
    
    free(allocate_space);
    
    memorySize = memsize();
    printf("The running process is using %d bytes of memory after the release\n", memorySize);
    
    exit(0, "No exit message\n");
}