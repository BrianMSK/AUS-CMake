add_library(tests SHARED
    _details/console_output.cpp
    _details/console.cpp
    _details/test.cpp
)

target_link_libraries(tests PRIVATE ds)

set_target_properties(ds
    PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS        NO
)
