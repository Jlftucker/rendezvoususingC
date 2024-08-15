
#include <iostream>
#include "qpp/qpp.h"

int tee() {
    using namespace qpp;
    cmat H = gt.H; // Hadamard gate
    ket psi0 = 0_ket; // |0>
    ket result = H * psi0; // Apply H to |0>
    std::cout << "Resulting state: \n" << disp(result) << "\n";
    return 0;
}
