cmake_minimum_required(VERSION 3.28)
project(RTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(RTree main.cpp
        main.cpp
        RTree.h
        RTree.cpp
        Node.h
        Node.cpp
        DataProcessor.h
        DataProcessor.cpp
        #        RTree.cpp
#        RTree2.h
        )
