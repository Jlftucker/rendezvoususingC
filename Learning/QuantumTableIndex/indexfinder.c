#include <stdio.h>
#include<string.h>
#include <stdbool.h>



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



int main() {
    short int Ns = 3;
    short int player1position = 2;
    short int player2position = 2;
    char graph[] = "cyclic";
    bool check_first = false;

    int result = quantum_table_index(Ns, player1position, player2position, graph, check_first);
    printf("Result: %d\n", result);

    return 0;
}
