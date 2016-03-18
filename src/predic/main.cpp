#include <iostream>
#include "ProgramOption.h"
#include "ABoostDetection.h"
#include "Classification.h"

bool InitModels(ABoostDetection *p_adet);

using namespace std;

bool InitModels(ABoostDetection *p_adet) {

    if (!p_adet->LoadModel()){
        cerr << "ERROR: " << "Loading  detector model of adaboost" << endl;
        return false;
    }
    cout<<"Model Adaboost is loaded"<<endl;


    return true;
}

int main(int argc, char **argv) {

    // vytvoreni konstruktoru a prace se vstupem
    ProgramOption *opt = new ProgramOption(argc, argv);

    // nacteni a kontrola vstunich dat
    vector<string> list_input;
    list_input = opt->GetVectorOfFiles();

    // vytvoreni konruktoru ori aboost detektor
    ABoostDetection *adet = new ABoostDetection(opt->GetPathModelAboost());

    //nacteni model

    if (!InitModels(adet)){
        delete opt;
        delete adet;
        return EXIT_FAILURE;
    }




    delete opt;
    delete adet;

    return 0;
}

