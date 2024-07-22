// example.c
#include <stdio.h>

__declspec(dllexport) int print_array(int array[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    int j =6;

    return j;
}