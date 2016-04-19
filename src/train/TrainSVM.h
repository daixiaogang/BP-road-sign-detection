//
// Created by maiikeru on 4/17/16.
//

#ifndef TRAIN_TRAINSVM_H
#define TRAIN_TRAINSVM_H


#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <vector>

namespace libsvm {
#include "Library/SVM/svm.h"
}

#define Malloc(type, n) (type *)malloc((n)*sizeof(type))

class TrainSVM {


private:
    struct libsvm::svm_problem prob;

    struct libsvm::svm_model *model;

    struct libsvm::svm_node *x_space;


    bool trainingData;
    bool predictionData;

    int predict_probability;
    int max_nr_attr;
    char *line;
    size_t max_line_len;

    char* readline(FILE *input);

    void exit_input_error(int line_num);

public:

    TrainSVM();

    ~TrainSVM();


    void freeMem();

    libsvm::svm_parameter param;


    void read_problem(char *filename);

    void inicialization();

    void train();

    void saveModelToFile(const std::string _modelFileName);


};


#endif //TRAIN_TRAINSVM_H
