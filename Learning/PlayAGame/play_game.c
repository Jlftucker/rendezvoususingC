#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <time.h>

/*
A function that takes both the players old positions and new positions and then checks to see if they have won

INPUTS 

int new_player_positions[] - Integer array that contains the players new positions
int old_player_positions[] - Integer array that contains the players old positions before they have moved
bool edges - Boolean variable that decides whether or not players are allowed to meet via transposition

OUTPUT

bool win - boolean variable that declares whether players have won or not


*/


bool win_checker(int new_player_positions[], int old_player_positions[], bool edges){
    bool win;//Declare win variable

    if (edges == false){//if edges is false check they are on the same node
        if (new_player_positions[0]== new_player_positions[1]){
            win = true;//Win variable set to true
        } 
        else{
            win = false;//win variable set to false
        }

    }
    else if (edges == true){//If no edges
        if(new_player_positions[0]== new_player_positions[1]){//Compare to see if players are in the same position at end 
            win = true;//Win variable set to true
            
        }
        else if ( new_player_positions[0]==old_player_positions[0] && new_player_positions[0]==old_player_positions[0] ){// Compare to see if players have transposed positions
            win = true;//Win variable set to true
            
        }
        else{
            win = false;//win variable set to false
        }
    }
    return win;
}




// Player move function that moves the player

/*
Input 

old_pos --- Integer --- Players positions before moving
coin ------ Integer --- Result of the coin toss 1 = heads 0 = tails
Ns -------- Integer --- Number of sites on the grap



*/


int make_move(short int old_pos, short int coin, short int Ns, char graph[]){
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



/*
A function that finds that part of the 2d Quantum Table array to look at for the players coin results

Input 

short int Ns - The number of sites on the graph
short int player1position - The first players position
short int player2position - The second players position
char graph[] - A character array containing the string of the graph name 
bool check_first - A boolean variable that decides whether the game is check first of check later ( true is check first)

Output

int final_counter - what part of the sub array to look at


*/


int quantum_table_index(short int Ns, short int player1position, short int player2position, char graph[], bool check_first) {
    int counter = 0; // counter for array
    int final_counter = -1; // initialize final_counter

    char cycle[] = "cyclic"; // String array for graph names
    int cyclegraph_check = strcmp(graph, cycle);

    if (cyclegraph_check == 0) { // If we have a cycle graph
        for (int i = 0; i < Ns; i++) { // iterate through first player sites
            for (int j = 0; j < Ns; j++) { // iterate through second player sites
                if (i == j && check_first == true) { // if possible positions match check to see if it's allowed
                    continue; // continue j loop
                } else if (i == player1position && j == player2position) { // if player positions match the indices then we have our variable
                    final_counter = counter;
                }
                else{
                    counter++;
                }
            }
        }
    }

    printf("Ns: %d, player1position: %d, player2position: %d, graph: %s, check_first: %d, final_counter: %d\n", Ns, player1position, player2position, graph, check_first, final_counter); // Debug statement
    return final_counter;
}


/*
Function - Random integer generator - needs to be reseeded evertime it is called 

Input 
int N - Upper boundary for random integer

Output

int randomnumber - random number that is generated 

*/


int random(int N){
    int randomnumber = rand() % N;
    printf("%d\n", randomnumber);
    return randomnumber;
}








/*
A function that moves the player 

Input 

short int Ns - The number of sites on the graph
short int player1position - The first players position
short int player2position - The second players position
char graph[] - A character array containing the string of the graph name 
bool check_first - A boolean variable that decides whether the game is check first of check later ( true is check first)

Output

int final_counter - what part of the sub array to look at




*/

bool player_move(int initial_pos[], char graph[], int Np, bool edge, int *quantum_table_pointer,bool check_first, int Ns, int Nm ){
    int new_pos[Np];//Declare an array with a position per player
    bool win = false;//Declare boolean winning variable
    int i = 0;//Declare counter 
    int j =0;
    int table_index; //Declare table index
    int player_newpos;


    //Win check for check first games
    if (check_first == true){
        win = win_checker(initial_pos,new_pos,check_first);//check to see if they win by landing, new_pos is an empty array so the bool needs to be false 
        // so that edges are not checked since this cannot occur right now
    }
    
    while(win==false && j <Nm, j++){
        int player1position = initial_pos[0];
        int player2position = initial_pos[1];

        table_index = quantum_table_index(Ns,player1position,player2position,graph,check_first);

        int num_coinflips = 20000 ;
        int potentialcoins[num_coinflips];
        //Stil needs work to pull out coin from array

        while(i <Np, i++){//iterate through the number of players
            new_pos[i] = make_move(initial_pos[i],0,Ns,graph);//Set new_positions = to the intial positions
        }
            win = win_checker(new_pos,initial_pos,check_first);
            initial_pos[0] = new_pos[0];//set positions for next move
            initial_pos[1] = new_pos[1];  
    }

    return win ;
}




/*

*/

void many_runs(char graph[], int Np, bool edge, int *quantum_table_pointer,bool check_first, int Ns, int Nr, int Nm  ){
    int initial_pos[Np];//Initial position array
    bool win; //win variable
    int num_wins = 0;//number of wins counter
    int i = 0;//counter for number of runs
    int j =0;//counter for number of players

    while(i<Nr, i++){//Number of runs loop

        while (j<Np, j++ )// Setup the game 
        {
            srand(time(NULL));
            initial_pos[j] = random(Ns)

        }
        
        win = player_move(initial_pos,graph,Np,edge,*quantum_table_pointer,check_first,Ns,Nr);
    }

}





void main(){

    int Ns =3;
    bool check_first =true;

    int number_of_combos;//declare number of combos

    if(check_first == true){
        number_of_combos = (Ns*Ns) - Ns;//calculate the number of combos, minus Ns combos for check first
    }
    else{
        number_of_combos = (Ns*Ns);//calculate the number of combos, Ns^2
    }

    int quantum_table[20000][number_of_combos];
    int *quantum_table_pointer;//define integer pointer 
    quantum_table_pointer = &quantum_table;//Set the pointer to the table

    many_runs("cyclic",2,false,quantum_table_pointer,true,Ns,2)

    return;
}