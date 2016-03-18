//
// Created by maiikeru on 3/17/16.
//

#ifndef PREDIC_CLASSIFICATION_H
#define PREDIC_CLASSIFICATION_H
#include <iostream>



using namespace std;

class Classification {
private:
    string model_file_name;
    struct svm_model *ModelClasificator;

public:
    Classification(string model_file);
    bool LoadModel();
};


#endif //PREDIC_CLASSIFICATION_H
