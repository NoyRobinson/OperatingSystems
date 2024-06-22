#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//returns 0 if n is not a a prime number and 1 if it is prime
int
isPrime(int n)
{    
    if(n <= 1)
        return 0;

    for(int i = 2; i * i <= n; i++){
        if(n % i == 0)
            return 0;
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    //main is the parent process
    int checkers_num;
    checkers_num = atoi(argv[1]);
    if(!(checkers_num > 0 && checkers_num <= 100)){
        printf("invalid input- enter a number between 1 to 100\n");
        exit(0);
    }

    while(1){
        int generator_cd = channel_create(); //for the generator to send numbers to thr checkers
        int checker_cd = channel_create(); //for the checkers to send results to the printer
        if(generator_cd < 0 || checker_cd < 0)
            exit(0);

        //generator
        if(fork() == 0){
            int n = 2;
            while(channel_put(generator_cd, n) == 0){
                n++;
            }
            exit(0); //destroy
        }

        //checkers
        for(int i = 0; i < checkers_num; i++){
            if(fork() == 0){
                int data;
                while(channel_take(generator_cd, &data) == 0){
                    if(isPrime(data) == 1){
                        if(channel_put(checker_cd, data) < 0){ //channel destroyed
                            channel_destroy(generator_cd);
                            exit(0);
                        }
                    }
                    sleep(2);
                }
                exit(0);
            }
        }

        //printer
        if(fork() == 0){
            int toPrint;    
            int counter = 0;
            while(counter < 100){
                channel_take(checker_cd, &toPrint);
                printf("The number %d is a prime number.\n", toPrint);
                counter++;
            }
            channel_destroy(checker_cd);
            exit(0);
        }

        //wait for child processes (checker,printer,generator) to exit
        for (int i = 0; i < checkers_num + 2; i++) {
            if(i == 0)
                printf("Printer with pid %d shutting down\n", wait(0));
            else if (i != checkers_num + 1)
                printf("Checker %d with pid %d shutting down\n", i, wait(0));
            else
                printf("Generator with pid %d shutting down\n", wait(0));
        }

        char ans[3];
        printf("Do you want to start the system again? (y/n) ");
        if(read(0, ans, sizeof(ans) - 1) < 0){
            printf("invaild stdin");
            exit(1);
        }
        if(ans[0] != 'y')
            break;
    }
    exit(0);
}