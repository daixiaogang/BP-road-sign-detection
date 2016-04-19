#include <iostream>
#include "ProgramOption.h"
#include "ABoostDetection.h"
#include "Classification.h"

using namespace std;
using namespace cv;

bool InitModels(ABoostDetection *p_adet, Classification *pClassification, ProgramOption *pOption) {

    if (!p_adet->LoadModel()) {
        cerr << "ERROR: " << "Loading  detector model of adaboost:" << endl;
        return false;
    }
    cout << "Model Adaboost is loaded" << endl;

    if (!pOption->GetModelClassif()){
        if (!pClassification->loadModel()) {
            cerr << "ERROR: " << "Loading  classificator model of svm" << endl;
            return false;
        }
        cout << "Model SVM is loaded" << endl;
    }

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

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

int main(int argc, char **argv) {

    // vytvoreni konstruktoru a prace se vstupem
    ProgramOption *opt = new ProgramOption(argc, argv);

    // nacteni a kontrola vstunich dat
    vector<string> list_input;
    list_input = opt->GetVectorOfFiles();

    // vytvoreni konruktoru ori aboost detektor
    ABoostDetection *adet = new ABoostDetection(opt->GetPathModelAboost());

    Classification *classif = new Classification(opt->GetPathModelSVM());

    //nacteni model
    if (!InitModels(adet, classif, opt)) {
        delete opt;
        delete adet;
        return EXIT_FAILURE;
    }



    vector<Mat> classif_icon = classif->GetImgIcon();

    time_t start, end;

    // fps calculated using number of frames / seconds
    double fps = 0;

    // frame counter
    int counter = 0;

    // floating point seconds elapsed since start
    double sec;


    VideoCapture capture;
    Mat frame;
    Mat original;
    Mat small_image;
    int cont = 84;
    string WindowName;

    if (opt->GetMode() == 1) {
        WindowName = "Camera";
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

            resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);
            original = frame.clone();

            std::vector<Rect> sign;

            sign = adet->Detection(frame, fps);


            if (!sign.empty() && !opt->GetModelClassif()) {
                classif->predic(frame, original, sign);
            }

            if (opt->GetModeShow()) {

                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);

                //imshow("window_name", frame);
                //waitKey(1);
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
        WindowName = "Video";

        for (int i = 0; i < list_input.size(); i++) {
            cout << list_input.at(i) << ":" << list_input.size() << endl;
            if (!InitVideoCapture(&capture, list_input.at(i))) {
                cerr << "RROOR load file: " << list_input.at(i) << endl;

                delete opt;
                delete adet;
                return EXIT_FAILURE;
            }

            cont += 1;
            string pat = "/tmp/" + to_string(i) + ".avi";
            VideoWriter video(pat, CV_FOURCC('D', 'I', 'V', 'X'), 20, Size(640, 480), true);

            // start the clock
            counter = 0;
            fps = 0;
            double t, td, tc, te;

            while (capture.read(frame)) {
                t = (double) getTickCount();

                if (frame.empty()) {
                    printf(" --(!) No captured frame -- Break!");
                    break;
                }


                resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);
                original = frame.clone();

                std::vector<Rect> sign;
                std::vector<float> descriptors;

                sign = adet->Detection(frame, fps);

                string patr = "/tmp/cc/" + to_string(counter) + ".jpg";
                imwrite(patr, frame);

                if (!sign.empty() && !opt->GetModelClassif()) {
                    for (int j = 0; j < sign.size(); ++j) {

                        Mat cropedImage = original(
                                Rect(sign.at(j).x, sign.at(j).y, sign.at(j).width, sign.at(j).height));
                        imwrite("/tmp/aa/" + to_string(counter) + ".jpg", cropedImage);

                        td = (double) getTickCount();

                        descriptors = classif->extractHog(sign.at(j), original);
                        td = ((double) getTickCount() - td) / getTickFrequency();

                        tc = (double) getTickCount();

                        double index = classif->detekuj_prob(descriptors);
                        tc = ((double) getTickCount() - tc) / getTickFrequency();

                        cout << index << endl;

                        te = (double) getTickCount();
                        if (classif->CheckRoi(sign.at(j).x, sign.at(j).height)) {
                            small_image = classif_icon.at(abs((int) index) - 1);
                            resize(small_image, small_image, Size(sign.at(j).height, sign.at(j).width), INTER_CUBIC);
                            small_image.copyTo(
                                    frame(cv::Rect(sign.at(j).x - sign.at(j).height, sign.at(j).y, small_image.cols,
                                                   small_image.rows)));
                            small_image.release();
                        }
                        te = ((double) getTickCount() - te) / getTickFrequency();


                    }

                    string patr = "/tmp/dd/" + to_string(counter) + ".jpg";
                    imwrite(patr, frame);
                }
                else {
                    td = tc = te = 0;
                }

                if (opt->GetModeShow()) {

                    namedWindow(WindowName, WINDOW_AUTOSIZE);
                    imshow(WindowName, frame);
                    waitKey(1);

                }

                video.write(frame);


                //sign.clear();
                descriptors.clear();
                frame.release();
                original.release();

                t = ((double) getTickCount() - t) / getTickFrequency();

                cout << "QQ:" << t << ":" << 1 / t << ":" << td << ":" << tc << ":" << te << endl;

                fps = 1 / t;
                counter++;
            }


            capture.release();
            video.release();


        }
    }
    else {
        WindowName = "Image";
        for (int i = 0; i < list_input.size(); ++i) {
            cout << list_input.at(i) << endl;

            frame = imread(list_input.at(i));
            original = frame.clone();

            //resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);

            // start the clock
            time(&start);
            counter = 0;
            std::vector<Rect> sign;
            std::vector<float> descriptors;

            sign = adet->DetectionCross(frame);

            if (!sign.empty() && !opt->GetModelClassif()) {
                for (int j = 0; j < sign.size(); ++j) {

                    Mat cropedImage = original(Rect(sign.at(j).x, sign.at(j).y, sign.at(j).width, sign.at(j).height));
                    imwrite("/tmp/aa/" + to_string(i) + ".jpg", cropedImage);


                    descriptors = classif->extractHog(sign.at(j), original);


                    double index = classif->detekuj_prob(descriptors);

                    cout << index << endl;


                    if (classif->CheckRoi(sign.at(j).x, sign.at(j).height)) {
                        small_image = classif_icon.at( index) ;
                        resize(small_image, small_image, Size(sign.at(j).height, sign.at(j).width), INTER_CUBIC);
                        small_image.copyTo(
                                frame(cv::Rect(sign.at(j).x , sign.at(j).y, small_image.cols,
                                               small_image.rows)));
                        small_image.release();
                    }

                }
            }


            if (opt->GetModeShow()) {

                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);
            }

            string pat = "/tmp/bb/" + to_string(i) + ".jpg";
            imwrite(pat, frame);


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

