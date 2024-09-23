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
#include <numeric>
#include <sstream> // stringstream
#include <iomanip> // put_time



#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Define the type for our genetic algorithm's vectors
using Chromosome = std::vector<float>;


using namespace qpp;

//////////PLAYING THE GAME FUNCTIONS ///////////////////////////////////////////////


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


int make_move(short int old_pos, char coin, short int Ns, const char graph[]){
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

        if (coin == '0')
            new_pos = available_positions[0];
        else if (coin == '1')
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
    char coin;//Declare coin variable
    int table_index; //Declare table index
    std::vector<std::string> coins = {"00","01", "10", "11"};//declare string array for coins
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
            coin ='0';
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
           // std::cout << "coin index" << "" << coin_index << std::endl; //Debug statement
            player_coins =coins[coin_index];
            //std::cout << "Player 1 coin " << player_coins[0]; Debug statement
        }
            //std::cout<< player_coins;
        while(i <Np){//iterate through the number of players
            if (Quantum_strategycheck == 0){
                   coin = player_coins[i];
                }
            //std::cout << "Player" << i << " coin from the array is " << player_coins[i] << "\n";
            //std::cout << "coin used is " << coin;}
            //std::cout << "The coin is now" << coin_used << std::endl;
            //the coins are currently characters in a string array so held in
            //std::cout << "player " << i << " position is " << initial_pos[i] << "\n";
           // std::cout << "coin used is " << coin << "\n";
            new_pos[i] = make_move(initial_pos[i],coin,Ns,graph);//Set new_positions = to the intial positions
           // std::cout << "New position is " << new_pos[i] << "\n";
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
FUNCTION - many_runs - A function that runs the game multiple times with the total number of times decided by an input variable

INPUT

graph - Character Array - Name of the graph the game is being played on
Np - INT - The number of players in a game
edge - bool - decides whether or not players can meet on the edge
quantum_table - vector<vector<float>> - a two dimensional vector that holds the probabilities of every scenario
check_first - bool - decides whether players check before moving or check after moving
Ns - INT - Number of sites on the graph the game is being played on
Nr - INT - Number of runs the simulation will do
Nm - INT - Number of moves the players are allowed to make
strategy - character array - Name of the strategy the players are using

OUTPUT

num_wins - float - Number of wins out of the runs


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

// Function to generate all possible outcomes for n qubits

std::vector<std::string> generatePossibleOutcomes(int numQubits) {
    std::vector<std::string> outcomes;
    int totalOutcomes = 1 << numQubits; // 2^numQubits

    for (int i = 0; i < totalOutcomes; ++i) {
        std::string outcome;
        for (int j = numQubits - 1; j >= 0; --j) {
            outcome += (i & (1 << j)) ? '1' : '0';
        }
        outcomes.push_back(outcome);
    }

    return outcomes;
}



/*
Function quantum_circuit_maker - Generates the quantum circuit and runs it 20000 times. Then stores in a vector array and returns it

INPUT

graph - Character Array - Name of the graph the game is being played on
player1_position - INT - position of player one
player2_position - INT - position of player two
Ns - INT - Number of sites on the graph the game is being played
probabiltiies - Vector<float> - An empty vector that will hold the probability results calculated from the circuit

OUTPUT

probabiltiies - Vector<float> - The now full vector that holds the probability results calculated from the circuit
*/


std::vector<float> quantum_circuit_maker(const char graph[], int player1_position, int player2_position, int Ns, std::vector<float> probabilities){
    char cycle[] = "cyclic"; // String array for graph names
    int cyclegraph_check = std::strcmp(graph, cycle);
    float thetas[] = {0,0,(2/3)*M_PI,M_PI/2, 0.4*M_PI,M_PI/3,0.5714*M_PI,M_PI/2}; // angles for cyclic graphs
    float theta_i;//angle of rotation for player 1
    float theta_j;//angle of rotation for player 2

    if(cyclegraph_check ==  0){//If graph is cyclic


        if(Ns ==3){//If  N=3 cycle graph
            theta_i = player1_position*(M_PI/3);
            theta_j = player2_position*(M_PI/3);
        }
        else{// Anything above 3 sites follows this pattern
            theta_i =  player1_position*thetas[Ns-1];
            theta_j =  player2_position*thetas[Ns-1];
            if(player1_position == Ns-1 || player1_position == 0){theta_i = theta_i +M_PI;}
            if(player2_position == Ns-1 || player2_position == 0){theta_j = theta_j +M_PI;}
        }
        //First create a bell state |00>+|11>
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

        //Get statistics from the quantum engine
        const auto& stats = engine.get_stats();
        // Access raw data
        const auto& raw_data = stats.data();

        //vector of possible outcomes
        std::vector<std::string> possibleOutcomes = {"00", "01", "10", "11"};

        // Initialize the outcomeCounts map with all possible outcomes
       std::unordered_map<std::string, float> outcomeCounts;
    for (const auto& outcome : possibleOutcomes) {
        outcomeCounts[outcome] = 0.0f; // Initialize with 0.0f

    }

    // Update the map with counts from raw_data
    for (const auto& entry : raw_data) {
        auto outcomes = entry.first;
        std::string outcome = std::to_string(outcomes[0])+std::to_string(outcomes[1]); // Extract the outcome name
        float count = entry.second;               // Extract the count
        outcomeCounts[outcome] = count;           // Update the map
    }

    // Compute probabilities and store in a vector
    const float totalRuns = 20000.0f; // Total number of runs

    for (const auto& outcome : possibleOutcomes) {
        float count = outcomeCounts[outcome];
        float prob = count / totalRuns; // Calculate probability
        probabilities.push_back(prob);  // Store probability
        }
    }

     /*Debug statement to check the probability table
     for(auto p:probabilities){
        std::cout << p;
    }
    std::cout<< "new line" << "\n";
    */

    return(probabilities);
}

//Debug for checking probability tables
    //for (auto prob : probabilities) {
          //  std::cout << prob << " ";
          // }
   // std::cout << "New line";

/*
Function - Genetic Quantum circuit
*/

std::vector<float> genetic_quantum_circuit(int player1_position, int player2_position, int Ns, std::vector<float> probabilities, std::vector<std::vector<float>> angle_vector, bool symmetric, bool classical_search){
    float theta_ix;//player 1 x rotation
    float theta_iy;//player 1 y rotation
    float theta_iz;//player 1 z rotation
    float theta_jx;//player 2 x rotation
    float theta_jy;//player 2 y rotation
    float theta_jz;//player 2 z rotation

    //For the moment this will just be for 2 qubits for a one step game but in the future it will be more

    //First create a bell state |00>+|11> - we can make this part of the genome actually (do later)
    QCircuit qc{2,2};//Create a quantum circuit with 2 qubits(1st number) and 2 classical bits(2nd number)

    if (classical_search==false){//If we aren't searching classically we entangle the qubits
        qc.gate(gt.H,0);//apply a Hadamard gate to the first qubit
        qc.CTRL(gt.X,0,1);//Apply a controlled not gate between the first and second qubit
    }


    if(symmetric==true){
        //player 1` angles of rotation
        theta_ix = angle_vector[player1_position][0];
        theta_iy = angle_vector[player1_position][1];
        //std::cout<< "Player 1 position is" << player1_position << "\n"; Debug to check the right angles are being picked for the right site
        //std::cout<< "Player 1 angle is" << theta_iy <<"\n"; Debug to check the right angles are being picked for the right site
        theta_iz = angle_vector[player1_position][2];
        //player 2 angles of rotation
        theta_jx = angle_vector[player2_position][0];
        theta_jy = angle_vector[player2_position][1];
        //std::cout<< "Player 2 position is" << player2_position << "\n";
        //std::cout<< "Player 2 angle is" << theta_jy <<  "\n";
        theta_jz = angle_vector[player2_position][2];
    }
    else if(symmetric==false){// if in an asymmetric game
        //player 1` angle of rotation
        theta_ix = angle_vector[player1_position][0];
        theta_iy = angle_vector[player1_position][1];
        theta_iz = angle_vector[player1_position][2];

        //player 2 angle of rotation
        theta_jx = angle_vector[player2_position][3];
        theta_jy = angle_vector[player2_position][4];
        theta_jz = angle_vector[player2_position][5];
    }

    // Now apply rotations to the circuit
    //Player 1 rotations
    qc.gate(gt.RX(theta_ix), 0);
    qc.gate(gt.RX(theta_jx), 1);

    qc.gate(gt.RY(theta_iy), 0);
    qc.gate(gt.RY(theta_jy), 1);

    //player 2 rotations

    qc.gate(gt.RZ(theta_iz), 0);
    qc.gate(gt.RZ(theta_jz), 1);

    qc.measure({0,1});//attach measurement to classical bits
    // initialize the quantum engine with a circuit
    QEngine engine{qc};
    // display the quantum circuit and its corresponding resources
    //  std::cout << qc << "\n\n" << qc.get_resources() << "\n\n";
    // execute the entire circuit
    engine.execute(20000);
    //Get statistics from the quantum engine
    const auto& stats = engine.get_stats();
    // Access raw data
    const auto& raw_data = stats.data();

    //vector of possible outcomes
    std::vector<std::string> possibleOutcomes = {"00", "01", "10", "11"};

    // Initialize the outcomeCounts map with all possible outcomes
    std::unordered_map<std::string, float> outcomeCounts;
    for (const auto& outcome : possibleOutcomes) {
        outcomeCounts[outcome] = 0.0f; // Initialize with 0.0f
    }

    // Update the map with counts from raw_data
    for (const auto& entry : raw_data) {
        auto outcomes = entry.first;
        std::string outcome = std::to_string(outcomes[0])+std::to_string(outcomes[1]); // Extract the outcome name
        float count = entry.second;               // Extract the count
        outcomeCounts[outcome] = count;           // Update the map
    }

    // Compute probabilities and store in a vector
    const float totalRuns = 20000.0f; // Total number of runs

    for (const auto& outcome : possibleOutcomes) {
        float count = outcomeCounts[outcome];
        float prob = count / totalRuns; // Calculate probability
        probabilities.push_back(prob);  // Store probability
        }

    /*Debug statement to check the probabilities are being generated correctly
    std::cout << "Player 1 position" << player1_position << "\n";
    std::cout << "Player 2 position" << player2_position << "\n";
    for(auto p:probabilities){//Debug statement to check prob table
        std::cout << p << " ";
    }
    std::cout<< "Next site combo" << "\n";
    */


    return(probabilities);

}



/*
Function gene_quantum_circuit - A function that takes in a gene vector and builds a quantum table with it


*/

std::vector<std::vector<float>> genetic_quantum_table(std::vector<float> gene, int Ns, int Np, bool symmetric,bool check_first, bool classical_search){
//Each input vector will either be Ns (symmetric games) in length or 2Ns(asymmetric games) in length with each 3 numbers representing the three angles the player will rotate around the x y and z axis
    int a = 0; //counter for angle list
    std::vector<std::vector<float>> angle_vector;//2d vector that will hold arrays of angles -makes the positioning of the players much easier
    //std::cout << "We are about to build angle vector"; Debug statement

    if(symmetric==true){//if we are playing a symmetric game
        while(a<(gene.size() - 1)){
            std::vector<float> site_angles;//Declare site angle array using vector - Declare inside loop to reinitialize it
            float theta_x = gene[a]*2*M_PI;
            float theta_y = gene[a+1]*2*M_PI;
            float theta_z = gene[a+2]*2*M_PI;
            //Push the angles
            site_angles.push_back(theta_x);
            site_angles.push_back(theta_y);
            site_angles.push_back(theta_z);

            angle_vector.push_back(site_angles);
            a= a+3;
            //std::cout << "Building the angles"; Debug Statement
        }
    }
    else{//if we are playing an asymmetric game
        while(a<(gene.size() - 1)){//player 1 angles - increase by 6 each time
            std::vector<float> site_angles;//Declare site angle array using vector - Declare inside loop to reinitialize it
             //Player 1 angles
            float theta_x1 = gene[a]*2*M_PI;
            float theta_y1 = gene[a+1]*2*M_PI;
            float theta_z1 = gene[a+2]*2*M_PI;
            //Player 2 angles
            float theta_x2 = gene[a+3]*2*M_PI;
            float theta_y2 = gene[a+4]*2*M_PI;
            float theta_z2 = gene[a+5]*2*M_PI;
            //add the angles to the angle array now
            site_angles.push_back(theta_x1);
            site_angles.push_back(theta_y1);
            site_angles.push_back(theta_z1);
            site_angles.push_back(theta_x2);
            site_angles.push_back(theta_y2);
            site_angles.push_back(theta_z2);

            angle_vector.push_back(site_angles);
            a= a+6;
        }

    }
    //Now we need build our quantum table - luckily we can just copy this
   // std::cout << "We are about to build the table"; Debug statement
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
                std::vector<float> probabilities;
                probabilities = genetic_quantum_circuit(i,j,Ns,probabilities,angle_vector,symmetric, classical_search);
                // Generate circuit result
                quantum_table.push_back(probabilities);
            }

             // Move to the next row
        }
    }



    return quantum_table;
}




/*
Function quantum_table_generator - generates the quantum table for us to use based off of the game we are playing

INPUT

graph - Character Array - Name of the graph the game is being played on
Ns - INT - Number of sites on the graph the game is being played
Np - INT - The number of players in a game
check_first - bool - decides whether players check before moving or check after moving

OUTPUT

quantum_table - vector<vector<float>> - quantum table that stores the probabilities of each different scenario of the game

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
                std::vector<float> probabilities;
               // probabilities.reserve(4);
                probabilities = quantum_circuit_maker(graph, i, j, Ns, probabilities);
                // Generate circuit result
                quantum_table.push_back(probabilities);
            }

             // Move to the next row
        }
    }



    return quantum_table; // Return the populated quantum table
}


/*
Function quantum_table_generator - Runs game and sets input variables(separate to main as a main function has to return a int and want a float for the win percent)
INPUT

NS - INT - Number of sites on the graph

OUTPUT

win_percent - float - The win percentage of the game

*/




float run_game(int Np, int Ns, int Nm, bool check_first, bool edge, const char graph[], const char strategy[], bool symmetric){

    float win_percent;//declare win percent variable
    float number_wins;//declare number of wins variable
    int Nr = 3;//Number of runs of the game


    // if we know the strategy then use the one programmed in
        clock_t start1 = clock();//Start clock
        std::vector<std::vector<float>> quantum_table = quantum_table_generator(graph, Ns, Np, check_first);
        clock_t end1 = clock();//finish clock
        double table_time = (double)(end1 - start1) / CLOCKS_PER_SEC;//calculate execution time

        std::cout<< "Quantum Table generation time is "<< ""<< table_time << "\n" ;//Print execution time

        clock_t start2 = clock();//Start clo
        number_wins = many_runs(graph,Np,edge,quantum_table,check_first,Ns,Nr,Nm,strategy);//run many_runs function
        clock_t end2 = clock();//finish clock
        double run_time = (double)(end2 - start2) / CLOCKS_PER_SEC;//calculate execution time

        std::cout<< "Time to play game is "<< ""<< run_time << "\n" ;//Print runtime

        win_percent = number_wins/Nr;//calculate win percentage



    return win_percent;
}


float genetic_fitness(std::vector<float> gene){
    float win_percent;//declare win percent variable
    float number_wins;//declare number of wins variable
    // GAME SETTINGS
    int Np =2;//Number of players variable
    int Ns =3;//Number of sites in the game
    int Nr = 1000000;//Number of runs of the game
    int Nm = 1;//Number of moves players are allowed to make
    bool check_first = false;//Check first or check later variant of the game
    bool edge = false;//Are players allowed to meet one edges
    const char graph[] = "cyclic";//What graph are we playing on
    const char strategy[] = "quantum";//What Strategy are the players using
    bool symmetric = true;// Symmetric strategy ? Only incorporated in genetic_fitness function right now
    bool classical_search = true;// This will allow us to use the genetic algorithm to find the classical bound of rendezvous game
    /////


    //clock_t start1 = clock();//Start clock
    //std::cout << "Ping1"; Debug statement
    std::vector<std::vector<float>> quantum_table = genetic_quantum_table(gene,Ns,Np,symmetric,check_first, classical_search);
    //  std::cout << "Ping2"; Debug statement
    //clock_t end1 = clock();//finish clock
    //double table_time = (double)(end1 - start1) / CLOCKS_PER_SEC;//calculate execution time
    //std::cout << "Time to generate table is " << "" << table_time << "\n";


   // clock_t start2 = clock();//Start clock
    number_wins = many_runs(graph,Np,edge,quantum_table,check_first,Ns,Nr,Nm,strategy);//run many_runs function - this is our fitness function
    //clock_t end2 = clock();//finish clock
   // double run_time = (double)(end2 - start2) / CLOCKS_PER_SEC;//calculate execution time
    //std::cout<< "Time to play game is "<< ""<< run_time << "\n" ;//Print runtime
    win_percent = number_wins/Nr;//calculate win percentage
    std::cout << "Win percent" << win_percent << "\n";
    return win_percent;
}




/////////////////////////////////////////GENETIC ALGORITHM//////////////////////////////////////

// Function to initialize a population of floats

/*
initialize_population - A function that initializes a population of vectors containing floats between 1 and 0

INPUT

populationSize -  INT - The total amount of individuals in this generation
chromosomeLength - INT - The length of the chromosome of an individual

OUTPUT

population - vector<Chromosome> - a 2 dimensional vector containing vectors that are the population of
                                   the current generation

*/

std::vector<Chromosome> initializePopulation(int populationSize, int chromosomeLength) {
    std::vector<Chromosome> population(populationSize, Chromosome(chromosomeLength));//declare the 2D array of a population size vectors all with length chromosome length
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);//allows for generation of numbers between 1 and 0

    for (auto& chromosome : population) {//create a new chromosome for each in the population
        for (auto& gene : chromosome) {//While accessing that chromosome generate a gene
            gene = dis(gen); // Random float between 0 and 1
        }
    }

    return population;
}


/*
Function - fitness_values - Evaluate the population of fitness functions and store the results in an vector

INPUT

population - std::vector<Chromosome> - a 2d vector that contains the genomes of the population


*/


std::vector<float> fitness_values(const std::vector<Chromosome>& population){
    std::vector<float> fitness_value_table;//Declare fitness value holder
    //std::cout << "ping inside"; Debug statement
    for(int i = 0; i < population.size(); ++i){//for i in the range of the size of the population
        fitness_value_table.push_back(genetic_fitness(population[i]));//Evaluate the genome using the fitness function
       // std::cout<<"ping number" << "" << i; Debug
    }
    return fitness_value_table;
}


// Function to perform crossover between two chromosomes

/*
Function - crossover - performs crossover between 2 parents genes

INPUT

parent 1 - Chromosome(1D vector float) pointer - points to the memory address of parent 1 (solution 1)
parent 2 - Chromosome(1D vector float) pointer - points to the memory address of parent 2 (solution 2)

OUTPUT

offspring - Chromosome(1D vector float) - the offspring of two individuals ( a new solution chromosome)
*/


Chromosome crossover(const Chromosome& parent1, const Chromosome& parent2) {
    std::random_device rd; //random gen
    std::mt19937 gen(rd());//set seed for randomness
    std::uniform_real_distribution<> dis(0.0, 1.0);//set distribution between 0 and 1

    Chromosome offspring(parent1.size());//We create a vector that represents the offspring using our chromosone type that is a 1D vector
    for (size_t i = 0; i < parent1.size(); ++i) {
        // Use random value to decide whether to take from parent1 or parent2
        offspring[i] = dis(gen) < 0.5 ? parent1[i] : parent2[i];//this is a conditional whether to pull from parent 1 or parent 2
    }

    return offspring;//return the offspring
}


/*
Function - mutate - performs a mutation on a chromosome

INPUT

chromosome - Chromosome(1D vector float) pointer - Points to the memory address of the individual chromosome
mutationRate - float - the number that decides if we mutate a gene of an individual or not

*/


void mutate(Chromosome& chromosome, float mutationRate) {
    std::random_device rd;//random device generator
    std::mt19937 gen(rd());//generate random
    std::uniform_real_distribution<> dis(0.0, 1.0);//set distribution between 0 and 1

    for (auto& gene : chromosome) {// for each gene (element of an individual chromosome)
        // Apply mutation with a given probability
        if (dis(gen) < mutationRate) {//if the random number generated is below the mutation rate
            gene = dis(gen); // mutate the gene and then move onto the next one in the chromosome
        }
    }
}



// Function to perform selection using tournament selection

/*
Function - tournamentSelection - A function that takes the input population and runs a tournament using the "winner stays on method"

INPUT
population     - vector<Chromosome>& - A pointer to the memory address of the 2D population vector
fitness_values - vector<float>& - A pointer to the memory address of the 1D vector that holds all of the
                                  fitness values for the chromosomes

tournamentSize - int - The number of rounds of the tournament that wish to be held to decide an optimal parent

OUTPUT

bestChromosome - Chromosome(1D vector float) - A 1d vector list containing the winning gene decided to be the parent gene

*/


Chromosome tournamentSelection(const std::vector<Chromosome>& population, const std::vector<float>& fitnessValues, int tournamentSize) {
    std::random_device rd;//random_device for generation of random
    std::mt19937 gen(rd());//random generator
    std::uniform_int_distribution<> dis(0, population.size() - 1);//picks an integer between 0 and the max population number(-1)

    // Randomly select chromosomes for the tournament
    size_t bestIndex = dis(gen);//randomly pick one
    float bestFitness = fitnessValues[bestIndex];;//pull out the fitness of the randomly picked individual

    //iterate through the population which is the tournament size
    for (int i = 1; i < tournamentSize; ++i) {//for i in tournament size (set via external parameter)
        size_t candidateIndex = dis(gen);//randomly pick a candidate from the population size
        float candidateFitness = fitnessValues[candidateIndex];//pull out this candidates fitness from the fitness value table
        if (candidateFitness > bestFitness) {//if the fitness of the new candidate is better than the old one
            bestIndex = candidateIndex;//best candidate is the new one
            bestFitness = candidateFitness;//best fitness is now the new ones fitness
        }
    }

    return population[bestIndex];//return the optimal chromosome from the tournament
}


std::string return_current_time_and_date(){
    auto now = std::chrono::system_clock::now();//Grab time clock
    auto in_time_t = std::chrono::system_clock::to_time_t(now);//turn clock time to real time

    std::stringstream ss;//declare a new string stream
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");//format
    return ss.str();//return time
}




int main() {
    float win_percent;
    int gene_length;
    int Ns = 3;
    std::string name = "cycle3_classical";
    bool symmetric = true;


    if(symmetric){
        gene_length = Ns*Ns;
    }
    else{
        gene_length = (Ns+Ns)*Ns;
    }
    //std::vector<float> test_gene = {0.0,0.0,0.0,0.0,0.166666,0.0,0.0,0.3333,0.0}; //Test vector that represents the optimal three site strategy on check later
     // Initial
     std::string time = return_current_time_and_date();
     std::cout << time;
     std::string gene_filename = name +"geneoutput_"+time+".txt";
     //std::cout << gene_filename;
     std::string data_filename = name+"datafile_"+time+ ".csv";

     std::string gene_directory = "data/genomes/";//gene directory
     std::string data_directory = "data/fitness_vals/";

     std::ofstream outFile(gene_directory+ gene_filename);//Open the output file for gene storage
     std::ofstream dataFile(data_directory + data_filename);//open up data storage file


    //std::cout<<"ping here"; Debug
    const int populationSize = 15;
    const int chromosomeLength = gene_length;
    const int generations = 100;
    const float mutationRate = 0.05;
    const int tournamentSize = 10;
   // std::cout<<"ping here 1"; Debug
    // Initialize the first population - two dimensional vector containing our geneomes
    std::vector<Chromosome> population = initializePopulation(populationSize, chromosomeLength);
    //Determine the fitness of our initial population
    std::vector<float> population_fitness = fitness_values(population);
    std::cout<<"Initial population evaluated" << "\n";

    dataFile << "Generation0" << ",";
    for(auto n : population_fitness){
        dataFile << n << ",";//write the first generation to a file after it has been generated
    }
        dataFile<< "\n";
    //std::cout<<"ping here 3"; Debug
    //For a set amount of generations
    for (int generation = 1; generation < generations; ++generation) {
        std::vector<Chromosome> newPopulation;//Create an empty 2D vector to hold our new genome population
        // Create new population
        while (newPopulation.size() < populationSize) {//While the new population is less that the old population
            Chromosome parent1 = tournamentSelection(population,population_fitness,tournamentSize);//Decide Parent 1 through tournament selection
            Chromosome parent2 = tournamentSelection(population,population_fitness, tournamentSize);//Decide Parent 2 through tournament selection


            Chromosome offspring = crossover(parent1, parent2);//Create offspring through crossover

            mutate(offspring, mutationRate);//Apply mutation to the offspring

            newPopulation.push_back(offspring);//Push the new offspring into the new population list
        }

        population = std::move(newPopulation);//After the new generation has been fully created, replace it with out
        population_fitness = fitness_values(population);//We can now re-evaluate the populations fitness
        //std::cout<<"ping"; debug statement

        //Now we pick the best one

        int counter = 0;//initialize counter variable
        float best_fitness =0.0;//initialize fitness variable
        dataFile << "Generation" << "" << generation << "," ;
        for(int j =0;j<population_fitness.size();++j){
            float fit = population_fitness[j];//pull out the fitness
                dataFile << fit << ",";

            if(fit>best_fitness){
                best_fitness = fit;
                counter = j;
            }
        }
        dataFile << "\n";//new line in data file



        std::cout << "Generation " << generation + 1 << ": Best Fitness = " << best_fitness << "\n";
        std::cout << "found at location" << counter << "\n";

        // Check if the file was opened successfully
        if (outFile.is_open()) {
            for (const auto& element : population[counter]){
                    outFile << element << "";

            }
            outFile << "" << "Winning probability is" << best_fitness;
            outFile << "\n";

        }


    }
    outFile.close();//Close the file
    dataFile.close();
    return 0;
}




/*
// DEbugging start code
int main(){
   // ize the GA with a population of chromosomes containing 10 floats

Quantum table_index debugger
   const char graph[] = "cyclic";
   int index = quantum_table_index(5,1,4,graph,false);
   std::cout << "The index is " << index << "\n";


    // std::cout << std::endl;
    //Testing of the gene vector

    //std::vector<float> test_gene = {0.96688,0.262552,0.89326,0.081536,0.764002,0.323055,0.98588,0.758399,0.00876851}; //test gene from genetic algorithm
    //float win_percent;
     std::vector<float> test_gene = {0.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
     float win_percent = genetic_fitness(test_gene);
     std::cout << "Win percentage is " << win_percent << "\n";

   // Below is a game where we know the strategy
     //win_percent = run_game(Np,Ns,Nm,check_first,edge,graph,strategy,symmetric);
     //std::cout << "Win percentage is " << win_percent << "\n";


    return 0;
}
*/
