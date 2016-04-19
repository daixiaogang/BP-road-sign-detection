#include <iostream>
#include "ProgramOption.h"
#include "TrainSVM.h"
#include "Extractor.h"
#include "TrainWaldBoost.h"

using namespace std;

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;

    ProgramOption *opt = new ProgramOption(argc, argv);


    int mode = 3;


    if (mode == 0) {
        std::cout << "SVM\n";
        TrainSVM *svm = new TrainSVM();

        std::cout << "Read model\n";
        svm->read_problem((char *) "/home/maiikeru/Downloads/model_hog/train0.t");

        std::cout << "Training model\n";
        svm->train();

        std::cout << "Save model\n";
        svm->saveModelToFile("/tmp/model");

        free(svm);

    }
    else if (mode == 1) {
        std::cout << "Extractor \n";

        Extractor *ext = new Extractor();

        std::string in_dir = "/home/maiikeru/Downloads/GTSRB/Final_Training/Images/";
        std::string out_dir = "tmp/A/";


        ext->walk_directory("/home/maiikeru/Downloads/GTSRB/Final_Training/Images/", "tmp/A/");


        free(ext);
    }
    else if (mode == 2){
        std::cout << "Waldboost \n";

        TrainWaldBoost *wald = new TrainWaldBoost();

       // wald->train("pos", "neg", "model");

        //wald->predic("model", "in", "out", "label");

    }


    else if (mode == 3){
        std::cout << "Cascade \n";
    }


    free(opt);


    return 0;
}