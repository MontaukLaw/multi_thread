#include "demo.h"

uint8_t countMax = 0;
int counter;;

void print_demo_log(void) {

    printf("nice\n");
}

char *get_out_some_str(void) {
    char *testChar = "ok";
    sprintf(testChar, "countMax: %d\n", countMax);
    return testChar;
}

void *sub_thread(void *arg) {

    while (countMax) {
        counter++;
        countMax++;
        sleep(1);
    }

    return NULL;
}

void start_new_thread(void) {

    pthread_t ptid;

    pthread_create(&ptid, NULL, sub_thread, NULL);
    // pthread_join(ptid, NULL);

}