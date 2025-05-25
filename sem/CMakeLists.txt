add_executable(sem 
    main.cpp 
    bus_stop.cpp 
    bus_stop_node.cpp 
    predicates.cpp 
    bus_stop_filter.cpp
)

target_compile_features(sem PUBLIC cxx_std_17)

target_include_directories(sem PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(sem
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        NO
)
