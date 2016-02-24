#include "boost/program_options.hpp"

#include <iostream>
#include <string>

namespace {
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace


using namespace std;

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;

    string input;
    string output;
    string annotation;

    int camera = 0;
    bool show = false, no_class = false, clasify = false, debug = false, cross = false;
    bool image_mode = false, video_mode = false, cam_mode = false;
    try {
        /** Define and parse the program options
         */
        namespace po = boost::program_options;
        po::options_description desc("Options");
        //po::variables_map vm;
        boost::program_options::variables_map vm;

        try {
            desc.add_options()
                    ("image,i", po::value<string>(&input),
                     "File or directory input images. If not chosen than default camera mode run.")
                    ("video,v", po::value<string>(&input),
                     "File or directory input videos. If not chosen than default camera mode run.")
                    ("camera,e", po::value<int>(&camera), "Selectin of the camera. Default 0.")
                    ("output,o", po::value<string>(&input), "Directory output result")
                    ("show,s", "Show current image or frame by video")
                    ("annotation,a", po::value<string>(&annotation), "File of output annotation")
                    ("classification,c", "Make only classification")
                    ("no_classification,n", "Make only detection")
                    ("model_detect,t", "File of model  detection")
                    ("model_classif,f", "File of model  clasificatin")
                    ("help,h", "Print help messages")
                    ("cross_validation,r", "Cross-validation")
                    ("debug,g", "Debug mode");
            po::store(po::parse_command_line(argc, argv, desc), vm); // can throw


            /** --help option
             */
            if (vm.count("help")) {
                std::cout << "Basic Command Line Parameter App" << std::endl
                << desc << std::endl;
                cout << "help" << endl;
                return SUCCESS;
            }

            if (vm.count("show")) {
                show = true;
                cout << "show" << endl;
            }

            if (vm.count("classification") && vm.count("no_classification")) {
                cerr << "ERROR: " << "Can be only one parameter --classification|--no_classification" << endl;
                return ERROR_IN_COMMAND_LINE;
            }

            if (vm.count("classification")) {
                cout << "classif" << endl;
                clasify = true;
            }

            if (vm.count("no_classification")) {
                no_class = true;
                cout << "no clasiif" << endl;
            }

            cout << vm.count("image") << "JJ" << vm.count("video") << "KK" << vm.count("camera") << endl;

            if (!vm.count("image") && (!vm.count("video"))) {
                cam_mode = true;
                cout << "camera mode" << endl;
            }
            else if (vm.count("image") && !vm.count("video") && !vm.count("camera")) {
                image_mode = true;
                cout << "image mode" << endl;
            }
            else if (!vm.count("image") && vm.count("video") && !vm.count("camera")) {
                video_mode = true;
                cout << "video mode" << endl;
            }
            else {
                cerr << "ERROR: " << "Can be only one parameter --image|--video|--camera" << endl;
                return ERROR_IN_COMMAND_LINE;
            }

            if (vm.count("cross_validation")) {
                cross = true;
                cout << "cross_validaton" << endl;
            }


            if (vm.count("debug")) {
                debug = true;
                cout << "debug" << endl;
            }

            po::notify(vm); // throws on error, so do after help in case
            // there are any problems
        }
        catch (po::error &e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << desc << std::endl;
            return ERROR_IN_COMMAND_LINE;
        }

        // application code here //

    }
    catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: "
        << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;

    }


    return 0;
}