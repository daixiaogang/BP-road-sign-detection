#include <iostream>
#include "ProgramOption.h"


using namespace std;


int main(int argc, char **argv) {

    ProgramOption *opt;

    vector<string> list_input;

    opt = new ProgramOption(argc, argv);


    delete opt;

    return 0;
}