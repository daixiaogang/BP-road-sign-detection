cmake_minimum_required(VERSION 3.3)
project(train)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp ")
find_package(Boost 1.58.0 COMPONENTS date_time program_options filesystem system REQUIRED)
find_package(OpenCV REQUIRED)

if (Boost_FOUND AND OpenCV_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")
    message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
endif ()

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

set(SOURCE_FILES main.cpp ProgramOption.cpp ProgramOption.h TrainSVM.cpp TrainSVM.h Library/SVM/svm.def Library/SVM/svm.h Library/SVM/svm.cpp Extractor.cpp Extractor.h TrainWaldBoost.cpp TrainWaldBoost.h
        Library/xobject/precomp.hpp Library/xobject/waldboost.cpp Library/xobject/waldboost.hpp Library/xobject/feature_evaluator.hpp/ Library/xobject/feature_evaluator.cpp Library/xobject/lbpfeatures.cpp
        Library/xobject/lbpfeatures.h Library/xobject/cascadeclassifier.h)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(train ${SOURCE_FILES})
    target_link_libraries(train ${Boost_LIBRARIES} ${OpenCV_LIBS})
endif ()



