cmake_minimum_required(VERSION 3.19)

project(
    DataStructures
    VERSION     1.1.0
    DESCRIPTION "Algorithms and Data Structures"
    LANGUAGES   CXX
)

add_subdirectory(complexities)

add_subdirectory(libds)

add_subdirectory(tests)

add_subdirectory(cli)

add_subdirectory(lab3)

add_subdirectory(sem)
