//
// Created by maiikeru on 3/17/16.
//

#include "Classification.h"

Classification::Classification(string model_file) {
    this->model_file_name = model_file;
}

bool Classification::LoadModel() {
    cout << "Loading model for LibSVM...." << endl;

    /*
    if ((this->ModelClasificator = libSVM::getInstance()->loadModelFromFile(this->model_file_name)) == 0) {
        cerr << "Can't load SVM model " << this->model_file_name << endl;
    }
    */
    return false;
}
