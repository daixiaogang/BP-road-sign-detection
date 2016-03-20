//
// Created by maiikeru on 3/17/16.
//

#ifndef PREDIC_CLASSIFICATION_H
#define PREDIC_CLASSIFICATION_H
#include <iostream>

using namespace std;

class Classification {
private:
    struct svm_model *ModelClasificator;
    string model_file_name;



public:



    Classification(string model_file);
};


#endif //PREDIC_CLASSIFICATION_H
