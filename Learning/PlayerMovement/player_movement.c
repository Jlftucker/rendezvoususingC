#include <stdio.h>
#include<string.h>
#include <stdbool.h>

// Player move function that moves the player

/*
Input 

old_pos --- Integer --- Players positions before moving
coin ------ Integer --- Result of the coin toss 1 = heads 0 = tails
Ns -------- Integer --- Number of sites on the grap



*/
__declspec(dllexport) int make_move(short int old_pos, short int coin, short int Ns, char graph[]){
    int new_pos; //Declare new position variable
    int number_choices;//Declare variable that will be used to decide array length

    char cycle[] = "cyclic"; //String array for graph names

    int cyclegraph_check = strcmp(graph, cycle);//Returns zero if the two character arrays are identical
    
    if (cyclegraph_check ==0)//If we are on a cycle graph 
        {
        number_choices = 2;//Number of options available 
        int available_positions[number_choices];//Declare array with a size dependent on the number of sites
        if (old_pos == Ns -1)
        {
            available_positions[0] = 0;
            available_positions[1] = Ns-2;
        }
        else if (old_pos == 0)
        {
            available_positions[0] = 1;
            available_positions[1] = Ns-1;
        }
        else
        {
            available_positions[0] = old_pos-1;
            available_positions[1] = old_pos+1;
        }
        
        if (coin == 0)
            new_pos = available_positions[0];
        else if (coin == 1)
        {
            new_pos = available_positions[1];
        }
        }

    return new_pos;
}
