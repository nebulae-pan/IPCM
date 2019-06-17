# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        ipcm

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/ipcm.h
        src/main/cpp/ipcm.cpp
        src/main/cpp/thread_lock.h
        src/main/cpp/thread_lock.cpp
        src/main/cpp/logger.h
        src/main/cpp/logger.cpp
        src/main/cpp/mmap_file.h
        src/main/cpp/mmap_file.cpp
        src/main/cpp/file_lock.h
        src/main/cpp/file_lock.cpp
        src/main/cpp/ipc_buffer.h
        src/main/cpp/ipc_buffer.cpp
        src/main/cpp/out_data.h
        src/main/cpp/out_data.cpp
        src/main/cpp/constant.h
        src/main/cpp/bridge.cpp
        src/main/cpp/openssl/aes.h
        src/main/cpp/openssl/aes_cfb.c
        src/main/cpp/openssl/aes_core.c
        src/main/cpp/openssl/aes_locl.h
        src/main/cpp/openssl/cfb128.c
        src/main/cpp/openssl/md5.h
        src/main/cpp/openssl/md5_dgst.c
        src/main/cpp/openssl/md5_locl.h
        src/main/cpp/openssl/md5_one.c
        src/main/cpp/openssl/md32_common.h
        src/main/cpp/openssl/modes.h
        src/main/cpp/openssl/opensslconf.h
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ipcm

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        z )