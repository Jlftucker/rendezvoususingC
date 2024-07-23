#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <time.h>


int random(int N){
    int randomnumber = rand() % 10;
    printf("%d\n", randomnumber);
    return 0;
}

int main(){

    srand(time(NULL));
    random(10);

    return 0;

}