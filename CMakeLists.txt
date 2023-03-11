cmake_minimum_required(VERSION 3.16)

project(pong VERSION 1.0.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "C:/Projects/llvm-project/build/bin/include-what-you-use.exe")
# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Include and run the conan package manager to install some external libraries for us
# Requires Python or Conan on the path
# include(conan)
# conan_cmake_autodetect(settings)
# conan_cmake_install(
#     PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
#     BUILD missing
#     REMOTE conancenter
#     SETTINGS ${settings}
#     )

# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()
# Include Qt deployment helper function
include(windeployqt)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS REQUIRED
    Core
    Gui
    Widgets
    OpenGLWidgets
    Xml
    Charts
    )

qt_add_executable(pong WIN32
    main.cpp
    qtpong.cpp
    mainwindow.cpp
)

target_link_libraries(pong PRIVATE
    OpenGL32
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Xml
    Qt6::Charts
    ${CONAN_LIBS}
)
windeployqt(pong)

# add_subdirectory(tests)
