find_library( # Sets the name of the path variable.  设置要使用的系统库的变量名
        log-lib

        # Specifies the name of the NDK library that   指定NDK的库名
        # you want CMake to locate.
        log)

add_subdirectory(demo)

add_library( # Sets the name of the library. 设置库名称
        myapplicationcpp
        # Sets the library as a shared library. 设置为动态库
        SHARED
        # Provides a relative path to your source file(s).  指定源文件
        native-lib.cpp)

add_library( # Sets the name of the library. 设置库名称
        logtest
        # Sets the library as a shared library. 设置为动态库
        SHARED
        # Provides a relative path to your source file(s).  指定源文件
        test-lib.cpp)

# add_library(myapplicationcpp)

target_link_libraries( # Specifies the target library.
        myapplicationcpp
        ${log-lib}
        )

target_link_libraries( # Specifies the target library.
        logtest
        demo
        ${log-lib}
        )