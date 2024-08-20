#define _USE_MATH_DEFINES
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>
#include "qpp/qpp.h"
#include <boost/version.hpp>
#include <typeinfo>

using namespace qpp;
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


int make_move(short int old_pos, short int coin, short int Ns, const char graph[]){
    int new_pos; //Declare new position variable
    int number_choices;//Declare variable that will be used to decide array length

    char cycle[] = "cyclic"; //String array for graph names

    int cyclegraph_check = std::strcmp(graph, cycle);//Returns zero if the two character arrays are identical

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


int quantum_table_index(short int Ns, short int player1position, short int player2position, const char graph[], bool check_first) {
    int counter = 0; // counter for array
    int final_counter = -1; // initialize final_counter

    char cycle[] = "cyclic"; // String array for graph names
    int cyclegraph_check = std::strcmp(graph, cycle);

    if (cyclegraph_check == 0) { // If we have a cycle graph
        for (int i = 0; i < Ns; i++){ // iterate through first player sites
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
    //printf("%d\n", randomnumber); Debug statement
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

bool player_move(int initial_pos[],const char graph[], int Np, bool edge, std::vector<std::vector<float>> quantum_table,bool check_first, int Ns, int Nm, const char strategy[]){
    int new_pos[Np];//Declare an array with a position per player
    bool win = false;//Declare boolean winning variable
    int i = 0;//Declare player counter
    int j =0;//Declare number of moves counter
    int coin = 0;//Declare coin variable
    int table_index; //Declare table index
    std::vector<std::string> coins = {"00","01", "10", "00"};//declare string array for coins
    std::vector<float> probability_distro;
    std::string player_coins;
    char QuantumStrategy[] = "quantum";
    char Classical_golow[] = "classical_go_to_lowest";

    int Quantum_strategycheck = std::strcmp(strategy,QuantumStrategy);
    int Classical_strategycheck = std::strcmp(strategy,Classical_golow);

    //Win check for check first games
    if (check_first == true){
        win = win_checker(initial_pos,new_pos,edge);//check to see if they win by landing, new_pos is an empty array so the bool needs to be false
        // so that edges are not checked since this cannot occur right now
    }

    while(!win && j <Nm){
        int player1position = initial_pos[0];
        int player2position = initial_pos[1];

        if(Classical_strategycheck ==0){//if classical coin then each player uses the same go to lowest coin and we declare outside the loop
            coin =0;
            //std::cout << "ping you are classical";
        }
        else if (Quantum_strategycheck == 0){// if quantum coin we need to declare a placeholder variable as the coin is a string of each players coin now so we need to split them
            table_index = quantum_table_index(Ns,player1position,player2position,graph,check_first);
            probability_distro = quantum_table[table_index];


            //Create a random number generator and a discrete distribution
            std::random_device rd; // Obtain a random number from hardware
            std::mt19937 gen(rd()); // Seed the generator
            std::discrete_distribution<> dist(probability_distro.begin(), probability_distro.end());

            //Generate a random index
            int coin_index = dist(gen);
           // std::cout << "coin index" << "" << coin_index << std::endl; Debug statement
            player_coins =coins[coin_index];
            //std::cout << "Player 1 coin " << player_coins[0]; Debug statement
        }

        while(i <Np){//iterate through the number of players
            if (Quantum_strategycheck == 0){coin = player_coins[i] - '0';}
            //std::cout << "The coin is now" << coin_used << std::endl;
            //the coins are currently characters in a string array so held in ascII, by subtraction '0' we subtract that ascii code and get the number we want
            new_pos[i] = make_move(initial_pos[i],coin,Ns,graph);//Set new_positions = to the intial positions
            i++;
        }

        win = win_checker(new_pos,initial_pos,edge);
        initial_pos[0] = new_pos[0];//set positions for next move
        initial_pos[1] = new_pos[1];

        j++;
    }

    return win;
}


/*

*/

float many_runs(const char graph[], int Np, bool edge, std::vector<std::vector<float>> quantum_table,bool check_first, int Ns, int Nr, int Nm, const char strategy[]){
    int initial_pos[Np];//Initial position array
    bool win; //win variable
    float num_wins = 0;//number of wins counter
    int i = 0;//counter for number of runs//counter for number of players
    srand(time(NULL));//seed random number generator

    while(i<Nr){//Number of runs loop
        //printf("%s\n", "Loop is here now");
        int j =0;//counter for number of players
        while (j<Np){// Setup the game
            initial_pos[j] = random(Ns);
            //printf("Initial position of player %d: %d\n", j + 1, initial_pos[j]); Debug statement
            j++;
        }

        win = player_move(initial_pos,graph,Np,edge,quantum_table,check_first,Ns,Nm,strategy);

        if (win == true){
            num_wins++;
            win = false;
        }
        i++;
    }

    return(num_wins);
}


/*

Function quantum_circuit_maker - Generates the quantum circuit result

*/


std::vector<float> quantum_circuit_maker(const char graph[], int player1_position, int player2_position, int Ns, std::vector<float> probabilities){
    char cycle[] = "cyclic"; // String array for graph names
    int cyclegraph_check = std::strcmp(graph, cycle);
    float thetas[] = {0,0,(2/3)*M_PI,M_PI/2, 0.4*M_PI,M_PI/3,0.5714*M_PI,M_PI/2}; // angles for cyclic graphs
    float theta_i;//angle of rotation for player 1
    float theta_j;//angle of rotation for player 2
    float prob;

    if(cyclegraph_check ==  0){//If graph is cyclic

        if(Ns ==3){
            theta_i = player1_position*(M_PI/3);
            theta_j = player2_position*(M_PI/3);
        }


        //First create a bell state
        QCircuit qc{2,2};//Create a quantum circuit with 2 qubits(1st number) and 2 classical bits(2nd number)
        qc.gate(gt.H,0);//apply a Hadamard gate to the first qubit
        qc.CTRL(gt.X,0,1);//Apply a controlled not gate between the first and second qubit

        //Now apply rotations

        qc.gate(gt.RY(theta_i), 0);
        qc.gate(gt.RY(theta_j), 1);

        // Now measure them
        qc.measure({0,1});//attach measurement to classical bits

        // initialize the quantum engine with a circuit
        QEngine engine{qc};

        // display the quantum circuit and its corresponding resources
        //  std::cout << qc << "\n\n" << qc.get_resources() << "\n\n";
        // execute the entire circuit
        engine.execute(20000);
        // Measure qubit 0
        const auto& stats = engine.get_stats();
        // Access raw data
        const auto& raw_data = stats.data();

        // Print raw data
        //std::cout << "Raw data:\n";
        for (const auto& entry : raw_data) {
           // const auto& outcome = entry.first;
            float count = entry.second;
            //Print the data
            //std::cout << "Outcome: ";
           // for (auto bit : outcome) {
                //std::cout << bit << " ";
           // }
            //std::cout << " - Count: " << count << "\n";
            prob = count/20000;
            //std::cout << "Probability is" << prob << std::endl;
            probabilities.insert(probabilities.end(),prob);//probabilities are in order of 00 01 10 11
           // std::cout << probabilities[0]; Debug statement
    }
    //Debug for checking probabilite tables
    /*for (auto prob : probabilities) {
            //std::cout << prob << " ";
           // }
    std::cout << "New line";
    */
    }

    return(probabilities);
}


/*
Function quantum_table_generator - generates the quantum table for us to use based off of the game we are playing
*/




std::vector<std::vector<float>> quantum_table_generator(const char graph[], int Ns, int Np, bool check_first){

    // Initialize the quantum table as a  vector
    std::vector<std::vector<float>> quantum_table;
    // Set up random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    // Example of setting up the random number generator in a quantum computing library
    // RandomDevices::get_instance().get_prng() = gen; // Ensure this is correctly integrated

    for (int i = 0; i < Ns; ++i) { // Iterate through possible player 1 positions
        for (int j = 0; j < Ns; ++j) { // Iterate through possible player 2 positions
            if (i == j && check_first) { // Skip if players are on the same position and check_first is true
                continue;
            }
            else{
             // Iterate through circuit shots
                std::vector<float> probabilities;
                probabilities = quantum_circuit_maker(graph, i, j, Ns, probabilities);
                // Generate circuit result
                quantum_table.push_back(probabilities);
            }

             // Move to the next row
        }
    }



    return quantum_table; // Return the populated quantum table
}






float run_game(){

    float win_percent;//declare win percent variable
    float number_wins;//declare number of wins variable
    int Np =2;//Number of players variable
    int Ns =3 ;//Number of sites in the game
    int Nr = 1000000;//Number of runs of the game
    int Nm = 1;//Number of moves players are allowed to make
    bool check_first = true;//Check first or check later variant of the game
    bool edge = false;//Are players allowed to meet one edges
    const char graph[] = "cyclic";//What graph are we playing on
    const char strategy[] = "quantum";//What Strategy are the players using


    clock_t start1 = clock();//Start clock
    std::vector<std::vector<float>> quantum_table = quantum_table_generator(graph, Ns, Np, check_first);
    clock_t end1 = clock();//finish clock
    double table_time = (double)(end1 - start1) / CLOCKS_PER_SEC;//calculate execution time

    std::cout<< "Quantum Table generation time is "<< ""<< table_time << std::endl ;//Print execution time

    clock_t start2 = clock();//Start clo
    number_wins = many_runs(graph,2,edge,quantum_table,true,Ns,Nr,Nm,strategy);//run many_runs function
    clock_t end2 = clock();//finish clock
    double run_time = (double)(end2 - start2) / CLOCKS_PER_SEC;//calculate execution time

    std::cout<< "Time to play game is "<< ""<< run_time << std::endl ;//Print runtime

    win_percent = number_wins/Nr;//calculate win percentage

    return win_percent;
}



int main(){

    float win_percent;
    win_percent = run_game();
    std::cout << "Win percentage is " << ""<< win_percent;





    return 0;
}
