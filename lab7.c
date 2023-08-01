#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/timex.h>
#include <time.h>
#include <stdbool.h>

#include <pigpio.h>

#define LED 2
#define BUTTON 4

// static int button1 = BUTTON;
// static int led1 = LED;

long currNanos;
time_t currTime;

long* nanoPtr = &currNanos;
time_t* currPtr = &currTime;

long ledTime = 0;
long* ledPtr = &ledTime;
 
int buttonPress = 0;
int* buttonPtr = &buttonPress;

long ans;
long* ansPtr = &ans;

long answers[10000];

long* answersPtr = &answers;


//custom get time function
struct timespec ts1;


long getTime(void){

    long times[2];
    long* timesPtr = &times;
    for(int i = 0; i < 2; i++){

        timesPtr[i] = malloc(sizeof(long));
    }
    


    if(clock_gettime(CLOCK_REALTIME, &ts1) < 0 ){
        perror("There was an error with the CLOCK_REALTIME()");

    }else{

        currTime = ts1.tv_sec;
        currNanos = ts1.tv_nsec;
    }

    times[0] = currTime;
    times[1] = currNanos;
    
    return *times;

}

// void* buttonFunk(int gpio, int level, uint32_t tick){
void* buttonFunk(){
    printf("isr reached\n");

    if (gpioRead(BUTTON) != PI_HIGH){

        buttonPress = 1;

        printf("%s", "button pressed\n");
        getTime();
        ledTime = currNanos;
        gpioWrite(LED, 1);

        sleep(0.01);

        gpioWrite(LED,0);

    }else{
        buttonPress = 0;
    }

}



long fib(n){

    //set index 0 to 1
    answers[0] = 1;
    //base case
    if(n == 1 || n == 2){
        ans = 1;
        return ans;
    }
    else if(answers[n] != NULL){
        ans = answers[n];
        return ans;
    }
    else if(answers[n - 1] != NULL && answers[n - 2] != NULL){
        ans = ( answers[n - 1] + answers[n - 2] );
        answers[n] = ans;
        return ans;
    } else{
        ans = (fib(n - 1) + fib(n - 1));
        answers[n] = ans;
        return ans;
    }
}


void* cpuLoad1(void* arg){
    long nacc[1000];

    for(int i = 0; i < 1000; i++){

        nacc[i] = malloc(sizeof(long));

    }
    

    while(1){

        for(int i = 0; i < 1000; i++){
            nacc[i] = fib(i);
        }

    }

    

}


int main(int argc, char **argv ){

    // int loads = 0;

    // loads = malloc(sizeof(int));

    // for(int i = 0; i < argc; i++){
    //     if (strcmp(argv[i], "-a") == 0){
    //     printf("running one cpu load \n");
        
    //     }else{
            
    //     }
    // }



    currNanos = malloc(sizeof(long));
    currTime = malloc(sizeof(time_t));

    ledTime = malloc(sizeof(long));

    buttonPress = malloc(sizeof(int));

    ans = malloc(sizeof(long));

    for(int j = 0; j < 10000; j++){

        answers[j] = malloc(sizeof(long));

    }
    long nacc = 0;
    
    nacc = malloc(sizeof(long));

    
    if (gpioInitialise()<0) return -1;

    pthread_t thread1, thread2, thread3;

    long pressedTime = 0;
    pressedTime = malloc(sizeof(long));

    long responseTime = 0;
    responseTime = malloc(sizeof(long));
    


    gpioSetMode(BUTTON, PI_INPUT);
    gpioSetMode(LED, PI_OUTPUT);

    gpioWrite(LED, 0);

    gpioSetPullUpDown(BUTTON,PI_PUD_UP);

    gpioSetISRFunc(BUTTON, FALLING_EDGE, 1000, buttonFunk);

    pthread_create(&thread1,NULL,cpuLoad1,NULL);
    pthread_create(&thread2,NULL,cpuLoad1,NULL);
    pthread_create(&thread3,NULL,cpuLoad1,NULL);

    

    while(1){

        // while(buttonPress == 1){
        //     printf("Button pressed");

        // }
        if(gpioRead(BUTTON) != PI_HIGH){
            // printf("Button pressed");
            getTime();
            pressedTime = currNanos;
            buttonFunk();
        }

        responseTime = ledTime - pressedTime;

        if(responseTime > 0){
            printf("%ld", responseTime);
            printf("\n");

        }
        

        responseTime = 0;
        ledTime = 0;
        pressedTime = 0;
    }

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);

    //fib(n) function test
    // nacc = fib(15);

    // printf("%ld", nacc);
    

    gpioTerminate();
}



