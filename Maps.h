
#ifndef INTERPRETER_MAPS_H
#define INTERPRETER_MAPS_H

#include <unordered_map>
#include "Number.h"
#include <vector>
#include <string>

using namespace std;
class Maps {
public:
    /* a map between variable names and pointers to the corresponding Number
     * objects (only for variables which aren't bound to paths) */
    static unordered_map<string, Number *> symbolTable;
    // a map between paths in the simulator and their corresponding raw values
    static unordered_map<string, double> rawValues;
    /* a map between bound variable names and their corresponding paths in the
     * simulator to which they're bound*/
    static unordered_map<string, string> boundVars;
    /* a vector that contains all the paths from the xml in the correct order */
    static vector<string> allPaths;
};


#endif //INTERPRETER_MAPS_H
