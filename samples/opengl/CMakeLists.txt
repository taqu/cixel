cmake_minimum_required(VERSION 3.15)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")
set(ProjectName "opengl")
project(${ProjectName})
include_directories(AFTER "${CMAKE_CURRENT_SOURCE_DIR}/../..")

if(MSVC)
    include_directories(AFTER "D:\\glew-2.1.0\\include")
    include_directories(AFTER "D:\\glfw-3.3.2\\include")
    include_directories(AFTER "D:\\glm-0.9.9.8\\")
    link_directories("D:\\glew-2.1.0\\lib\\Release\\x64")
    link_directories("D:\\glfw-3.3.2\\lib-vc2019")
endif()

set(SOURCES "triangle.cpp;../shader.cpp")

set(FILES ${SOURCES})

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
    target_link_libraries(${ProjectName} glfw glew32 opengl32)
else()
    set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -std=gnu++11 -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    target_link_libraries(${ProjectName} glfw GLEW GL)
endif()
