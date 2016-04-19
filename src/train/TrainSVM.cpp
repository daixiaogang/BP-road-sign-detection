//
// Created by maiikeru on 4/17/16.
//

#include "TrainSVM.h"

TrainSVM::TrainSVM() {

}

TrainSVM::~TrainSVM() {
    this->freeMem();
}

void TrainSVM::freeMem() {

    if (this->trainingData) {
        free(prob.y);
        free(prob.x);
        free(x_space);
        free(line);
    }
    libsvm::svm_destroy_param(&this->param);
    libsvm::svm_free_and_destroy_model(&model);


}

void TrainSVM::read_problem(char *filename) {
    setlocale(LC_NUMERIC, "C");
    setlocale(LC_ALL, "POSIX");

    int elements, max_index, inst_max_index, i, j;
    FILE *fp = fopen(filename, "r");

    char *endptr;
    char *idx, *val, *label;

    if (fp == NULL) {
        fprintf(stderr, "can't open input file %s\n", filename);
        exit(1);
    }

    prob.l = 0;
    elements = 0;

    max_line_len = 50000;
    line = Malloc(char, max_line_len);
    while (readline(fp) != NULL) {
        char *p = strtok(line, " \t"); // label

        // features
        while (1) {
            p = strtok(NULL, " \t");
            if (p == NULL || *p == '\n')
                break;
            ++elements;
        }
        ++elements;
        ++prob.l;
    }
    rewind(fp);

    prob.y = Malloc(double, prob.l);
    prob.x = Malloc(struct libsvm::svm_node *, prob.l);
    x_space = Malloc(struct libsvm::svm_node, elements);

    max_index = 0;
    j = 0;
    for (i = 0; i < prob.l; i++) {
        inst_max_index = -1;
        readline(fp);
        prob.x[i] = &x_space[j];
        label = strtok(line, " \t\n");
        if (label == NULL) {
            printf("Empty line encountered!\n");
            exit_input_error(i + 1);
        }

        prob.y[i] = strtod(label, &endptr);
        if (endptr == label || *endptr != '\0') {
            printf("Wrong line ending encountered!\n");
            exit_input_error(i + 1);
        }

        while (1) {
            idx = strtok(NULL, ":");
            val = strtok(NULL, " \t");

            if (val == NULL)
                break;

            errno = 0;
            x_space[j].index = (int) strtol(idx, &endptr, 10);
            if (endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index) {
                printf("File input error at feature index encountered: %i, %i, %i, %i!\n", endptr == idx, errno != 0,
                       *endptr != '\0', x_space[j].index <= inst_max_index);
                exit_input_error(i + 1);
            } else {
                inst_max_index = x_space[j].index;

            }


            errno = 0;
            x_space[j].value = std::strtod(val, &endptr);

            if (endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr))) {
                printf("File input error at feature value encountered: %i, %i, %i: '%s'\n", endptr == val, errno != 0,
                       (*endptr != '\0' && !isspace(*endptr)), endptr);
                exit_input_error(i + 1);
            }
            ++j;
        }

        if (inst_max_index > max_index)
            max_index = inst_max_index;
        x_space[j++].index = -1;
    }

    if (param.gamma == 0 && max_index > 0) {
        param.gamma = 1.0 / max_index;
    }
    if (param.kernel_type == libsvm::PRECOMPUTED) {
        for (i = 0; i < prob.l; i++) {
            if (prob.x[i][0].index != 0) {
                fprintf(stderr, "Wrong input format: first column must be 0:sample_serial_number\n");
                exit(1);
            }
            if ((int) prob.x[i][0].value <= 0 || (int) prob.x[i][0].value > max_index) {
                fprintf(stderr, "Wrong input format: sample_serial_number out of range\n");
                exit(1);
            }
        }
    }
    fclose(fp);
    this->trainingData = true;
}


char *TrainSVM::readline(FILE *input) {
    int len;

    if (fgets(line, max_line_len, input) == NULL)
        return NULL;

    while (strrchr(line, '\n') == NULL) {
        max_line_len *= 2;
        line = (char *) realloc(line, max_line_len);
        len = (int) strlen(line);
        if (fgets(line + len, max_line_len - len, input) == NULL)
            break;
    }
    return line;
}

void TrainSVM::exit_input_error(int line_num) {
    fprintf(stderr, "Wrong input format at line %d\n", line_num);
    exit(1);
}

void TrainSVM::train() {

    this->inicialization();

    this->model = svm_train(&prob, &param);
    this->trainingData = true;
}

void TrainSVM::inicialization() {

    this->line = NULL;
    this->max_nr_attr = 1;
    this->predict_probability = 1;

    this->param.cache_size = 512;
    this->param.coef0 = 0.0;
    this->param.degree = 3;
    this->param.eps = 1e-3;
    this->param.gamma = 0.001;
    this->param.kernel_type = 2;
    this->param.nr_weight = 0;
    this->param.nu = 0.5;
    this->param.p = 0.1;
    this->param.probability = this->predict_probability;
    this->param.C = 10;
    this->param.shrinking = 0;
    this->param.svm_type = libsvm::C_SVC;
    this->param.weight_label = NULL;
    this->param.weight = NULL;


    this->model = NULL;


}

void TrainSVM::saveModelToFile(const std::string _modelFileName) {
    if (libsvm::svm_save_model(_modelFileName.c_str(), this->model)) {
        fprintf(stderr, "Error: Could not save model to file %s\n", _modelFileName.c_str());
        exit(EXIT_FAILURE);
    }
}
