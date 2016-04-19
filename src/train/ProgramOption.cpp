//
// Created by maiikeru on 4/14/16.
//

#include "ProgramOption.h"

ProgramOption::ProgramOption(int argc, char **argv) {
    InitDefaultValues();

    ParseArgument(argc, argv);
}

ProgramOption::~ProgramOption() {


}

void ProgramOption::InitDefaultValues() {

}

int ProgramOption::ParseArgument(int argc, char **argv) {

    namespace po = boost::program_options;
    // Declare the supported options.
    int opt;
    // Declare a group of options that will be
    // allowed only on command line
    po::options_description generic("Generic options");
    generic.add_options()
            ("version,v", "print version string")
            ("help,h", "produce help message");

    // Declare a group of options that will be
    // allowed both on command line and in
    // config file
    po::options_description config("Extraction");
    config.add_options()
            ("HOG",
             "Hog feature")
            ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden("SVM training");
    hidden.add_options()
            ("SVM", "SVM training");

    po::options_description cascade("Cascade clasifier");
    cascade.add_options()
            ("Cascade","Cascade classifier")
            ;


    po::options_description waldboost("Waldboost trainig");
    waldboost.add_options()
            ("WaldBoost","WaldBoost trainig")
            ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden).add(cascade).add(waldboost);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden).add(cascade).add(waldboost);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config).add(hidden).add(cascade).add(waldboost);




    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, visible), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << visible << "\n";
        return 1;
    }

    if (vm.count("optimization")) {
        std::cout << "optimization level was set to "
        << vm["optimization"].as<int>() << ".\n";
    } else {
        std::cout << "optimization level was not set.\n";
    }

    return 0;
}
