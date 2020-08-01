cmake_minimum_required(VERSION 3.2)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(ProjectName TestCixel)
project(${ProjectName})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(USE_C)
    set(SOURCES "main.c;test.c;../cixel.h")
else()
    set(SOURCES "main.cpp;test.cpp;../cixel.h")
endif()

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(AFTER "${CMAKE_CURRENT_SOURCE_DIR}/../")

source_group("include" FILES ${HEADERS})
source_group("src" FILES ${SOURCES})

set(FILES ${HEADERS} ${SOURCES})

set(OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

add_executable(${ProjectName} ${FILES})

if(MSVC)
    set(DEFAULT_CXX_FLAGS "/DWIN32 /D_WINDOWS /D_MBCS /W4 /WX- /nologo /fp:precise /arch:AVX /Zc:wchar_t /TP /Gd")
    if("1800" VERSION_LESS MSVC_VERSION)
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /EHsc")
    endif()

    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1 /Gy /GR- /GS /Gm-")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /GL /GR- /DNDEBUG")

    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
elseif(UNIX)
    # set configuration by "cmake -DCMAKE_BUILD_TYPE=Release"
    if(USE_C)
        set(CMAKE_C_FLAGS "-Wall -std=c11 -std=gnu11 -march=native")
        set(CMAKE_C_FLAGS_DEBUG "-O0")
        set(CMAKE_C_FLAGS_RELEASE "-O2")
    else()
        set(DEFAULT_CXX_FLAGS "-Wall -O2 -std=c++11 -std=gnu++11 -march=native")
        set(CMAKE_C_FLAGS "${DEFAULT_CXX_FLAGS}")
    endif()
elseif(APPLE)
endif()

set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG "${ProjectName}" OUTPUT_NAME_RELEASE "${ProjectName}")

