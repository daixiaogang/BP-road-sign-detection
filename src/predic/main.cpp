#include <iostream>
#include "ProgramOption.h"
#include "ABoostDetection.h"
//#include "Classification.h"
#include "libSVM.h"
#include "extractor.h"

using namespace std;
using namespace cv;


class Classify {

private:

    struct svm_model *ModelClasificator;

public:

    void load_modelLibClass(const string model);


    double detekuj(vector<float> value);

    double detekuj_prob(vector<float> value);


    vector<float> extractHog(Rect_<int> &vector, Mat mat);
};


void Classify::load_modelLibClass(const string model) {
    cout << "Loading model for LibSVM...." << endl;


    if ((this->ModelClasificator = libSVM::getInstance()->loadModelFromFile(model)) == 0) {
        cerr << "Can't load SVM model " << model << endl;
    }
}


bool InitModels(ABoostDetection *p_adet) {

    if (!p_adet->LoadModel()) {
        cerr << "ERROR: " << "Loading  detector model of adaboost:" << endl;
        return false;
    }
    cout << "Model Adaboost is loaded" << endl;


    /*
    if (!pClassification->LoadModel()){
        cerr << "ERROR: " << "Loading  classificator model of svm" << endl;
        return false;
    }
    cout << "Model SVM is loaded" << endl;
*/
    return true;
}


bool InitVideoCapture(VideoCapture *pCapture, int input_cam) {

    pCapture->open(input_cam);
    return pCapture->isOpened();
}

bool InitVideoCapture(VideoCapture *pCapture, string input_file) {

    pCapture->open(input_file);
    return pCapture->isOpened();
}


int main(int argc, char **argv) {

    // vytvoreni konstruktoru a prace se vstupem
    ProgramOption *opt = new ProgramOption(argc, argv);

    // nacteni a kontrola vstunich dat
    vector<string> list_input;
    list_input = opt->GetVectorOfFiles();

    // vytvoreni konruktoru ori aboost detektor
    ABoostDetection *adet = new ABoostDetection(opt->GetPathModelAboost());

    //Classification *classify = new Classification(opt->GetPathModelSVM());

    //nacteni model
    if (!InitModels(adet)) {
        delete opt;
        delete adet;
        return EXIT_FAILURE;
    }


    Classify *classif = new Classify();
    classif->load_modelLibClass(opt->GetPathModelSVM());

    time_t start, end;

    // fps calculated using number of frames / seconds
    double fps = 0;

    // frame counter
    int counter = 0;

    // floating point seconds elapsed since start
    double sec;


    VideoCapture capture;
    Mat frame;

    if (opt->GetMode() == 1) {
        if (!InitVideoCapture(&capture, opt->GetCameraRun())) {
            cerr << "RROOR load camera: " << opt->GetCameraRun() << endl;

            delete opt;
            delete adet;
            return EXIT_FAILURE;
        }

        // start the clock
        time(&start);

        while (capture.read(frame)) {
            if (frame.empty()) {
                printf(" --(!) No captured frame -- Break!");
                break;
            }

            resize(frame, frame, Size(640, 480), INTER_CUBIC);


            std::vector<Rect> sign;
            std::vector<float> descriptors;

            sign = adet->Detection(frame, fps);

            if (!sign.empty()) {

                for (int i = 0; i < sign.size(); ++i) {

                    descriptors = classif->extractHog(sign.at(i), frame);

                    //cout<<classif->detekuj(descriptors);
                    cout << classif->detekuj_prob(descriptors);
                }
            }



            // see how much time has elapsed
            time(&end);
            // calculate current FPS
            ++counter;
            sec = difftime(end, start);

            fps = counter / sec;

            // overflow protection
            if (counter == (INT_MAX - 1000))
                counter = 0;
        }
    } else if (opt->GetMode() == 2) {

        for (int i = 0; i < list_input.size(); ++i) {
            cout << list_input.at(i) << endl;

            if (!InitVideoCapture(&capture, list_input.at(i))) {
                cerr << "RROOR load file: " << list_input.at(i) << endl;

                delete opt;
                delete adet;
                return EXIT_FAILURE;
            }

            // start the clock
            time(&start);
            counter = 0;

            while (capture.read(frame)) {
                if (frame.empty()) {
                    printf(" --(!) No captured frame -- Break!");
                    break;
                }

                std::vector<Rect> sign;
                std::vector<float> descriptors;

                sign = adet->Detection(frame, fps);

                if (!sign.empty()) {

                    for (int i = 0; i < sign.size(); ++i) {

                        descriptors = classif->extractHog(sign.at(i), frame);

                        //cout<<classif->detekuj(descriptors);
                        cout << classif->detekuj_prob(descriptors);
                    }
                }
                // see how much time has elapsed
                time(&end);
                // calculate current FPS
                ++counter;
                sec = difftime(end, start);

                fps = counter / sec;

                // overflow protection
                if (counter == (INT_MAX - 1000))
                    counter = 0;


            }
        }
    }
    else {
        for (int i = 0; i < list_input.size(); ++i) {
            cout << list_input.at(i) << endl;

            frame = imread(list_input.at(i));

            // start the clock
            time(&start);
            counter = 0;
            std::vector<Rect> sign;
            std::vector<float> descriptors;

            sign = adet->Detection(frame, fps);

            if (!sign.empty()) {

                for (int i = 0; i < sign.size(); ++i) {

                    descriptors = classif->extractHog(sign.at(i), frame);

                    //cout<<classif->detekuj(descriptors);
                    cout << classif->detekuj_prob(descriptors);
                }
            }
            // see how much time has elapsed
            time(&end);
            // calculate current FPS
            ++counter;
            sec = difftime(end, start);

            fps = counter / sec;

            // overflow protection
            if (counter == (INT_MAX - 1000))
                counter = 0;


        }
    }


    delete opt;
    delete adet;

    return 0;
}


vector<float> Classify::extractHog(Rect_<int> &faces, Mat mat) {
    Mat cropedImage = mat(Rect(faces.x, faces.y, faces.width, faces.height));

    std::vector<float> descriptors;
    descriptors = getHog(cropedImage);

    return descriptors;
}

double Classify::detekuj(vector<float> value) {

    struct svm_node *svmVecT;
    svmVecT = (struct svm_node *) malloc((value.size() + 1) * sizeof(struct svm_node));
    int i;
    for (i = 0; i < value.size(); i++) {
        svmVecT[i].index = i + 1;
        svmVecT[i].value = value.at(i);
    }

    svmVecT[i].index = -1;   // End of line

    double result = svm_predict(this->ModelClasificator, svmVecT);
    return result;
}

double Classify::detekuj_prob(vector<float> value) {
    struct svm_node *svmVecT;
    svmVecT = (struct svm_node *) malloc((value.size() + 1) * sizeof(struct svm_node));
    int i;
    for (i = 0; i < value.size(); i++) {
        svmVecT[i].index = i + 1;
        svmVecT[i].value = value.at(i);
    }

    svmVecT[i].index = -1;   // End of line

    double prob[1000];


    double result = svm_predict_probability(this->ModelClasificator, svmVecT, prob);

    cout << " : " << prob[0] << prob[1] << prob[2] << endl;

    return result;
}
