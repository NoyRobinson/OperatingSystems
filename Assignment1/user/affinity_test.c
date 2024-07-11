#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int p_id = getpid();
    set_affinity_mask(5); //5 = 101 - we want it to run on cpu's 0 and 2 only
    for(;;){
        printf("Process ID: %d\n", p_id);
        sleep(10);
    }
    return 0;
}