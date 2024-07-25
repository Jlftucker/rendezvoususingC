#include <cppsim/state.hpp>

int main(){
    unsigned int n = 5;
    QuantumState state(n);
    state.set_zero_state();
    return 0;
}