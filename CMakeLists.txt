cmake_minimum_required(VERSION 2.8)
project(DUORead)


# Get DUO_SDK install path from environment variable if set
if (DEFINED ENV{DUO_SDK})
    set(DUO_SDK $ENV{DUO_SDK})
else ()
    set(DUO_SDK ${PROJECT_SOURCE_DIR}/SDK)
endif ()
message("SDK dir${DUO_SDK}")
# Add DUO include
include_directories(${DUO_SDK}/include)

# Detect Compiler / Arch and add libraries
if (WIN32)
    if (CMAKE_CL_64)
        set(DUOLib_ARCH x64)
    else ()
        set(DUOLib_ARCH x86)
    endif ()
    find_library(DUO_LIBRARY DUOLib HINTS ${DUO_SDK}/windows/${DUOLib_ARCH})

    # Copy DUOLib.dll
    file(TO_NATIVE_PATH ${DUO_SDK}/windows/${DUOLib_ARCH}/DUOLib.dll SRC)
    file(TO_NATIVE_PATH ${CMAKE_BINARY_DIR}/../bin/${DUOLib_ARCH}/ DST)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})

    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY
            ${PROJECT_BINARY_DIR}/../bin/${DUOLib_ARCH}
            CACHE PATH
            "Single Directory for all Libraries"
            )

elseif (APPLE)
    set(DUOLib_ARCH x64)
    find_library(DUO_LIBRARY DUO HINTS ${DUO_SDK}/osx/${DUOLib_ARCH})
elseif (UNIX)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL armv7l OR CMAKE_SYSTEM_PROCESSOR STREQUAL aarch64)
        set(DUOLib_ARCH arm)
    else ()
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(DUOLib_ARCH x64)
        else ()
            set(DUOLib_ARCH x86)
        endif ()
    endif ()
    find_library(DUO_LIBRARY DUO HINTS ${DUO_SDK}/linux/${DUOLib_ARCH})
endif ()

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${DUOLib_ARCH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/${DUOLib_ARCH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/${DUOLib_ARCH})

include_directories(src)

add_executable(DUORead src/DUORead.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(DUORead ${DUO_LIBRARY} ${OpenCV_LIBS})

add_executable(test_DUOReader Example/testDUOReader.cpp
        src/DUOReader.cpp
        )

target_link_libraries(test_DUOReader ${DUO_LIBRARY} ${OpenCV_LIBS})


add_executable(recordDUO Example/DataRecord.cpp
        src/DUOReader.cpp
        )

target_link_libraries(recordDUO ${DUO_LIBRARY} ${OpenCV_LIBS})