cmake_minimum_required(VERSION 3.19)

project(
    DataStructures
    VERSION     1.1.0
    DESCRIPTION "Algorithms and Data Structures"
    LANGUAGES   CXX
)

# Find Doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # Note the option ALL which allows to build the docs together with the application
    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    # Add docs to clean target
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES 
        "${CMAKE_CURRENT_BINARY_DIR}/docs")
else ()
    message("Doxygen not found, documentation will not be generated")
endif ()

add_subdirectory(complexities)

add_subdirectory(libds)

add_subdirectory(tests)

add_subdirectory(cli)

add_subdirectory(lab3)

add_subdirectory(sem)
