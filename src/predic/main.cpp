#include <iostream>
#include "ProgramOption.h"
#include "ABoostDetection.h"


using namespace std;

bool InitModels(ABoostDetection *p_adet) {

    if (!p_adet->LoadModel()) {
        cerr << "ERROR: " << "Loading  detector model of adaboost" << endl;
        return false;
    }
    cout << "Model Adaboost is loaded" << endl;


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

    //nacteni model
    if (!InitModels(adet)) {
        delete opt;
        delete adet;
        return EXIT_FAILURE;
    }

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

            adet->Detection(frame, fps);
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

                resize(frame, frame, Size(640, 480), INTER_CUBIC);
                adet->Detection(frame, fps);
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

            adet->Detection(frame, fps);
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




