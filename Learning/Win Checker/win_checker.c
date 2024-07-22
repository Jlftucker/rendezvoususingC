#include <stdio.h>
#include <stdbool.h>

/*
A function that takes both the players old positions and new positions and then checks to see if they have won

INPUTS 

int new_player_positions[] - Integer array that contains the players new positions
int old_player_positions[] - Integer array that contains the players old positions before they have moved
bool edges - Boolean variable that decides whether or not players are allowed to meet via transposition

OUTPUT

bool win - boolean variable that declares whether players have won or not


*/



__declspec(dllexport) bool win_checker(int new_player_positions[], int old_player_positions[], bool edges){
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

