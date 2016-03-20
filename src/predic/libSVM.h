//
// Created by maiikeru on 4/21/15.
//

#ifndef PROGRAM_LIBSVM_H
#define PROGRAM_LIBSVM_H

#include <iostream>

namespace libsvm {
#include "svm.h"
}

using namespace libsvm;
using namespace std;

class libSVM {

private:
    struct svm_model *model;


public:
    bool usedModel = false;
    static libSVM *getInstance();

    struct svm_parameter param;

    virtual ~libSVM() {

    }

    svm_model * loadModelFromFile(const string modelFileName);

    void freeMem();

    void saveModelToFile(const std::string _modelFileName);
};

/// Singleton
libSVM *libSVM::getInstance() {
    static libSVM theInstance;
    return &theInstance;
}




svm_model * libSVM::loadModelFromFile(const string _modelFileName) {

    this->freeMem();
    /// @WARNING: This is really important, ROS seems to set the system locale which takes decimal commata instead of points which causes the file input parsing to fail
    // Do not use the system locale setlocale(LC_ALL, "C");
    setlocale(LC_NUMERIC, "C");
    setlocale(LC_ALL, "POSIX");
    /// @TODO Test if this works as intended
    if (this->usedModel && model != NULL) {
//            svm_free_and_destroy_model(&model);
        delete model; // clear the previous model and deallocate/free the occupied memory
        model = NULL; // set the pointer to NULL
    }
    printf("Loading model from file '%s'\n", _modelFileName.c_str());
    model = svm_load_model(_modelFileName.c_str());
    if (model == NULL) {
        printf("Loading of SVM model failed!\n");
        exit(EXIT_FAILURE);
    }

    int classNr = svm_get_nr_class(model);
    int labels[classNr];
    for (int i = 0; i < classNr; ++i) {
        labels[i] = 0;
    }
    svm_get_labels(model, labels);
    double b = -(model->rho[0]);
    //double probA = (model->probA[0]);
    double probA = 0.0;
    //double probB = (model->probB[0]);
    double probB = 0.0;
    int kernelType = model->param.kernel_type;
    printf("Loaded model: SVM type %d, Kernel type %d, %d classes: labels %d, %d, #SVs %d, bias b %3.5f, probA %3.5f, probB %3.5f\n",
           svm_get_svm_type(model), kernelType, classNr, labels[0], labels[1], model->l, b, probA, probB);
    this->usedModel = true;
    return model;
}

void libSVM::freeMem() {
    svm_destroy_param(&this->param);
    svm_free_and_destroy_model(&model);
}

void libSVM::saveModelToFile(const std::string _modelFileName) {
    if (svm_save_model(_modelFileName.c_str(), getInstance()->model)) {
        fprintf(stderr, "Error: Could not save model to file %s\n", _modelFileName.c_str());
        exit(EXIT_FAILURE);
    }
}
#endif //PROGRAM_LIBSVM_H