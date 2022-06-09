#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

//thread returns some value till t1 waits for it to complete
void* roll_dice() {
    int value = (rand() % 6) + 1;
  //we cannot return value or reference to value direcly as it is local to this function
  //so create a dynamic allocation and assign value to it and return it
    int* result = malloc(sizeof(int));
    *result = value;
    // printf("%d\n", value);
    printf("Thread result: %p\n", result);
    return (void*) result;
}


int main(int argc, char* argv[]) {
    int* res;
    srand(time(NULL));
    pthread_t th;
    if (pthread_create(&th, NULL, &roll_dice, NULL) != 0) {
      //error
        return 1;
    }
  //send the value as void pointer (wildcard , can be anything)
    if (pthread_join(th, (void**) &res) != 0) {
      //error
        return 2;
    }
  //return value and print it
    printf("Main res: %p\n", res);
    printf("Result: %d\n", *res);
  
  //deallocate the allocated memory
    free(res);
    return 0;
}
