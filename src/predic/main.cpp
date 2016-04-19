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

    if (!pOption->GetModelClassif()) {
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

string JoinFile(string out_dit, string file);

void CreateFile(string filename);

int main(int argc, char **argv) {

    // vytvoreni konstruktoru a prace se vstupem
    ProgramOption *opt = new ProgramOption(argc, argv);

    if (opt->finish != 0) {
        delete opt;
        return opt->finish;
    }

    if (opt->GetModeHelp()) {
        delete opt;
        return EXIT_SUCCESS;
    }

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

    std::string dbg_frame_dir = "frame/";
    std::string dbg_d_roc_file = "D_ROC.txt";
    std::string dbg_d_msr_file = "D_MSR.txt";

    std::string dbg_c_roc_file = "C_ROC.txt";
    std::string dbg_c_msr_file = "C_MSR.txt";


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
    else if (opt->GetMode() == 3) {
        WindowName = "Image";

        for (int i = 0; i < list_input.size(); ++i) {

            frame = imread(list_input.at(i));
            original = frame.clone();
            resize(frame, frame, Size(FRAME_WIDTH, FRAME_HEIGHT), INTER_CUBIC);

            std::vector<Rect> sign;
            sign = adet->Detection(frame, fps);

            if (!sign.empty() && !opt->GetModelClassif()) {
                classif->predic(frame, original, sign);
            }


            if (opt->GetModeShow()) {

                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);
            }

            string pat = "/tmp/bb/" + to_string(i) + ".jpg";
            imwrite(pat, frame);


        }
    }
    else if (opt->GetMode() == 4) {
        WindowName = "Cross-validation";

        // Create output directory for frame
        string full_path_frame = JoinFile(opt->GetOutputPath(), dbg_frame_dir);

        if (opt->GetModeDebug()) if (!opt->CreateDir(full_path_frame))
            cerr << "Do not create degug directory" << endl;

        // Create outputs for detector
        string d_roc_file = JoinFile(opt->GetOutputPath(), dbg_d_roc_file);
        string d_msr_file = JoinFile(opt->GetOutputPath(), dbg_d_msr_file);

        CreateFile(d_roc_file);
        CreateFile(d_msr_file);

        // Create outputs for clasificator
        string c_roc_file = JoinFile(opt->GetOutputPath(), dbg_c_roc_file);
        string c_msr_file = JoinFile(opt->GetOutputPath(), dbg_c_msr_file);

        CreateFile(c_roc_file);
        CreateFile(c_msr_file);


        if (!opt->GetModelClassif())
            classif->WriteLabel(c_roc_file);

        for (int i = 0; i < list_input.size(); ++i) {
            // Load image
            frame = imread(list_input.at(i));

            // Clone image
            original = frame.clone();

            // Detection
            std::vector<Rect> sign;
            sign = adet->DetectionCross(frame, d_roc_file, d_msr_file);

            // Classification
            if (!sign.empty() && !opt->GetModelClassif()) {
                classif->predicCross(original, sign, c_roc_file, c_msr_file);
            }

            // Show output
            if (opt->GetModeShow()) {
                adet->Draw_object(frame, sign);
                namedWindow(WindowName, WINDOW_AUTOSIZE);
                imshow(WindowName, frame);
                waitKey(1);
            }

            // Save frame
            if (opt->GetModeDebug()) {
                adet->Draw_object(frame, sign);
                boost::filesystem::path image = list_input.at(i);
                string pat;
                pat = full_path_frame + image.filename().string().c_str();
                imwrite(pat, frame);
            }
        }

    }
    else {
        cerr << "Sometimg input is wrong" << endl;
    }


    delete opt;
    delete adet;

    return 0;
}

void CreateFile(string filename) {
    ofstream file;
    file.open(filename, ios::out);
    file.close();
}

string JoinFile(string out_dit, string file) {
    boost::filesystem::path full_path_frame;

    boost::filesystem::path dir = out_dit;
    boost::filesystem::path filename = file;
    full_path_frame = dir / filename;

    return full_path_frame.string();
}

