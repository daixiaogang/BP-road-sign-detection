cmake_minimum_required(VERSION 3.3)
project(predic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
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

set(SOURCE_FILES main.cpp ProgramOption.cpp ProgramOption.h FileOperation.cpp FileOperation.h ABoostDetection.cpp
        ABoostDetection.h Classification.cpp Classification.h libSVM.h svm.cpp svm.def svm.h extractor.cpp extractor.h
        lutTable.h)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(predic ${SOURCE_FILES})
    target_link_libraries(predic ${Boost_LIBRARIES} ${OpenCV_LIBS})
endif ()



