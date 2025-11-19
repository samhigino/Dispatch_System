#include <fstream>
#include <iostream>
#include <chrono>
#include "2D_point.hpp"

using namespace std;

int main() {
    ifstream in("tests/input_1.txt", fstream::in);
    ofstream out("output/output.txt", fstream::out);
    ofstream debug("output/debug.txt", fstream::out);

    if(!debug.is_open()) {
        cout << "Debug file did not open." << endl;
        return 1;
    }

    if(!in.is_open() || !out.is_open()) {
        debug << "Error opening input or output." << endl;
        return 2;
    }
    
    debug << "Beginning tests.\n";
    
    return 0;
}